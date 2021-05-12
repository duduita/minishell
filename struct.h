#ifndef STRUCT_H
#define STRUCT_H

// Libs utilizadas
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Configuração do minishell
#define MaxInputSize 1024
#define MaxComandSize 128
#define MaxArgumentSize 128
char* promptStr="minishell> ";

typedef struct process Process;

struct process
{
    char command[MaxComandSize]; // comando
    char* args[MaxArgumentSize]; // argumentos
    char* output_file; // arquivo de saída
    char* input_file; // arquivo de entrada
    Process* pipein; // noh filho
    Process* pipeout ; // noh pai
};

#endif