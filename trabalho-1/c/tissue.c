#include "utils.h"

/*Arquivo para gerar os valores do gráfico do tecido (2D).*/

int main(int argc, char* argv[]){
    read_config_txt("../inout/config.txt"); //read values from config.txt
    init_vars();

    export_output("../inout/tissue/tissue.bin", (void**) tissue, sizeof(int));

    end_vars();
    
    return 0;
}