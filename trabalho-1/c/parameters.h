#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <math.h>
#include <stdio.h>

/*Arquivo responsável pelo controle das variáveis do problema. Isso inclui constantes, variáveis globais, funções para definir/lidar com os parâmetros da EDP, etc.*/

/*--------------------------------Constantes---------------------------------------------*/
#define real double 

/*--------------------------------Variáveis Globais---------------------------------------------*/
//Parâmetros do problema (INFORMADOS ATRAVÉS DE UM .txt)
real h; //discretização.
real y_fix; //y_fixado. 
real muscle_thick, fat_thick, dermis_thick; //grossura das camadas
real xt1, yt1, zt1, rt1, xt2, yt2, zt2, rt2; //dados dos tumores 1 e 2.
real x0, xf, z0, zf; //limites do domínio 
real tol; //tolerancia do gauss-seidel 
real T_a; //temperatura arterial
real T_thresh1, T_thresh2; // Temperatura limiar do omega
real t0 = 0, tf = 10; // Tempo inicial e final em segundos.

//Variáveis ou vetores implicadas a partir dos dados do problema
int tamx, tamz, tamt; //número de pontos em x e z
real *x, *z, *t; //vetores com os valores de x e z
int **tissue; //Matriz contendo o mapeamento => 1: tumor, 2: músculo, 3: gordura, 4: derme.
real **Q_r; //Matriz contendo os cálculos prontos da hipertermia.
real ht; // Discretização do tempo

// Parâmetros da Hipertermia.
real xi1, yi1, zi1, Ai1, ri1;
real xi2, yi2, zi2, Ai2, ri2;
real xi3, yi3, zi3, Ai3, ri3;

/*------------------------Função para ler as configurações do .txt-------------------------------*/

/*
h
y_fix
muscle_thick fat_thick dermis_thcik
xt1 yt1 zt1 rt1 xt2 yt2 zt2 rt2
x0 xf z0 zf
T_a
T_thresh1 T_thresh2
tol
*/
void read_config_txt(char* filename){
    FILE *fptr;
    
    fptr = fopen(filename, "r");
    
    if(fptr == NULL) {
        printf("Incorret filename! Error at line 51 on parameters.h\n");
    }
    
    if (fscanf(fptr, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
               &h, &y_fix, &muscle_thick, &fat_thick, &dermis_thick, &xt1, &yt1, &zt1, &rt1, &xt2, &yt2, &zt2, &rt2, &x0, &xf, &z0, &zf, &T_a, &T_thresh1, &T_thresh2, &tol) != 21) {
        printf("Error reading file's values. Line 56 on parameters.h\n");
        fclose(fptr);
    }

    fclose(fptr);
}

/*---------------------------------Inside Tumor-----------------------------------*/
real distance(real x1, real y1, real z1, real x2, real y2, real z2){
    real dx1 = x1 - x2;
    real dz1 = z1 - z2;
    real dy1 = y1 - y2; 
    return dx1 * dx1 + dz1 * dz1 + dy1 * dy1;
}

int is_inside_tumor(real x, real z){
    return distance(x, y_fix, z, xt1, yt1, zt1) <= rt1*rt1 || distance(x, y_fix, z, xt2, yt2, zt2) <= rt2*rt2;
}

/*---------------------------------Parâmetros do Problema-----------------------------------*/

real Q_m(int j, int i){
    if(tissue[j][i] == 1)
        return 4200.0;
    else
        return 420.0;
}

real c_b(int j, int i){
    return 4200.0;
}

real rho_b(int j, int i){
    return 1000.0;
}

real omega_b(int j, int i, real T){
    if(tissue[j][i] == 1){
        if(T < T_a)
            return 0.833;
        else if( T_a <= T && T <= T_thresh2)
            return 0.833 - ( pow(T-T_a, 4.8) / pow(5.438, 3) );
        else
            return 0.416;
    }else if(tissue[j][i] == 2){
        if(T <= T_thresh1)
            return 0.45 + 3.55 * exp(- (pow(T-T_thresh1, 2) / 12));
        else
            return 4.0;
    }else if(tissue[j][i] == 3){
        if(T <= T_thresh1)
            return 0.36 + 0.36 * exp(- (pow(T-T_thresh1, 2) / 12));
        else
            return 0.72;
    }else{
        return 0.5;
    }
}

real k(int j, int i){
    if(tissue[j][i] == 1)
        return 0.55;
    else if(tissue[j][i] == 2)
        return 0.45;
    else if(tissue[j][i] == 3)
        return 0.21;
    else
        return 0.4;
}

real k_harm(real k1, real k2){
    return (2 * k1 * k2) / (k1 + k2);
}

#endif