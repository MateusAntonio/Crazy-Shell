#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include "auxiliar.c"



int main() {
    pid_t pai = getpid();
    
    signal(SIGINT, ctrl_c);
    
    while(1) {
        
        if(pai == getpid()){
            
            char instrucao[100] = {'\0'};  // linha toda que sera lida
            char executavel[30] = {'\0'}; // conterah apenas o nome do executavel
            char* argumentos[6]={NULL};  // contera o nome do programa e os argumentos
            int i = 0;
            int j = 0;
            char* token;
            
            prompt(); // imprime prompt
            
            scanf("%[^\n]", instrucao);
            getchar(); // para pegar o enter
            
            if(instrucao[0] != ' ' && instrucao[0] != '\n' && (strcmp(instrucao, "wait") != 0) && (strcmp(instrucao, "exit") != 0)) { // em caso de digitar comando , ex : ./telegram
                int whitespace = 0;
                for (i=0; i<strlen(instrucao); i++) { // conta espacos
                    if(instrucao[i] == ' ')
                        whitespace++;
                }
                i = 0;
                
                token = strtok(instrucao, " ");
                argumentos[i] = (char*)malloc((strlen(token)+1)*sizeof(char)); //i == 0 aqui.
                strcpy(executavel, token);
                strcpy(argumentos[i++], token); //usa i == 0 aqui e incrementa para que use i==1 depois.
                
                
                while(j < whitespace){ // ao final, j sera o n de argumentos + comando
                    argumentos[i] = (char*)malloc((strlen(token)+1)*sizeof(char));//alocando espaço para cada "String" no vetor argumentos
                    strcpy(argumentos[i],token = strtok(NULL, " "));// quebra a string em um vetor de strings e atribui isso ao indice i do vetor
                    j++;
                    i++;
                }
                
                if(j < 7) { // exec
                    exec(executavel, argumentos);
                }
                else { // mais parâmetros do que o permitido
                    printf("Apenas sao permitidos ate 5 argumentos!\n");
                }
                
                
                //<wait>
            } else if(strcmp(instrucao, "wait") == 0) {
                crazy_wait();
                //</wait>
                
                
                //<exit>
            } else if(strcmp(instrucao, "exit") == 0) {
                for (int x=0; x<j; x++) { // libera memoria
                    free(argumentos[x]);
                }
                crazy_exit();
                
            }
            //</exit>
        }
    }
    return 0;
}




