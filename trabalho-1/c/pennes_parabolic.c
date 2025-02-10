#include "utils.h"
#include <time.h>
#include <omp.h>

void solver(){

    int j_p, j_m, i_p, i_m;
    real k_zp, k_zm, k_xp, k_xm;
    int i,j;

    real **restrict T_n = (real**) alloc_matrix(tamz, tamx, sizeof(real), &T_a); // initiliazes T^n with left boundary condition.
    real **restrict T_np1 = (real**) alloc_matrix(tamz, tamx, sizeof(real), &T_a); // initiliazes T^{n+1} with left boundary condition.
    real **restrict aux;

    // char filepath[256]; // COMENTE PARA RODAR O EXPERIMENTO

    for(int z = 0; z < tamt; z++){ //loop in timesteps

        //loops in spatial coordinates
        #pragma omp parallel for private(i, j_p, j_m, i_p, i_m, k_zp, k_zm, k_xp, k_xm) shared(T_np1, T_n, tamz, tamx)
        for(j = 0; j < tamz; j++){
            for(i = 1; i < tamx; i++){
                
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
        }
        
        // COMENTE O IF ABAIXO PARA RODAR O EXPERIMENTO
        // if(z % snapshot_interval == 0){
        //     printf("Timestep: %d\n", z);
        //     sprintf(filepath, "../inout/snapshots/snapshot_%d.bin", z); // saves each snapshot in the respective folder
        //     export_output(filepath, (void**) T_np1, sizeof(real));
        // }

        aux = T_n;
        T_n = T_np1;
        T_np1 = aux;
    }

    free_matrix((void**) T_np1);
    free_matrix((void**) T_n);
}

int main(int argc, char* argv[]){
    
    read_config_txt("../inout/config.txt");
    init_vars();

    double start, end;

    //COMENTE PARA RODAR O EXPERIMENTO
    // printf("Discretização espacial: %lf\n", h);
    // printf("Discretização temporal: %lf\n", ht);
    // printf("Número de timesteps: %d\n", tamt);

    start = omp_get_wtime();
    solver();
    end = omp_get_wtime();

    //COMENTE PARA RODAR O EXPERIMENTO
    // printf("Tempo paralelo: \t %f com %d threads.\n", end-start, omp_get_max_threads());
    
    /*Experiment saving time*/

    // COMENTE PARA GERAR A ANIMAÇÃO
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "../inout/parabolic/%d/%g.dat", omp_get_max_threads(), h);

    FILE *f = fopen(filepath, "w");
    if(f){
        fprintf(f, "%lf\n", end-start);
        fclose(f);
    }

    end_vars();
}