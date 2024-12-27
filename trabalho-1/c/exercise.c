#include <stdio.h>
#include <stdlib.h>
#include <math.h>
# define M_PI           3.14159265358979323846

void copy(long double* origin, long double* destination, int size){
    for(int i = 0; i < size; i++)
        destination[i] = origin[i];
}

long double norm(long double* vet, int size){
    long double max = -HUGE_VAL;
    for(int i = 0; i < size; i++){
        if(vet[i] > max)
            max = vet[i];
    }
    return max;
}

long double* sub_vectors(long double* vet1, long double* vet2, int size){
    long double* vet3 = (long double*) malloc(size * sizeof(long double));

    for(int i = 0; i < size; i++){
        vet3[i] = vet1[i] - vet2[i];
    }

    return vet3;
}


void export_outputs(char* filename, long double* array, int size){
    FILE *fptr = fopen(filename, "w");
    if(fptr == NULL){
        printf("Erro ao criar/abrir arquivo.");
    }

    for(int i = 0; i < size; i++){
        fprintf(fptr, "%Lf", array[i]);
        if(i < size-1)
            fprintf(fptr, ",");
    }

    fclose(fptr);
}

int get_index(int i, int j){
    int num_points = 21;
    return i * num_points + j;
}

int main(int argc, char* argv[]){
    long double u_left, u_top, u_right, u_bottom; //condições de dirichlet
    u_left = 0;
    u_top = 0;
    u_right = 0;
    u_bottom = 0; //neumann

    long double uijp, uijm, uipj, uimj;

    int k = 0, k_max = 100000;
    long double error = HUGE_VAL, error_max = 1e-8;

    long double x_0 = 0, x_f = 1, y_0 = 0, y_f = 1;
    int num_points = 21;
    long double h = (x_f - x_0)/(num_points-1);

    long double* u = (long double*) malloc(num_points * num_points * sizeof(long double));

    for(int i = 0; i < num_points*num_points; i++)
        u[i] = 0;

    long double* u_new = (long double*) malloc(num_points * num_points * sizeof(long double));

    long double* f = (long double*) malloc(num_points * num_points * sizeof(long double));

    long double x;
    for(int i = 0; i < num_points; i++)
    {
        x = x_0 + i * h;
        for(int j = 0; j < num_points; j++){
            f[get_index(i, j)] = cos(2 * M_PI * x);
            // printf(" %f ", f[get_index(i, j)]);
        }
        // printf("\n");
    }
    long double *aux;
    while(k < k_max && error > error_max){
        
        for(int i = 0; i < num_points; i++){
            
            for(int j = 0; j < num_points; j++){
                
                if(i == 0)
                    u_new[get_index(i,j)] = u_top;
                else{
                    if(j == num_points-1)
                        uijp = 2*h*u_right + u[get_index(i, num_points-2)];
                    else
                        uijp = u[get_index(i, j+1)];

                    if(j == 0)
                        uijm = 2*h*u_left + u[get_index(i, 1)];
                    else
                        uijm = u[get_index(i, j-1)];

                    if(i == num_points - 1)
                        uipj = 2*h*u_bottom + u[get_index(num_points-2, j)];
                    else
                        uipj = u[get_index(i+1, j)];

                    uimj = u[get_index(i-1, j)];
                    u_new[get_index(i, j)] = (uipj + uimj + uijp + uijm - f[get_index(i,j)]*h*h) / 4.0;
                }
            } 
        }

        aux = sub_vectors(u_new, u, num_points*num_points);
        long double val1 = norm(aux, num_points*num_points);
        long double val2 =  norm(u_new, num_points*num_points);
        // printf("%f %f\n", val1, val2);
        error = val1 / val2;
        k++;
        // printf("Erro: %lf", error);
        copy(u_new, u, num_points*num_points);
        free(aux);
    }
    printf("Valor de k: %d\n", k);
    printf("Valor de error: %.10Lf\n", error);

    export_outputs("../inout/exercise.csv", u, num_points*num_points);
}