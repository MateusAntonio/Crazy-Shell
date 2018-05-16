#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
    int quantidadePalavrasRecebidas = 0;
    char instrucao[100];
    char* argumentos[5];


    quantidadePalavrasRecebidas = scanf("%[^\n]s", instrucao);
    printf("%d", quantidadePalavrasRecebidas);


    return 0;
}



    // setbuf(stdin, NULL);
    // fgets(instrucao, 100, stdin);
    // // getchar();
    // for
    // scanf("%[^\n]s", instrucao);
    // argumentos[j++] = token = strtok(instrucao, " ");
    

    // while(token != NULL){
    //     // printf("%s\n", token);
    //     token = strtok(NULL, " ");
    //     argumentos[j++] = token;
    // }
    // for (i=0; i < strlen(argumentos); i++)
    //     printf("%s ", argumentos[i]);
    // // printf("%d\n", i);
    // // printf("\n\n\n\n%d\n\n\n\n\n", argumentos[i]);
    
    return 0;


    // char vet[];
    // printf("CrazyShell@user:~>");
    // scanf("%s", vet);
    // token = strtok(vet, " ");
    // while(token != NULL){
    //     printf("%s ", token);
    //     token = strtok(vet, " ");
    // }
    // // scanf("%s", vet);
    // return 0;
}