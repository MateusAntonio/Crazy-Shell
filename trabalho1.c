#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

int pidfilho;

int testPID(pid_t pid);

void ctrl_c(int signo) {

    char c;
    sigset_t sigset, oldset;
    sigfillset(&sigset);
    sigprocmask(SIG_SETMASK, &sigset, &oldset);
    signal(signo, SIG_IGN); // ignora um possivel control c novo
    printf("Não adianta me enviar um sinal por Ctrl-c, não vou morrer! Você quer"
            "suspender meu filho que está rodando em foreground? S/n:");
     c = getchar();
     if (c == 'S' || c == 's')
          kill(pidfilho, 20);
     getchar(); 
     sigprocmask(SIG_SETMASK, &oldset, NULL);
     return;

}

void crazy_exit() {

    int aux;
    int status;
    while(aux = waitpid(-1, &status, 0)) {
        if(aux > 0)
            printf("Esperanado filho liberar");
        else
            break;
    }
    return 0;
}

int main(int argc, const char* argv[]){

    //char vet[100]; // palavra recebida
    char exec[100]; // comando para passar a funcao exec
    char argumentos[5]; // argumentos a serem passados
    char *token;
    char c;
    for(;;) { // crazy devera acontecer dentro de um loop

    prompt();
    // char vet[] = "./exec ert yui oiu nht jmh";
    // token = strtok(vet, " ");
    // while(token != NULL){
    //     printf("%s ", token);
    //     token = strtok(NULL, " ");
    // }
    // //scanf("%s", vet);
        // fazer as separacoes dos vetores, argumentos, comandos etc..

    pid_t pid = fork(); //fork a child

    if(pid < 0){ //error forking
        fprintf(stderr, "Fork failed! \n");
        return 1;
    } else if(pid == 0){ // child process

        pid_t pid2 = fork();
        if(pid2 < 0){ //error forking
            fprintf(stderr, "Fork failed! \n");
            return 1;
        } else if(pid2 == 0){ // grandson process
            printf("print do neto\n");
            execv("./hello", NULL);
        } else {
            pidfilho = pid;
            sleep(2);
            printf("print do filho/pai do neto\n");
            execv("./hello", NULL);
        }
    
    } else {
        signal(SIGINT, ctrl_c);
        int status;
        waitpid(pidfilho, &status, WUNTRACED);
        printf("filho retornou! \n");
    }
}
    //printf("[PARENT:] ")


return 0;
}

void prompt(){
    printf("CrazyShell@user:~ ");
}


// int testPID(pid_t pid){ //test error
//     if (pid < 0){ //error forking
//         fprintf(stderr, "Fork failed! \n");
//         return 1;
//     }
// }