#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include "parameters.h"

/*Arquivo com funções úteis/essenciais para o programa*/
real* arange(real min, real max, real step){
    int size = (max - min) / step + 1;
    real* vet = (real*) malloc(size * sizeof(real));

    for(int i = 0; i < size; i++)
        vet[i] = min + i * step;

    return vet;
}

real** alloc_matrix(int tam1, int tam2, int t){
    real** matrix = (real**) malloc(tam1 * sizeof(real*));

    for(int i = 0; i < tam1; i++){
        matrix[i] = (real*) malloc(tam2 * sizeof(real));
        for(int j = 0; j < tam2; j++){
            matrix[i][j] = t;
        }
    }

    return matrix;
}

void free_matrix(real** matrix) {
    for (int i = 0; i < tamz; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void print_array(real* array, int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%f", array[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

void print_matrix(real** matrix){
    for(int j = 0; j < tamz; j++){
        for(int i = 0; i < tamx; i++){
            printf("%lf ", matrix[j][i]);
        }
        printf("\n");
    }
}

void print_variables() {
    printf("Variáveis:\n");
    printf("h: %f\n", h);
    printf("y_fix: %f\n", y_fix);
    printf("muscle_thick: %f\n", muscle_thick);
    printf("fat_thick: %f\n", fat_thick);
    printf("dermis_thick: %f\n", dermis_thick);
    printf("xt1: %f, yt1: %f, zt1: %f, rt1: %f\n", xt1, yt1, zt1, rt1);
    printf("xt2: %f, yt2: %f, zt2: %f, rt2: %f\n", xt2, yt2, zt2, rt2);
    printf("x0: %f, xf: %f, z0: %f, zf: %f\n", x0, xf, z0, zf);
    printf("tol: %f\n", tol);
    printf("max_iter: %d\n", max_iter);
    printf("T_a: %f\n", T_a);
    printf("T_thresh1: %f, T_thresh2: %f\n", T_thresh1, T_thresh2);
    printf("tamx: %d, tamz: %d\n", tamx, tamz);

    // Imprimindo os arrays
    printf("Array x: ");
    print_array(x, tamx);
    printf("Array z: ");
    print_array(z, tamz);
}

void init_vars(){
    tamx = (xf - x0)/h + 1; tamz = (zf - z0)/h + 1;
    x = arange(x0, xf, h);
    z = arange(z0, zf, h);
}

//-----------------------------------------------------------------------------------------
//Parte do Tecido
int get_values_tissue(real x, real z){
    if(inside_tumor(x, z) == 1)
        return 4;
    else if(x <= muscle_thick)
        return 3;
    else if(x <= muscle_thick + fat_thick)
        return 2;
    else
        return 1;
}

real** generate_tissue_matrix(){
    real** tissue = alloc_matrix(tamz, tamx, 0);

    for(int j = 0; j < tamz; j++)
        for(int i = 0; i < tamx; i++)
            tissue[j][i] = get_values_tissue(x[i], z[j]);

    return tissue;
}

// Salva os valores finais obtidos em filename. Os valores são salvos no formato .csv
void export_output(char* filename, real** matrix){

    FILE *fptr = fopen(filename, "w");
    
    if(fptr == NULL){
        printf("Erro ao criar/abrir arquivo.\n");
    }

    for (int j = 0; j < tamz; j++) {
        fwrite(matrix[j], sizeof(real), tamx, fptr);
    }

    fclose(fptr);
}

void export_data(char *filename, double time, int num_iters){
    FILE *fptr = fopen(filename, "w");
    
    if(fptr == NULL){
        printf("Erro ao criar/abrir arquivo.\n");
    }

    fprintf(fptr, "%lf\n", h);
    fprintf(fptr, "%.6f\n", time);
    fprintf(fptr, "%d\n", num_iters);  
    
    fclose(fptr);
}

//-----------------------------------------------------------------------------------------
//Parte do Gauss-Seidel

real calculate_error(real** u_new, real ** u){
    real max = -HUGE_VAL;
    real dif;
    for(int j = 0; j < tamz; j++){
        for(int i = 0; i < tamx; i++){
            dif = u_new[j][i] - u[j][i];
            if(dif < 0)
                dif = -dif;
            if(dif > max)
                max = dif; 
        }
    }
    return max;
}

void copy(real** origin, real** destiny){
    for(int j = 0; j < tamz; j++){
        for(int i = 0; i < tamx; i++){
            destiny[j][i] = origin[j][i];
        }
    }
}

//-----------------------------------------------------------------------------------------
//Parte do Gauss Seidel Red Black



#endif

