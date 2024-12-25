#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <math.h>

#define real float // dica do Ruy em Algoritmos 1 :)


#define x_center 0.02
#define y_center 0.05

int inside_circle(real x, real y){
    return sqrt( pow(x-x_center, 2) + pow(y-y_center, 2) ) <= 0.013;
}

/*Calcula o valor de Q_m, a geração de calor metabólica*/
real Q_m(real x, real y){
    if(inside_circle(x, y) == 1){
        return 4200.0;
    } //dentro do tumor
    else if(x < 0.038) //fora do tumor mas dentro do músculo
        return 420.0;
    else if(x < 0.048) //fora do tumor mas dentro da gordura
        return 420.0;
    else //fora do tumor mas dentro da derme
        return 420.0;
}

/*Calcula o valor de c_b, calor específico do sangue*/
real c_b(real x, real y){
    if(inside_circle(x, y) == 1) //dentro do tumor
        return 4200.0;
    else if(x < 0.038) //fora do tumor mas dentro do músculo
        return 4200.0;
    else if(x < 0.048) //fora do tumor mas dentro da gordura
        return 4200.0;
    else //fora do tumor mas dentro da derme
        return 4200.0;
}

/*Calcula o valor de rho_b, a densidade sanguínea*/
real rho_b(real x, real y){
    if(inside_circle(x, y) == 1) //dentro do tumor
        return 1000.0;
    else if(x < 0.038) //fora do tumor mas dentro do músculo
        return 1000.0;
    else if(x < 0.048) //fora do tumor mas dentro da gordura
        return 1000.0;
    else //fora do tumor mas dentro da derme
        return 1000.0;
}

/*Calcula o valor de omega_b, a taxa de perfusão sanguínea*/
real omega_b(real x, real y){
    if(inside_circle(x, y) == 1) //dentro do tumor
        return 4200.0;
    else if(x < 0.038) //fora do tumor mas dentro do músculo
        return 420.0;
    else if(x < 0.048) //fora do tumor mas dentro da gordura
        return 420.0;
    else //fora do tumor mas dentro da derme
        return 420.0;
}

/*Calcula o valor de k, a condutividade térmica do sangue.*/
real k(real x, real y){
    if(inside_circle(x, y) == 1) //dentro do tumor
        return 0.757981;
    else if(x < 0.038) //fora do tumor mas dentro do músculo
        return 1.87895;
    else if(x < 0.048) //fora do tumor mas dentro da gordura
        return 0.504908;
    else //fora do tumor mas dentro da derme
        return 0.5;
}

/*Substitui os valores do vetor de origem usando os do vetor destino. 
Em outras palavras, origem = destino;
Ambos os vetores precisam ter mesmo tamanho.*/
void* copy(real* origin, real* destination, int size){
    for(int i = 0; i < size; i++)
        origin[i] = destination[i];
}


#endif