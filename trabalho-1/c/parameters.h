#ifndef PARAMETERS_H
#define PARAMETERS_H

#define real float // dica do Ruy em Algoritmos 1 :)

/*Calcula o valor de Q_m, a geração de calor metabólica*/
real Q_m(){
    return 420.0; // retornando o Q_m do músculo
}

/*Calcula o valor de c_b, calor específico do sangue*/
real c_b(){
    return 4200.0; // retornando o c_b do músculo
}

/*Calcula o valor de rho_b, a densidade sanguínea*/
real rho_b(){
    return 1000.0; // retornando o rho_b do músculo
}

/*Calcula o valor de omega_b, a taxa de perfusão sanguínea*/
real omega_b(){
    return 1.87895; // retornando o omega_b do músculo
}

/*Calcula o valor de k, a condutividade térmica do sangue.*/
real k(){
    return 1000.0; // retornando o k do músculo
}

/*Substitui os valores do vetor de origem usando os do vetor destino. 
Em outras palavras, origem = destino;
Ambos os vetores precisam ter mesmo tamanho.*/
void* copy(real* origin, real* destination, int size){
    for(int i = 0; i < size; i++)
        origin[i] = destination[i];
}


#endif