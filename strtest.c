#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
    char instrucao[100];
    char argumentos[5][30];
    int i = 0;
    char* token;
    printf("CrazyShell@user:~>");
    printf("chegou aqui 0");
    while(scanf("%s", instrucao));
    printf("chegou aqui 00");
    strcpy(argumentos[i++],token = strtok(instrucao, " "));
    printf("chegou aqui 1");
    while(token != NULL){
        strcpy(argumentos[i++],token = strtok(NULL, " "));
    }
    printf("chegou aqui 2");
    for(int j = 0; j < 5; j++){
        printf("%s\n", argumentos[j]);
    }
return 0;
}
