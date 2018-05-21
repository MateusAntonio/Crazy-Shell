#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void crazy_exit(void);
void crazy_wait(void);
void prompt(void);
void ctrl_c(int signo);
void exec(char* exe, char* argv[]);
void clear();

int key=0;
pid_t pidfilho; // global pq é necessária no handler ctrl_c

int main(int argc, const char* argv[]){
    
    pid_t son;
    pid_t pai = getpid();
    pid_t neto;
    int status;
    char executavel[30]; // conterah apenas o nome do executavel
    char* argumentos[5];  // conterah apenas os argumentos
    int i = 0;
    
    while(1) {
        
        if(getpid() == pai) {
        char instrucao[100];  // linha toda que sera lida
        
        int j = 0;
        char* token;
        
        //if(key == 0)
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
            
        } else if(strcmp(instrucao, "wait") == 0) { // wait
            crazy_wait();
        } else if(strcmp(instrucao, "exit") == 0) { //exit
            // liberar memoria
            for (int x=0; x<j; x++) {
                free(argumentos[x]);
            }
            crazy_exit();
        }
        else {
            son = fork();
            if(son < 0)
                printf("error forking");
    }
    }
        
        if(getpid() == pai) {
            status = waitpid(son, &status, WUNTRACED);
        }
        
        if(son == 0) {
            
            neto = fork();
            if(neto == 0)
                //daemon(0,0);
            
        execv(executavel, argumentos);
            
        }
  }
    return 0;
}

void prompt(){
    printf("CrazyShell@user:~>");
}

void exec(char* exe, char* argv[]){
    
    //signal(SIGINT, ctrl_c);
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
            //printf("print do neto\n");
            //    daemon(0, 0);
            erro = execv(exe, argv);
            if(erro == (-1))
                printf("deu erro krlh");
            printf("Executou neto");
            
        } else {
            pidfilho = pid;
            sleep(2);
            //printf("print do filho\n");
            erro = execv(exe, argv);
            if(erro == (-1))
                printf("deu erro krlh");
            printf("Executou filho/pai");
            sleep(2);
            
        }
        // erro = execv(exe, argv);
        // if(erro == (-1))
        //     printf("deu erro ai\n");
        
        
    } else { //parent process
        signal(SIGINT, ctrl_c);
        int status;
        waitpid(pidfilho, &status, WUNTRACED);
        printf("filho retornou! \n");
        key=0;
    }
    //return
}


void ctrl_c(int signo) {
    char c;
    sigset_t sigset, oldset;
    sigfillset(&sigset);
    sigprocmask(SIG_SETMASK, &sigset, &oldset);
    signal(signo, SIG_IGN); // ignora um possivel control c novo
    prompt();
    printf("Não adianta me enviar um sinal fpor Ctrl-c, não vou morrer! Você quer"
           "suspender meu filho que está rodando em foreground? S/n:\n");
    c = getchar();
    if (c == 'S' || c == 's')
        kill(pidfilho, 20);
    getchar();
    sigprocmask(SIG_SETMASK, &oldset, NULL);
    
}

void crazy_exit() {
    pid_t pid;
    int status;
    while(pid = waitpid(-1, &status, 0)) {
        if(pid > 0)
            printf("csh: Aguarde...\n");
        else
            break;
    }
    exit(0);
}

void crazy_wait() {
    
    pid_t pid;
    int status, filhos = 0;
    while((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        filhos++;
        if(WIFSIGNALED(status))
            printf("[INFO] O processo %d foi recebeu o sinal %d e foi finalizado", pid, WTERMSIG(status));
        else if(WIFEXITED(status))
            printf("[INFO]) O processo finalizou sem recebimento de sinais");
    }
    if(filhos)
        printf("Filhos zumbis liberados");
    else
        printf("Sem filhos zumbi");
}


//void clear(){
  //  if(!fork())
    //    execlp("clear","clear",NULL);
//}

