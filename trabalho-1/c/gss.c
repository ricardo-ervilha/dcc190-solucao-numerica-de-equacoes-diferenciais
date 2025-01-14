#include "utils.h"
#include <time.h>

//Dentro da pasta c
// gcc -Wall -o ../exec/gss gss.c -O3 -lm && ../exec/gss

int main(int argc, char* argv[]){
    char* fn1 = argv[1];
    char* fn2 = argv[2];
    
    fill_values("../inout/config.txt");
    init_vars();

    real** u_new = alloc_matrix(tamz, tamx, T_a);

    int iter = 0;
    real error = HUGE_VAL;
    int j_p, j_m, i_p, i_m;
    real k_zp, k_zm, k_xp, k_xm;
    real val1, val2, val3, val4;

    real tmp;
    real diff;

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    while(error > tol){
        diff = 0;
        for(int j = 0; j < tamz; j++){
            for(int i = 1; i < tamx; i++){

                tmp = u_new[j][i]; //guardando o valor antigo temporariamente

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

        error = diff;
        
        // if(iter%100 == 0)
            // printf("Iteração = %d, com erro = %lf\n", iter, error);
        iter += 1;
        
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // printf("Tempo gasto: %f\n", cpu_time_used);
    printf("Demorou %d iterações. Erro final: %.10lf\n", iter, error);

    export_output(fn1, u_new);
    export_data(fn2, cpu_time_used, iter);

    free(x);
    free(z);
    free_matrix(u_new);
}