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

//Informações do Qr
real xi1=0.02, yi1=0.045, zi1=0.055, Ai1=0.8e6, ri1=0.6e-3;
real xi2=0.025, yi2=0.045, zi2=0.045, Ai2=0.7e6, ri2=0.6e-3;
real xi3=0.015, yi3=0.045, zi3=0.04, Ai3=0.7e6, ri3=0.6e-3;

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

real distance(real x1, real z1, real x2, real y2, real z2){
    return (x1-x2)*(x1-x2) + (y_fix-y2)*(y_fix-y2) + (z1-z2)*(z1-z2);
}

// Ai * exp(-r^2 / r0^2)
real Q_r(real x, real z){
    real Q_1 = Ai1 * exp(- distance(x, z, xi1, yi1, zi1) / (ri1*ri1));
    real Q_2 = Ai2 * exp(- distance(x, z, xi2, yi2, zi2) / (ri2*ri2));
    real Q_3 = Ai3 * exp(- distance(x, z, xi3, yi3, zi3) / (ri3*ri3));
    return Q_1 + Q_2 + Q_3;
}

real Q_m(real x, real z){
    if(inside_tumor(x,z) == 1)
        return 4200;
    else
        return 420;
}

real c_b(real x, real z){
    return 4200;
}

real rho_b(real x, real z){
    return 1000;
}

real omega_b(real x, real z, real T) {
    if(inside_tumor(x,z) == 1){
        if(T < T_a)
            return 0.833;
        else if( T_a <= T && T <= T_thresh2)
            return 0.833 - ( pow(T-T_a, 4.8) / pow(5.438, 3) );
        else
            return 0.416;
    }else if(x <= muscle_thick){
        if(T <= T_thresh1)
            return 0.45 + 3.55 * exp(- (pow(T-T_thresh1, 2) / 12));
        else
            return 4.0;
    }else if(x <= muscle_thick + fat_thick){
        if(x <= T_thresh1)
            return 0.36 + 0.36 * exp(- (pow(T-T_thresh1, 2) / 12));
        else
            return 0.72;
    }else{
        return 0.5;
    }
}

real k(real x, real z){
    if(inside_tumor(x,z) == 1)
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