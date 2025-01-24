#include "utils.h"
#include <time.h>


real** solver(){
    
}

int main(int argc, char* argv[]){
    read_config_txt("../inout/config.txt");
    init_vars();

    clock_t start, end;
    real cpu_time_used;

    start = clock();
    real **u = solver();
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("=>Tempo gasto: %f\n", cpu_time_used);

    export_output("../inout/parabolic/parabolic.bin", (void**) u, sizeof(real));
    end_vars();
}