#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <math.h>
#include <stdio.h>

/*Arquivo responsável pelo controle das variáveis do problema. Isso inclui constantes, variáveis globais, funções para definir/lidar com configurações e mais.*/

/*--------------------------------Constantes---------------------------------------------*/
#define real double 

/*--------------------------------Parâmetros---------------------------------------------*/
real h; //discretização.
real y_fix; //y_fixado. 
real muscle_thick, fat_thick, dermis_thick; //grossura das camadas
real xt1, yt1, zt1, rt1, xt2, yt2, zt2, rt2; //dados dos tumores 1 e 2.
real x0, xf, z0, zf; //intervalos 
real tol; //tolerancia do gauss-seidel 
int max_iter; //máximo de iterações do gauss-sedeil
real T_a; //temperatura arterial
real T_thresh1, T_thresh2; // Temperatura limiar do omega
int tamx, tamz;
real *x, *z;
/*------------------------Função para ler as configurações-------------------------------*/

/*
h
y_fix
muscle_thick fat_thick dermis_thcik
xt1 yt1 zt1 rt1 xt2 yt2 zt2 rt2
x0 xf z0 zf
T_a
T_thresh1 T_thresh2
tol
max_iter
*/
void fill_values(char* filename){
    FILE *fptr;
    
    fptr = fopen(filename, "r");
    
    if(fptr == NULL) {
        printf("Ocorreu um erro durante a abertura do arquivo.\n");
    }
    
    if (fscanf(fptr, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %d",
               &h, &y_fix, &muscle_thick, &fat_thick, &dermis_thick, &xt1, &yt1, &zt1, &rt1, &xt2, &yt2, &zt2, &rt2, &x0, &xf, &z0, &zf, &T_a, &T_thresh1, &T_thresh2, &tol, &max_iter) != 22) {
        printf("Erro ao ler os dados do arquivo.\n");
        fclose(fptr);
    }

    fclose(fptr);
}

int inside_tumor(real x, real z){
    real dist_t1 = (x - xt1) * (x - xt1) + (z - zt1) * (z - zt1) + (y_fix - yt1) * (y_fix - yt1);
    real dist_t2 = (x - xt2) * (x - xt2) + (z - zt2) * (z - zt2) + (y_fix - yt2) * (y_fix - yt2);

    return dist_t1 <= rt1 * rt1 || dist_t2 <= rt2 * rt2;
}

/*---------------------Funções Relacionados a Equação Diferencial----=-------------------*/

real Q_m(real x, real y){
    if(inside_tumor(x,y) == 1)
        return 4200;
    else
        return 420;
}

real c_b(real x, real y){
    return 4200;
}

real rho_b(real x, real y){
    return 1000;
}

real omega_b(real x, real y, real T) {
    if (inside_tumor(x, y) == 1) { // dentro do tumor
        if (T >= T_a || T <= T_thresh2)
            return 0.833 - (pow(fmax(T - 37.0, 0), 4.8) / pow(5.438, 3));
        else
            return 0.416;
    } else if (x <= 0.038) { // fora do tumor mas dentro do músculo
        if (T <= T_thresh1)
            return 0.36 + 0.36 * exp(-pow(T - 45.0, 2) / 12);
        else
            return 0.72;
    } else if (x <= 0.048) { // fora do tumor mas dentro da gordura
        if (T <= T_thresh1)
            return 0.36 + 0.36 * exp(-pow(T - 45.0, 2) / 12);
        else
            return 4.0;
    } else { // fora do tumor mas dentro da derme
        return 0.5;
    }
}

real k(real x, real y){
    if(inside_tumor(x,y) == 1)
        return 0.55;
    else if(x <= muscle_thick)
        return 0.45;
    else if(x <= muscle_thick + fat_thick)
        return 0.21;
    else
        return 0.4;
}

real k_harm(real k1, real k2){
    return (2 * k1 * k2) / (k1 + k2);
}

#endif