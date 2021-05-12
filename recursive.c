#include "recursive.h"

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
            wait(&status);
            execve(process->command, process->args, NULL); // executa o comando
        }
    } 
}
