#include "utils.h"
#include <time.h>


real** solver(){
    
    //jacobi variables
    int iter = 0;
    real error = HUGE_VAL;

    int j_p, j_m, i_p, i_m;
    real k_zp, k_zm, k_xp, k_xm;

    real tmp;
    real diff;

    real **u_n = (real**) alloc_matrix(tamz, tamx, sizeof(real), &T_a); // u^n
    real **u_np1_new = (real**) alloc_matrix(tamz, tamx, sizeof(real), &T_a); // u^{n+1}
    real **u_np1_old = (real**) alloc_matrix(tamz, tamx, sizeof(real), &T_a); // cópia anterior do u^{n+1}

    // timesteps
    for(int z = 0; z < tamt; z++){

        //setup jacobi variables...
        copyMatrix(u_np1_old, u_np1_new); // save the current values to calculate error.
        iter = 0;
        error = HUGE_VAL;

        while(error > tol){
            for(int j = 0; j < tamz; j++){
        
                for(int i = 1; i < tamx; i++){
                    j_p = (j == tamz - 1) ? (j - 1) : (j + 1);
                    j_m = (j == 0) ? (j + 1) : (j - 1);
                    i_p = (i == tamx - 1) ? (i - 1) : (i + 1);
                    i_m = i - 1;

                    k_zp = k_harm(k(j, i), k(j_p, i));
                    k_zm = k_harm(k(j, i), k(j_m, i));
                    k_xp = k_harm(k(j, i), k(j, i_p));
                    k_xm = k_harm(k(j, i), k(j, i_m));

                    u_new[j][i] = ;
                }

            }
        }

    }

}

int main(int argc, char* argv[]){
    read_config_txt("../inout/config.txt");
    init_vars();

    clock_t start, end;
    real cpu_time_used;

    start = clock();
    real **u = solver();
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("=>Tempo gasto: %f\n", cpu_time_used);

    export_output("../inout/parabolic/parabolic.bin", (void**) u, sizeof(real));
    end_vars();
}