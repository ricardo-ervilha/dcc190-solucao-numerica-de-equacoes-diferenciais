#include "parameters.h"
#include "utils.h"

int main(int argc, char* argv[]){

    /*Baseado no arquivo config.txt, preenche os valores de parãmetros do problema.*/
    fill_values("../inout/config.txt"); // REVIEW: Passar como parâmetro no terminal.

    /*----------------------------------Variáveis----------------------------------------*/
    real uipj, uimj, uijp, uijm; // u_i+1,j || u_i-1,j || u_i,j+1 || u_i,j-1
    real dx = (x_f - x_0)/(Ix-1); //delta_x
    real dy = (y_f - y_0)/(Iy-1); //delta_y
    real *x_k, *x_kp1; // Vetores para armazenar a iteração (k) e iteração (k+1)
    real *xkp1_minus_xk;
    int tam = Ix * Iy;
    real error = HUGE_VAL; // Variável para armazenar o erro.
    int k = 0; // contabiliza o número de iterações

    real* x_k = (real*) malloc(tam * sizeof(real));
    zeros(x_k, tam);
    real* x_kp1 = (real*) malloc(tam * sizeof(real));
    real* xkp1_minus_xk = (real*) malloc(tam * sizeof(real));

    /*----------------------------------Gauss-Seidel-------------------------------------*/
    while(k < iter && error > epsilon_max){
        for(int i = 0; i < Ix; i++){
            for(int j = 0; j < Iy; j++){

            }
        }
        sub_vectors(x_kp1, x_k, xkp1_minus_xk, tam);
        error = norm(xkp1_minus_xk, tam) / norm(x_kp1, tam);
        copy(x_kp1, x_k, tam);
        k++;
    }

    printf("# Valor de k: %d\n", k);
    printf("# Valor do erro: %f\n", error);

    /*Exporta a saída para gerar os gráficos posteriormente.*/
    export_output("../inout/data.csv", x_k, tam);
    
    return 0;
}