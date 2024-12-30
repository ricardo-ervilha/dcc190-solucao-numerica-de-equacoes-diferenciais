#include "utils.h"


int main(int argc, char* argv[]){
    fill_values("../inout/config.txt");
    init_vars();
    
    real** tissue = generate_tissue_matrix();
    
    export_output("../inout/tissue.bin", tissue);

    free_matrix(tissue);
    free(x);
    free(z);
}