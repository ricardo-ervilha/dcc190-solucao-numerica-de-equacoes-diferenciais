#include "utils.h"
#include <time.h>

// kernel to solve PDE
__global__ void solver(real *T_n, real* T_np1){
    //aux variables to compute k's and indexes
    int j_p, j_m, i_p, i_m;
    real k_zp, k_zm, k_xp, k_xm;

    int j = threadIdx.x + blockIdx.x * blockDim.x;
    int i = threadIdx.y + blockIdx.y * blockDim.y;

    j_p = (j == tamz - 1) ? (j - 1) : (j + 1);
    j_m = (j == 0) ? (j + 1) : (j - 1);
    i_p = (i == tamx - 1) ? (i - 1) : (i + 1);
    i_m = i - 1;

    k_zp = k_harm(k(j, i), k(j_p, i));
    k_zm = k_harm(k(j, i), k(j_m, i));
    k_xp = k_harm(k(j, i), k(j, i_p));
    k_xm = k_harm(k(j, i), k(j, i_m));

    T_np1[j][i] = T_n[j][i] + (ht / (rho(j,i) * c(j,i) * h * h)) * (k_xp * T_n[j][i_p] - (k_xp + k_xm) * T_n[j][i] + k_xm * T_n[j][i_m] + k_zp * T_n[j_p][i] - (k_zp + k_zm) * T_n[j][i] + k_zm * T_n[j_m][i] + h * h * (omega_b(j, i, T_n[j][i]) * c_b(j,i)*(T_a - T_n[j][i]) + Q_m(j,i) + Q_r[j][i]));  
}

int main(int argc, char* argv[]){
    /*Alocating memory*/
    read_config_txt("../inout/config.txt");
    init_vars();

    /*Printing program infos.*/
    printf("Discretização espacial: %lf\n", h);
    printf("Discretização temporal: %lf\n", ht);
    printf("Número de timesteps: %d\n", tamt);

    /*Cuda Transfer Data*/

    //Alocates in host
    real **T_n = (real**) alloc_matrix(tamz, tamx, sizeof(real), &T_a); // initiliazes T^n with left boundary condition.
    real **T_np1 = (real**) alloc_matrix(tamz, tamx, sizeof(real), &T_a); // initiliazes T^{n+1} with left boundary condition.
    real *T_ndevice, *T_np1device;
    
    const size_t sizeMatrix = sizeof(real) * size_t(tamz * tamx);
    
    //Allocates in device
    cudaMalloc((void**)&T_ndevice, T_n);
    cudaMalloc((void**)&T_np1device, T_np1);

    //Transfer data to device
    cudaMemcpy(T_ndevice,   T_n,    sizeMatrix, cudaMemcpyHostToDevice); 
    cudaMemcpy(T_np1device, T_np1,  sizeMatrix, cudaMemcpyHostToDevice); 

    /*Call the kernel to compute the finite difference*/
    solver<<tamz, tamx>>(T_ndevice, T_np1device);
    
    /*Desalocating memory*/
    free_matrix((void**) T_np1);
    free_matrix((void**) T_n);

    end_vars();
}