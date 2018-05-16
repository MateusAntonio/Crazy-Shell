#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
    char instrucao[100];
    int j = 0;
    int i = 0;
    int k = 0;
    char* argumentos[6];
    char* token;
    printf("CrazyShell@user:~>");
    for(i = 0; i < 6; i++){
        scanf("%s", argumentos[i]);    
    }


if(strcmp(argumentos[i], "\0")){
    n++;
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