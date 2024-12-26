#include <stdio.h>
#include <stdlib.h>
#include <math.h>
# define M_PI           3.14159265358979323846

void copy(float* origin, float* destination, int size){
    for(int i = 0; i < size; i++)
        destination[i] = origin[i];
}

void export_outputs(char* filename, float* array, int size){
    FILE *fptr = fopen(filename, "w");
    if(fptr == NULL){
        printf("Erro ao criar/abrir arquivo.");
    }

    for(int i = 0; i < size; i++){
        fprintf(fptr, "%f", array[i]);
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
    float u_left, u_top, u_right, u_bottom; //condições de dirichlet
    u_left = 0;
    u_top = 0;
    u_right = 0;
    u_bottom = 0; //neumann

    float uijp, uijm, uipj, uimj;

    int k = 0, k_max = 100000;
    float error = HUGE_VAL, error_max = 1e-8;

    float x_0 = 0, x_f = 1, y_0 = 0, y_f = 1;
    int num_points = 21;
    float h = (x_f - x_0)/(num_points-1);

    float* u = (float*) malloc(num_points * num_points * sizeof(float));

    for(int i = 0; i < num_points*num_points; i++)
        u[i] = 0;

    float* u_new = (float*) malloc(num_points * num_points * sizeof(float));

    float* f = (float*) malloc(num_points * num_points * sizeof(float));

    float x;
    for(int i = 0; i < num_points; i++)
    {
        x = x_0 + i * h;
        for(int j = 0; j < num_points; j++){
            f[get_index(i, j)] = cos(2 * M_PI * x);
            printf(" %f ", f[get_index(i, j)]);
        }
        printf("\n");
    }

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
            // error = 
            copy(u_new, u, num_points*num_points);
        }
        k++;
    }

    export_outputs("../inout/exercise.csv", u, num_points*num_points);
}