#include "utils.h"
#include <time.h>
#include <omp.h>
#include <string.h>

// 392 segundos sequencial x 276 segundos paralelo 4 threads.

//Dentro da pasta c (aparentemente está dando metade)
// gcc -Wall -o ../exec/gsrb gsrb.c -O3 -lm -fopenmp && ../exec/gsrb

int main(int argc, char* argv[]){

    int NUM_THREADS = atoi(argv[1]); //numero de threads
    omp_set_num_threads(NUM_THREADS);
    char* fn1 = argv[2]; // arquivo contendo os dados de execução
    char* fn2 = argv[3]; // arquivo contendo a matriz

    fill_values("../inout/config.txt");
    init_vars();

    real** u_new = alloc_matrix(tamz, tamx, T_a);

    int iter = 0;
    int j_p, j_m, i_p, i_m;
    real error = HUGE_VAL;
    real k_zp, k_zm, k_xp, k_xm;
    real val1, val2, val3, val4;
    real diff;
    real tmp;
    int i, j;

    double start, end;
    double cpu_time_used;

    start = omp_get_wtime();
    while(error > tol){
        diff = 0;
        #pragma omp parallel num_threads(NUM_THREADS) private(tmp, i, j, j_p, j_m, i_p, i_m, k_zp, k_zm, k_xp, k_xm, val1, val2, val3, val4) reduction(+:diff)
        {
            #pragma omp for
            for(j = 0; j < tamz; j++){
                for(i = (j % 2 == 0 ? 1 : 2); i < tamx; i += 2){
                    
                    tmp = u_new[j][i];

                    j_p = (j == tamz - 1) ? (j - 1) : (j + 1);
                    j_m = (j == 0) ? (j + 1) : (j - 1);
                    i_p = (i == tamx - 1) ? (i - 1) : (i + 1);
                    i_m = i - 1;

                    k_zp = k_harm(k(x[i], z[j]), k(x[i], z[j_p]));
                    k_zm = k_harm(k(x[i], z[j]), k(x[i], z[j_m]));
                    k_xp = k_harm(k(x[i], z[j]), k(x[i_p], z[j]));
                    k_xm = k_harm(k(x[i], z[j]), k(x[i_m], z[j]));

                    val1 = (k_zp * u_new[j_p][i] + k_zm * u_new[j_m][i] + k_xp * u_new[j][i_p] + k_xm * u_new[j][i_m]) / (h*h);

                    val2 = omega_b(x[i], z[j], u_new[j][i]) * c_b(x[i], z[j]) * T_a + Q_m(x[i], z[j]) + Q_r(x[i], z[j]);
                    
                    val3 = k_zp / (h*h) + k_zm / (h*h) + k_xp / (h*h) + k_xm / (h*h);
                    
                    val4 = omega_b(x[i], z[j], u_new[j][i]) * c_b(x[i], z[j]);
                    
                    u_new[j][i] = (val1 + val2) / (val3 + val4);   

                    diff += fabs(u_new[j][i] - tmp); 

                }
            }  
            #pragma omp barrier
            
        }

        #pragma omp parallel num_threads(NUM_THREADS) private(tmp, i, j, j_p, j_m, i_p, i_m, k_zp, k_zm, k_xp, k_xm, val1, val2, val3, val4) reduction(+:diff)
        {
            #pragma omp for
            for(j = 0; j < tamz; j++){
                for(i = (j % 2 == 0 ? 2 : 1); i < tamx; i += 2){
                    
                    tmp = u_new[j][i];

                    j_p = (j == tamz - 1) ? (j - 1) : (j + 1);
                    j_m = (j == 0) ? (j + 1) : (j - 1);
                    i_p = (i == tamx - 1) ? (i - 1) : (i + 1);
                    i_m = i - 1;

                    k_zp = k_harm(k(x[i], z[j]), k(x[i], z[j_p]));
                    k_zm = k_harm(k(x[i], z[j]), k(x[i], z[j_m]));
                    k_xp = k_harm(k(x[i], z[j]), k(x[i_p], z[j]));
                    k_xm = k_harm(k(x[i], z[j]), k(x[i_m], z[j]));

                    val1 = (k_zp * u_new[j_p][i] + k_zm * u_new[j_m][i] + k_xp * u_new[j][i_p] + k_xm * u_new[j][i_m]) / (h*h);

                    val2 = omega_b(x[i], z[j], u_new[j][i]) * c_b(x[i], z[j]) * T_a + Q_m(x[i], z[j]) + Q_r(x[i], z[j]);
                    
                    val3 = k_zp / (h*h) + k_zm / (h*h) + k_xp / (h*h) + k_xm / (h*h);
                    
                    val4 = omega_b(x[i], z[j], u_new[j][i]) * c_b(x[i], z[j]);
                    
                    u_new[j][i] = (val1 + val2) / (val3 + val4);   

                    diff += fabs(u_new[j][i] - tmp); 
                    
                }
            } 
            #pragma omp barrier 
        }
        iter += 1;
        error = diff;
        
        // if(iter%100 == 0)
            // printf("Iteração = %d, com erro = %lf\n", iter, error);
    }

    end = omp_get_wtime();
    cpu_time_used = (end - start);
    printf("Demorou %d iterações. Erro final: %.10lf\n", iter, error);

    export_output(fn1, u_new);
    export_data(fn2, cpu_time_used, iter);

    free(x);
    free(z);
    free_matrix(u_new);
}