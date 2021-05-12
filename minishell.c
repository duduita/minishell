// Aluno: Eduardo Menezes Moraes
// CES33 - ITA
// Professor Lourenço

#include "initialize.c"
#include "parsing.c"
#include "recursive.c"

int readInput();

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
        recursive_execve(&process[size-1]); // inicia com o comando mais à direita
        free(process);
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