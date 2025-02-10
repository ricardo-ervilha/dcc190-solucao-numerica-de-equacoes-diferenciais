#include "utils.h"
#include <time.h>


void handleCudaError(cudaError_t cudaERR){
    if (cudaERR!=cudaSuccess){
      printf("CUDA ERROR : %s\n", cudaGetErrorString(cudaERR));
    }
}

// Computes for each thread the T_np1_{i,j}.
__global__ void solver(real *T_n, real *T_np1)
{
    int j = threadIdx.x + blockIdx.x * blockDim.x;
    int i = threadIdx.y + blockIdx.y * blockDim.y;

    int j_p = (j == tamz - 1) ? (j - 1) : (j + 1);
    int j_m = (j == 0) ? (j + 1) : (j - 1);
    int i_p = (i == tamx - 1) ? (i - 1) : (i + 1);
    int i_m = i - 1;

    real k_zp = k_harm(k(j, i), k(j_p, i));
    real k_zm = k_harm(k(j, i), k(j_m, i));
    real k_xp = k_harm(k(j, i), k(j, i_p));
    real k_xm = k_harm(k(j, i), k(j, i_m));

    T_np1[j * tamx + i] = T_n[j * tamx + i] + (ht / (rho_gpu(j,i) * c_gpu(j,i) * h * h)) * (k_xp * T_n[j*tamx + i_p] - (k_xp + k_xm) * T_n[j * tamx + i] + k_xm * T_n[j * tamx + i_m] + k_zp * T_n[j_p * tamx + i] - (k_zp + k_zm) * T_n[j * tamx + i] + k_zm * T_n[j_m * tamx + i] + h * h * (omega_b_gpu(j, i, T_n[j * tamx + i]) * c_b_gpu(j,i)*(T_a - T_n[j * tamx + i]) + Q_m_gpu(j,i) + Q_r[j * tamx + i]));   
}

int main(int argc, char* argv[]){
    read_config_txt("../inout/config.txt");
    init_vars();

    printf("Discretização espacial: %lf\n", h);
    printf("Discretização temporal: %lf\n", ht);
    printf("Número de timesteps: %d\n", tamt);

    clock_t start, end;

    /*Transfer Data from Host to Device*/
    real **T_n = (real**) alloc_matrix(tamz, tamx, sizeof(real), &T_a); // initiliazes T^n with left boundary condition.
    real **T_np1 = (real**) alloc_matrix(tamz, tamx, sizeof(real), &T_a); // initiliazes T^{n+1} with left boundary condition.

    const size_t matrixSize = sizeof(real) * size_t(tamz * tamx); 

    real *Tn_device, *Tnp1_device, *Qr_device; //pointers to access data in device
    
    //allocate poiinters in device
    handleCudaError(cudaMalloc((void**)&Tn_device, matrixSize));
    handleCudaError(cudaMalloc((void**)&Tnp1_device, matrixSize));
    handleCudaError(cudaMalloc((void**)&Qr_device, matrixSize));

    //transfer data to device
    handleCudaError(cudaMemcpy(Tn_device,   T_n,    matrixSize, cudaMemcpyHostToDevice));
    handleCudaError(cudaMemcpy(Tnp1_device, T_np1,  matrixSize, cudaMemcpyHostToDevice));
    handleCudaError(cudaMemcpy(Qr_device,   Q_r,    matrixSize, cudaMemcpyHostToDevice));

    dim3 blockSize(16, 16);
    dim3 gridSize((tamx + blockSize.x - 1) / blockSize.x, (tamz + blockSize.y - 1) / blockSize.y);

    start = clock();
    for(int z = 0; z < tamt; z++){
        solver<<gridSize, blockSize>>(Tn_device, Tnp1_device); //calculates a timestep
        cudaDeviceSynchronize(); //sincroniza threads após realização dos cálculos;
        real* temp = Tn_device;
        Tn_device = Tnp1_device;
        Tnp1_device = temp;
    }
    end = clock();

    real elapsed_time= (double) (end - start) / CLOCKS_PER_SEC;
    printf("Tempo paralelo: \t %f.\n", elapsed_time);

    handleCudaError(cudaMemcpy(T_n,     Tn_device,      matrixSize, cudaMemcpyDeviceToHost));
    handleCudaError(cudaMemcpy(T_np1,   Tnp1_device,    matrixSize, cudaMemcpyDeviceToHost));
    handleCudaError(cudaMemcpy(Q_r,     Qr_device,    matrixSize, cudaMemcpyDeviceToHost));

    free_matrix((void**) T_np1);
    free_matrix((void**) T_n);
    cudaFree(Tn_device);
    cudaFree(Tnp1_device);
    cudaFree(Qr_device);

    end_vars();
}