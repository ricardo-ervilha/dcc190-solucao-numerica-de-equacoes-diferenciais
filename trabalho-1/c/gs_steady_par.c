#include "utils.h"
#include <time.h>
#include <omp.h>

#define NUM_THREADS 2

real** solver(){
    int iter = 0; //counts iterations
    real error = HUGE_VAL; //variable to stores error
    
    //variables related to finite difference
    int j_p, j_m, i_p, i_m; 
    real k_zp, k_zm, k_xp, k_xm;

    //variables for error
    real tmp;
    real diff;

    real **u = (real**) alloc_matrix(tamz, tamx, sizeof(real), &T_a); //initializes a matrix with T_a values

    int i,j;

    while(error > tol){
        diff = 0;
        #pragma omp parallel num_threads(NUM_THREADS) private(tmp, i, j, j_p, j_m, i_p, i_m, k_zp, k_zm, k_xp, k_xm) reduction(+:diff)
        {
            #pragma omp for
            for(j = 0; j < tamz; j++){
                for(i = (j % 2 == 0 ? 1 : 2); i < tamx; i += 2){
                    
                    tmp = u[j][i]; //stores temp. the old value

                    j_p = (j == tamz - 1) ? (j - 1) : (j + 1);
                    j_m = (j == 0) ? (j + 1) : (j - 1);
                    i_p = (i == tamx - 1) ? (i - 1) : (i + 1);
                    i_m = i - 1;

                    k_zp = k_harm(k(j, i), k(j_p, i));
                    k_zm = k_harm(k(j, i), k(j_m, i));
                    k_xp = k_harm(k(j, i), k(j, i_p));
                    k_xm = k_harm(k(j, i), k(j, i_m));

                    u[j][i] = (
                        (k_zp * u[j_p][i] + k_zm * u[j_m][i] + k_xp * u[j][i_p] + k_xm * u[j][i_m]) / (h * h)
                        + omega_b(j, i, u[j][i]) * c_b(j, i) * T_a + Q_m(j, i) + Q_r[j][i]
                    ) / (
                        (k_zp + k_zm + k_xp + k_xm) / (h * h)
                        + omega_b(j, i, u[j][i]) * c_b(j, i)
                    );
                    
                    diff += fabs(u[j][i] - tmp); //update error
                    
                }
            }
            #pragma omp barrier
        }

        #pragma omp parallel num_threads(NUM_THREADS) private(tmp, i, j, j_p, j_m, i_p, i_m, k_zp, k_zm, k_xp, k_xm) reduction(+:diff)
        {
            #pragma omp for
            for(j = 0; j < tamz; j++){
                for(i = (j % 2 == 0 ? 2 : 1); i < tamx; i += 2){
                    
                    tmp = u[j][i]; //stores temp. the old value

                    j_p = (j == tamz - 1) ? (j - 1) : (j + 1);
                    j_m = (j == 0) ? (j + 1) : (j - 1);
                    i_p = (i == tamx - 1) ? (i - 1) : (i + 1);
                    i_m = i - 1;

                    k_zp = k_harm(k(j, i), k(j_p, i));
                    k_zm = k_harm(k(j, i), k(j_m, i));
                    k_xp = k_harm(k(j, i), k(j, i_p));
                    k_xm = k_harm(k(j, i), k(j, i_m));

                    u[j][i] = (
                        (k_zp * u[j_p][i] + k_zm * u[j_m][i] + k_xp * u[j][i_p] + k_xm * u[j][i_m]) / (h * h)
                        + omega_b(j, i, u[j][i]) * c_b(j, i) * T_a + Q_m(j, i) + Q_r[j][i]
                    ) / (
                        (k_zp + k_zm + k_xp + k_xm) / (h * h)
                        + omega_b(j, i, u[j][i]) * c_b(j, i)
                    );
                    
                    diff += fabs(u[j][i] - tmp); //update error
                    
                }
            }
            #pragma omp barrier
        }
        
        error = diff;
        iter += 1;

        printf("#Iteração = %d, com erro = %lf\n", iter, error);
    }

    return u;
}

int main(int argc, char* argv[]){
    read_config_txt("../inout/config.txt"); //read values from config.txt
    init_vars();

    omp_set_num_threads(NUM_THREADS);

    clock_t start, end;
    real cpu_time_used;

    start = omp_get_wtime();
    real **u = solver();
    end = omp_get_wtime();

    cpu_time_used = end - start;

    printf("=>Tempo gasto: %f\n", cpu_time_used);

    export_output("../inout/steady/steady_par.bin", (void**) u, sizeof(real));
    end_vars();

    return 0;
}
