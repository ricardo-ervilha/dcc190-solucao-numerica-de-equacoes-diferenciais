#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <string.h>
#include "parameters.h"

/*Arquivo com funções utilizadas no programa principal.*/

/*-----------------------------------Funções de Propósito Geral----------------------------------*/
//Gera o arange dos pontos
real* arange(real min, real max, real step){
    int size = (max - min) / step + 1;
    real* vet = (real*) malloc(size * sizeof(real));
    for(int i = 0; i < size; i++)
        vet[i] = min + i * step;
    return vet;
}

//Aloca uma matriz com tipo genérico
void** alloc_matrix(int tam1, int tam2, size_t size_of_type, void* initial_value) {
    void** matrix = (void**) malloc(tam1 * sizeof(void*));

    for (int i = 0; i < tam1; i++) {
        matrix[i] = malloc(tam2 * size_of_type);
        
        for (size_t j = 0; j < tam2; j++) {
            memcpy((char*)matrix[i] + j * size_of_type, initial_value, size_of_type);
        }
    }
    
    return matrix;
}

//Desaloca os dados de uma matriz com tipo genérico
void free_matrix(void** matrix){
    for (int i = 0; i < tamz; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void print_array(void *array, int size, size_t element_size){
    for (int i = 0; i < size; i++) {
        if (element_size == sizeof(int)) {
            printf("%d", ((int*)array)[i]);
        } else if (element_size == sizeof(real)) {
            printf("%f", ((real*)array)[i]);
        }
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("\n");
}

void print_matrix(void **matrix, size_t element_size){
    for(int j = 0; j < tamz; j++){
        for(int i = 0; i < tamx; i++){
            if (element_size == sizeof(int)) {
                printf("%d", ((int**)matrix)[j][i]);
            }else{
                printf("%lf", ((real**)matrix)[j][i]);
            }
        }
        printf("\n");
    }
}

/*------------------------------Inicialização da Matriz com Dados do Tecido-----------------*/

/*Atribue um valor para cada tipo de parte do domínio.*/
int get_values_tissue(real x, real z){
    if(is_inside_tumor(x, z) == 1)
        return 1;
    else if(x <= muscle_thick)
        return 2;
    else if(x <= muscle_thick + fat_thick)
        return 3;
    else
        return 4;
}

void generate_tissue_matrix(){
    int initial_value = 0;
    tissue = (int**) alloc_matrix(tamz, tamx, sizeof(int), &initial_value);

    for(int j = 0; j < tamz; j++)
        for(int i = 0; i < tamx; i++)
            tissue[j][i] = get_values_tissue(x[i], z[j]);

    // print_matrix((void**)tissue, sizeof(int));
}

/*------------------------------Inicialização da Matriz de Hipertermia---------------------------*/

real get_values_hyperthermia(real x, real z){
    real Q_1 = Ai1 * exp(- distance(x, y_fix, z, xi1, yi1, zi1) / (ri1*ri1));
    real Q_2 = Ai2 * exp(- distance(x, y_fix, z, xi2, yi2, zi2) / (ri2*ri2));
    real Q_3 = Ai3 * exp(- distance(x, y_fix, z, xi3, yi3, zi3) / (ri3*ri3));
    return Q_1 + Q_2 + Q_3;
}

void generate_hyperthermia_matrix(){
    real initial_value = 0;
    Q_r = (real**) alloc_matrix(tamz, tamx, sizeof(real), &initial_value);

    for(int j = 0; j < tamz; j++)
        for(int i = 0; i < tamx; i++)
            Q_r[j][i] = get_values_hyperthermia(x[i], z[j]);

    print_matrix((void**)Q_r, sizeof(real));
}

/*-----------------------------------Função Para inicializar vars.-------------------------------*/
void init_vars(){
    tamx = (xf - x0)/h + 1; tamz = (zf - z0)/h + 1;
   
    x = arange(x0, xf, h);
    z = arange(z0, zf, h);

    // /*Inicialização dos dados da hipertermia*/
    xi1=0.02; yi1=y_fix; zi1=0.055; Ai1=0.8e6; ri1=0.6e-3;
    xi2=0.025; yi2=y_fix; zi2=0.045; Ai2=0.7e6; ri2=0.6e-3;
    xi3=0.015; yi3=y_fix; zi3=0.04; Ai3=0.7e6; ri3=0.6e-3;

    generate_tissue_matrix(); //obtém a matriz com os valores para melhorar desempenho depois
    generate_hyperthermia_matrix(); //obtém a matriz com os valores calculados da hipertermia para melhorar desempenho depois
}

void end_vars(){
    free(x);
    free(z);
    free_matrix((void**)tissue);
    free_matrix((void**)Q_r);
}

/*----------Função para salvar a matriz com a resposta final em binário--------------------------*/
void export_output(char* filename, void** matrix, size_t element_size){

    FILE *fptr = fopen(filename, "w");
    
    if(fptr == NULL){
        printf("Error during file creation. Error at line 133 on utils.h\n");
    }

    for (int j = 0; j < tamz; j++) {
        fwrite(matrix[j], element_size, tamx, fptr);
    }

    fclose(fptr);
}

// void export_data(char *filename, real time, int num_iters){
//     FILE *fptr = fopen(filename, "w");
    
//     if(fptr == NULL){
//         printf("Erro ao criar/abrir arquivo.\n");
//     }

//     fprintf(fptr, "%lf\n", h);
//     fprintf(fptr, "%.6f\n", time);
//     fprintf(fptr, "%d\n", num_iters);  
    
//     fclose(fptr);
// }

#endif