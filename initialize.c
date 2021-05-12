#include "initialize.h"

void inicializa_process(Process* process) {
    process->pipein = NULL;
    process->pipeout = NULL;
    process->input_file = NULL;
    process->output_file = NULL;
}

void allocate_processess(Process* process, int size){
    process = (Process*)malloc(size * sizeof(Process));
}