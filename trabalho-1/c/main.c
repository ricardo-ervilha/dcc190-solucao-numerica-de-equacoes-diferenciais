#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parameters.h"

//------------------------Variáveis do Problema e De Execução------------------------------
int Ix, Iy;
real x_0, x_f, y_0, y_f;
real epsilon;  
int iter;
real T_a;
real u_top, u_left, u_right, u_bottom;
real *vet, *x_kp1;
//----------------------------------------------------------------------------------------

//------------------------------------Funções-------------------------------------------
/*
Estrutura do Arquivo de Configurações:
Ix Iy (integers)
x_0 x_f y_0 y_f (real)
epsilon (real)
iter (integer)
T_a (real)
u_top u_right u_bottom u_left (real)
*/
void fill_values(char* filename){
    FILE *fptr;
    
    fptr = fopen(filename, "r");
    
    if(fptr == NULL) {
        printf("Ocorreu um erro durante a abertura do arquivo.");
    }
    
    if (fscanf(fptr, "%d %d %f %f %f %f %f %d %f %f %f %f %f",
               &Ix, &Iy, &x_0, &x_f, &y_0, &y_f, &epsilon, &iter,
               &T_a, &u_top, &u_right, &u_bottom, &u_left) != 13) {
        printf("Erro ao ler os dados do arquivo.\n");
        fclose(fptr);
    }

    fclose(fptr);
}

/*Salva os valores obtidos em filename.*/
void export_outputs(char* filename, int size){
    FILE *fptr = fopen(filename, "w");
    if(fptr == NULL){
        printf("Erro ao criar/abrir arquivo.");
    }

    for(int i = 0; i < size; i++){
        fprintf(fptr, "%f", vet[i]);
        if(i < size-1)
            fprintf(fptr, ",");
    }

    fclose(fptr);
}

/*Função intermediária para retornar os valores de x_k considerando o contorno do problema*/
real x_k(int i, int j){  //REVIEW: Considerar NEUMANN
    if(i == 0 && j > 0)
        return u_top;
    else if(i > 0 && j == 0)
        return u_left;
    else if(i == Ix-1 && j > 0)
        return u_right;
    else if(i > 0 && j == Iy-1)
        return u_bottom;
    else
        return vet[j*(i+1)];
}

//----------------------------------------------------------------------------------------


int main(int argc, char* argv[]){

    /*Preenche variáveis baseado no arquivo de configurações*/
    fill_values("../inout/config.txt"); // REVIEW: Passar como parâmetro no terminal.

    /*Variáveis que estão em função das variáveis globais ou variáveis extras.*/
    real dx, dy, dx2, dy2; 
    real num, den;
    real error;
    real size;
    real x, y;

    /*Preenchendo os valores*/
    dx = (x_f - x_0) / Ix; dy = (y_f - y_0) / Iy;
    dx2 = dx * dx; dy2 = dy * dy;
    error = HUGE_VAL; // equivalente a +INF
    size = (Ix-2)*(Iy-2); // REVIEW: Desconsiderando as bordas (!!!)

    /*Inicialização dos vetores que irão guardar as soluções de passo k e k+1*/
    vet = (real*) malloc(size * sizeof(real));
    x_kp1 = (real*) malloc(size * sizeof(real)); 
    // Para acessar um valor i, j no vetor: i*j + j = j*(i+1)

    /*Inicialmente, x_k representa nosso chute inicial, que nesse caso será um vetor somente com zeros.*/
    for(int i = 0; i < size; i++){
        vet[i] = 0;
    }

    /*LOOP PRINCIPAL: Gauss-Seidel*/
    while(iter > 0){
        
        //Atualização do x_kp1 baseado nas nossas discretizações e no x_k;
        for(int i = 1; i < Ix-1; i++){
            x = x_0 + i*dx;
            for(int j = 1; j < Iy-1; j++){
                y = y_0 + j*dy;
                num = dy2 * k(x,y) * x_k(i-1, j) + dx2 * k(x,y) * x_k(i, j-1) + dy2 * k(x,y) * x_k(i+1, j) + dx2 * k(x,y) * x_k(i, j+1) + omega_b(x,y) * rho_b(x,y) * c_b(x,y) * T_a + Q_m(x,y);

                den = dy2*(k(x,y) + k(x,y)) + dx2*(k(x,y) + k(x,y)) + omega_b(x,y) * rho_b(x,y) * c_b(x,y); 

                x_kp1[j*(i+1)] = num / den;

            }
        }

        copy(vet, x_kp1, size); 
        iter--;
    }

    export_outputs("../inout/output.csv", size);

    printf("Conclusão completa\n");

    /*Libera a memória alocada*/
    free(vet);
    free(x_kp1);

    return 0;
}