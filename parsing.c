#include "parsing.h"

int verifyChar(char *wantedChar, char string[])
{
   char *ptr = string;
   while (*ptr)
   {
      if (strchr(wantedChar, *ptr))
        return 1;
      ptr++;
   }
   return 0;
}

void parsing_in(char tokens[], Process* process){ // parseia <
    char* fraction = strtok(tokens, "<");
    char* parsing_in[128];
    int i = 0;
    while (fraction != NULL) {
        parsing_in[i] = fraction;
        // printf("fractions <: %s\n", parsing_in[i]); // para facilitar o debugger
        fraction = strtok(NULL, " <");
        i++;
    }
    if (parsing_in[1] != NULL)
        process->input_file = parsing_in[1];
}

void parsing_out(char tokens[], Process* process){ // parseia >
    char* fraction = strtok(tokens, ">");
    char* parsing_out[128];
    int i = 0;
    while (fraction != NULL) {
        parsing_out[i] = fraction;
        fraction = strtok(NULL, " >");
        // printf("fractions >: %s\n", parsing_out[i]); // para facilitar o debugger
        i++;
    }
    if (parsing_out[1] != NULL)
        process->output_file = parsing_out[1];
}

void parsing_space(char tokens[], Process* process){ // parseia os espaços
    char* fraction = strtok(tokens, " ");
    char* parsing[128];
    int i = 0;
    while (fraction != NULL) {
        parsing[i] = fraction;
        // printf("fractions[%d]: %s\n", i, parsing[i]); // para facilitar o debugger
        fraction = strtok(NULL, " ");
        i++;
    }
    strcpy(process->command, parsing[0]);
    for (int j = 0; j < i; j++)
        process->args[j] = parsing[j];
}

void parsing_commands(char tokens[], Process* process){
    if(verifyChar("<", tokens)) // se tiver <
        parsing_in(tokens, process); // ele pega o segundo string (geralmente o in.txt)
    if(verifyChar(">", tokens)) // se tiver >
        parsing_out(tokens, process); // ele pega o out.txt
    parsing_space(tokens, process);
}

int parsing_pipes(char input[], Process* process, char* tokens[]){
    char* token = strtok(input, "|");
    int size = 0;
    while (token != NULL) { // primeiro guarda os entre-pipes (tokens)
        tokens[size] = token;
        // printf("commands: %s\n", tokens[i]); // para facilitar o debugger
        token = strtok(NULL, "|");
        size++;
    }
    return size;
}

void populate(int size, Process* process, char* tokens[]){
    for (int i = 0; i < size; i ++){
        inicializa_process(&process[i]);
    }

    for(int j = 0; j < size; j++){
        if (j != size-1){
            process[j+1].pipein = &process[j];
            process[j].pipeout = &process[j+1];
        }
        parsing_commands(tokens[j], &process[j]); // parseia os entre-pipes
    }
}

void check_process(Process *process){ // função para se checar a struct (apenas para debugar mais facilmente)
    for (int i = 0; i < 3; i++){
        printf("\n");
        printf("command[%d] = %s\n", i, process[i].command);
        for (int k = 0; k < 3; k++){
            printf("args[%d] = %s\n", k, process[i].args[k]);
        }
        printf("input_file[%d]: %s\n", i, process[i].input_file);
        printf("output_file[%d]: %s\n", i, process[i].output_file);
        printf("pipein_com[%d]: %s\n", i, process[i].pipein->command);
        printf("pipeout_com[%d]: %s\n", i, process[i].pipeout->command);
    }
}