#ifndef PARSING_H
#define PARSING_H

#include "struct.h"

int parsing_pipes(char input[], Process process[], char* tokens[]);
int verifyChar(char *wantedChar, char string[]);
void check_process(Process *process);
void inicializa_process(Process* processo);
void parsing_commands(char tokens[], Process* process);
void parsing_in(char tokens[], Process* process);
void parsing_out(char tokens[], Process* process);
void parsing_space(char tokens[], Process* process);
void populate(int size, Process* process, char* tokens[]);

#endif