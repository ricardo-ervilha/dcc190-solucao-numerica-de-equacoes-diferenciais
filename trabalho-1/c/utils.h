#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include "parameters.h"

/*Arquivo com funções úteis/essenciais para o programa*/

//Inicializa um vetor com zeros.
void zeros(real* vet, int size){
    for(int i = 0; i < size; i++)
        vet[i] = 0.0;
}

//Imprime os valores de um vetor qualquer.
void print_vet(real* vet, int size){
    for(int i = 0; i < size; i++){
        printf("%f ", vet[i]);
    }
    printf("\n");
}

//Copia os valores da origem para o destino.
void* copy(real* origin, real* destination, int size){
    for(int i = 0; i < size; i++)
        destination[i] = origin[i];
}

//Calcula a norma infinito de um vetor qualquer.
real norm(real* vet, int size){
    real max = -HUGE_VAL; //define como -INF
    for(int i = 0; i < size; i++){
        if(vet[i] > max)
            max = vet[i];
    }

    return max;
}

//Retorna um novo vetor contendo a subtração de dois vetores, armazenando no terceiro vetor.
void sub_vectors(real* v1, real*v2, real* v3, int size){
    for(int i = 0; i < size; i++)
        v3[i] = v1[i] - v2[i];
}

//Dado um índice i e um índice j, retorna qual o índice no vetor solução.
int idx(int i, int j){
    return i * Ix + j; // cada i está associado a Ix posições.
}

// Salva os valores finais obtidos em filename. Os valores são salvos no formato .csv
void export_output(char* filename, real* output, int size){
    FILE *fptr = fopen(filename, "w");
    if(fptr == NULL){
        printf("Erro ao criar/abrir arquivo.\n");
    }

    for(int i = 0; i < size; i++){
        fprintf(fptr, "%f", output[i]);
        if(i < size-1)
            fprintf(fptr, ",");
    }

    fclose(fptr);
}

#endif

