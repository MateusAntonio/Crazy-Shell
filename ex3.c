//
//  ex3.c
//  linguagem c
//
//  Created by user on 4/5/18.
//  Copyright © 2018 user. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

    char instrucao[100];
    char argumentos[5][30];
    int i = 0;
    char* token;
    int ws;

    for(;;) { // loop infinito

        printf("CrazyShell@user:~>");

        scanf("%[^\n]", instrucao);
        getchar(); // para pegar o enter

        if(instrucao[0] == '.' && instrucao[1] == '/') { // em caso de digitar comando , ex : ./telegram
        	ws = 0;
                for (i=0; i<strlen(instrucao); i++) { // conta espacos
                    if(instrucao[i] == ' ')
                        ws++;
                }
               // printf("%d\n", ws);
                i = 0;
                strcpy(argumentos[i++],token = strtok(instrucao, " "));
                int j=0;

                while(j < ws){ // ao final, j sera o n de argumentos + comando
                    strcpy(argumentos[i++],token = strtok(NULL, " "));
                    j++;
                }

                if(j < 6) {
                    for(int x = 0; x <= j; x++){
                        printf("%s\n", argumentos[x]);
                    }
                }
                else {
                    printf("Apenas sao permitidos ate 5 argumentos!\n");
                }
        }
        else if(strcmp(instrucao, "wait") == 0) {

            printf("comando wait detectado\n");
        }
        else if(strcmp(instrucao, "exit") == 0) {

            printf("adeus\n");
            exit(0);

        }
        else
            printf("csh: Comando \"%s\" nao encontrado\n", instrucao);
    }
        return 0;
}










