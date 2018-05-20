#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void crazy_exit(void);
void prompt(void);
void ctrl_c(int signo);
void exec(char* exe, char* argv[]);

pid_t pidfilho; // global pq é necessária no handler ctrl_c

int main(int argc, const char* argv[]){
    
    while(1) { // loop infinito
        
        char instrucao[100];  // linha toda que sera lida
        char executavel[100]; // contera apenas o nome do executavel
        char* argumentos[5];  // contera apenas os argumentos
        int i = 0;
        int j = 0;
        char* token;
        
        prompt();
        
        scanf("%[^\n]", instrucao);
        getchar(); // para pegar o enter
        
        if(instrucao[0] == '.' && instrucao[1] == '/') { // em caso de digitar comando , ex : ./telegram
            int ws = 0;
            for (i=0; i<strlen(instrucao); i++) { // conta espacos
                if(instrucao[i] == ' ')
                    ws++;
            }
            i = 0;
            token = strtok(instrucao, " ");
            strcpy(executavel, token);
         
            
            while(j < ws){ // ao final, j sera o n de argumentos + comando
                argumentos[i] = (char*)malloc((strlen(token)+1)*sizeof(char));
                strcpy(argumentos[i],token = strtok(NULL, " "));
                j++;
                i++;
            }
            
            //argumentos[i] = NULL;
            
            if(j < 6) { // exec
                //execv(executavel, argumentos);
                exec(executavel, argumentos);
                //printf("comando: %s\n", executavel);
                 //for(int x = 0; x <= j; x++){
                   //  printf("%s\n", argumentos[x]);
                 //}
            }
            else { // mais parâmetros do que o permitido
                printf("Apenas sao permitidos ate 5 argumentos!\n");
            }
        } else if(strcmp(instrucao, "wait") == 0) { // wait
            printf("comando wait detectado\n");
        } else if(strcmp(instrucao, "exit") == 0) { //exit
            printf("adeus\n");
            exit(0);
        }
        else // comando incorreto
            printf("csh: Comando \"%s\" nao encontrado\n", instrucao);
    }
    return 0;
}

void prompt(){
    printf("CrazyShell@user:~>");
}

void exec(char* exe, char* argv[]){
    
    int erro = 1;
    //printf("eii! %s %s\n", exe, argv[0]);
    //char *const ArgumentV[] = {"2015100338", "162534"}; // ver como isso funciona
    pid_t pid = fork(); //fork a child
    
    if(pid < 0){ //error forking
        fprintf(stderr, "Fork failed! \n");
        return ;
    } else if(pid == 0){ // child process
        
        pid_t pid2 = fork();
        if(pid2 < 0){ //error forking
            fprintf(stderr, "Fork failed! \n");
            return ;
        } else if(pid2 == 0){ // grandson process
            printf("print do neto\n");
            daemon(0, 0);
           // erro = execv(exe, argv);
            if(erro == (-1))
                printf("deu erro krlh");
            //printf("Executou neto");
        } else {
            pidfilho = pid;
            //sleep(2);
            printf("print do filho\n");
            //erro = execv(exe, argv);
            if(erro == (-1))
                printf("deu erro krlh");
            //printf("Executou filho/pai");
        }
        erro = execv(exe, argv);
        
    } else { //parent process
        //signal(SIGINT, ctrl_c);
        int status;
        waitpid(pidfilho, &status, WUNTRACED);
        printf("filho retornou! \n");
    }
    //return
}


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
}
