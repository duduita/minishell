#ifndef STRUCT_H
#define STRUCT_H

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
    char* input_file; // arquivo de entrada
    char* output_file; // arquivo de saída
    Process* pipein; // noh filho
    Process* pipeout ; // noh pai
};

#endif