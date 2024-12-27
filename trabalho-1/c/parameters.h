#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <math.h>
#include <stdio.h>

/*Arquivo responsável pelo controle das variáveis do problema. Isso inclui constantes, variáveis globais, funções para definir/lidar com configurações e mais.*/

/*--------------------------------CONSTANTES---------------------------------------------*/
#define real float  // facilitando trocar tipo de ponto flutuante depois

/*--------------------------------Parâmetros---------------------------------------------*/
int Ix, Iy; // Número de pontos na discretização de x e de y
real x_tumor, y_tumor, radius; // Coordenadas do centro do tumor & raio do tumor
real x_0, x_f, y_0, y_f; // x_0 <= x <= x_f | y_0 <= y <= y_f
real epsilon_max;  // Tolerância do erro
int iter; // Número de iterações
real T_a; // Temperatura arterial
real u_a, u_b, u_c, u_d; // Condições de Contorno
/*
                                u_b (Neumann)
                    ------------------------------------
                    |                                  |
                    |                                  |
                    |                                  |
                    |                                  |
    u_a (Dirichlet) |                                  | u_c (Neumann)
                    |                                  |
                    |                                  |
                    |                                  |
                    |                                  |
                    ------------------------------------
                                u_d (Neumann)
*/


/*------------------------Função para ler as configurações-------------------------------*/

/*
Ix Iy
x_tumor y_tumor radius
x_0 x_f y_0 y_f
epsilon_max
iter
T_a
u_a u_b u_c u_d
*/
void fill_values(char* filename){
    FILE *fptr;
    
    fptr = fopen(filename, "r");
    
    if(fptr == NULL) {
        printf("Ocorreu um erro durante a abertura do arquivo.\n");
    }
    
    if (fscanf(fptr, "%d %d %f %f %f %f %f %f %f %f %d %f %f %f %f %f",
               &Ix, &Iy, &x_tumor, &y_tumor, &radius, &x_0, &x_f, &y_0, &y_f, &epsilon_max, &iter, &T_a, &u_a, &u_b, &u_c, &u_d) != 16) {
        printf("Erro ao ler os dados do arquivo.\n");
        fclose(fptr);
    }

    Ix += 1; //incluindo x = x_f, por causa do np.arange(0, 1+h, h);
    Iy += 1; //incluindo y = y_f, por causa do np.arange(0, 1+h, h);

    fclose(fptr);
}

/*---------------------Funções Relacionados a Equação Diferencial----=-------------------*/

/*Função auxiliar para descobrir se um dado ponto está dentro do tumor.*/
int inside_circle(real x, real y){
    return sqrt( pow(x-x_tumor, 2) + pow(y-y_tumor, 2) ) <= radius*radius;
}

/*Geração de calor metabólica*/
real Q_m(real x, real y){
    if(inside_circle(x, y) == 1) //dentro do tumor
        return 4200.0; 
    else if(x < 0.038) //fora do tumor mas dentro do músculo
        return 420.0;
    else if(x < 0.048) //fora do tumor mas dentro da gordura
        return 420.0;
    else //fora do tumor mas dentro da derme
        return 420.0;
}

/*Calor específico do sangue*/
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

/*Densidade sanguínea*/
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

/*Taxa de perfusão sanguínea*/
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

/*Condutividade térmica do sangue.*/
real k(real x, real y){ //REVIEW: Considerar Média Harmônica
    if(inside_circle(x, y) == 1) //dentro do tumor
        return 0.757981;
    else if(x < 0.038) //fora do tumor mas dentro do músculo
        return 1.87895;
    else if(x < 0.048) //fora do tumor mas dentro da gordura
        return 0.504908;
    else //fora do tumor mas dentro da derme
        return 0.5;
}

#endif