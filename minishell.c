#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "initialize.c"
#include "parsing.c"

void errorHandle(char* msg);
void recursive_execve(Process* process);
int readInput();

void errorHandle(char* msg) // função para retornar erro
{
	perror(msg);
	exit(1);
}

void recursive_execve(Process* process){
    int fd;
    if (process->pipein == NULL){ // caso trivial da recursão
        if (process->input_file != NULL){
            fd = open(process->input_file, O_RDONLY);
            if(fd < 0)
                errorHandle("Arquivo de entrada inválido!");
            dup2(fd, STDIN_FILENO); // altera a entrada padrão p/ input_file
        }
        if (process->pipeout == NULL){
            if (process->output_file != NULL){
                fd = open(process->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if(fd < 0)
                    errorHandle("Arquivo de entrada inválido!");
                dup2(fd, STDOUT_FILENO); // altera a saída padrão p/ output_file
            }
        }
        close(fd);
        execve(process->command, process->args, NULL);
    }
    else {
        int pipefd[2];
        if (process->pipeout == NULL){ // comando mais à direita
            if (process->output_file != NULL){
                fd = open(process->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                dup2(fd, STDOUT_FILENO); // altera a saída padrão p/ output_file
                close(fd);
            }
        }

        pipe(pipefd);
        pid_t pid = fork();
        if(pid == 0){
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            recursive_execve(process->pipein); // chama recursão novamente
        }
        else if(pid < 0)
            errorHandle("Falha ao criar o pipe!");        
        else {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            int status;
            wait(&status); // aguarda o filho
            execve(process->command, process->args, NULL); // executa o comando
        }
    } 
}

void printPrompt()
{
	write(STDOUT_FILENO, promptStr, strlen(promptStr))!=strlen(promptStr);
}

int readInput(){
    int size, length;
    char input[MaxInputSize];
    char* tokens[MaxComandSize];
    Process* process;
    while((length = read(STDIN_FILENO, input, MaxInputSize))>0){
        input[length-1]='\0';
        size = parsing_pipes(input, process, tokens); // calcula o tamanho da alocação e parseia o input
        process = (Process*)malloc(size * sizeof(Process)); // aloca a struct
        populate(size, process, tokens); // popula a struct com o input
        printf("\n");
        recursive_execve(&process[size-1]); // inicia com o comando mais à direita
    }
    return 0;
}

int main(int argc, char* argv[])
{
    while(1){ // laço infinito
        if (fork() == 0){
            printPrompt();
            readInput();
        }
        else wait(NULL);
    }
    return 0;   
}