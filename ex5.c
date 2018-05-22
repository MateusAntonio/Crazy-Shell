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
int testFork(pid_t pid);

int key=0;
pid_t pidfilho; // global pq é necessária no handler ctrl_c

int main(int argc, const char* argv[]){
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
        
        prompt();
        
        //---------------------------------------------------<entrada> ---------------------------------------------------
        
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
            //-----------------------------------------------</entrada>-------------------------------------------------------------


        //<wait>
        } else if(strcmp(instrucao, "wait") == 0) { // wait
            crazy_wait();
        //</wait>


        //<exit>
        } else if(strcmp(instrucao, "exit") == 0) { //exit        
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

void prompt(){
    printf("CrazyShell@user:~>");
}

void exec(char* exe, char* argv[]){
 
    //signal(SIGINT, ctrl_c);
    int erro = 1;
    pid_t pid = fork(); //fork a child
    
    if(testFork(pid)){ //error forking
        return;
    } else if(pid == 0){ // child process
        signal(SIGINT, SIG_IGN);
        
        pid_t pid2 = fork();
        if(testFork(pid2)){ //error forking
            return ;
        } else if(pid2 == 0){ // grandson process
            daemon(0, 0);
            // printf("espera-se que só o neto print aqui %d\n", getpid());
        } else {
            //nada por enquanto
        }
        erro = execv(exe, argv); //procura executavel na pasta local
        if(erro == -1){
            erro = execvp(exe, argv); //procura executavel no path base
            if(erro == -1){
                printf("csh: Comando \"%s\" nao encontrado\n", exe);
                exit(1);
            }
        }   
        //exec unico para dois
    } else { //parent process
        int status;
        waitpid(pidfilho, &status, WUNTRACED);
        printf("filho retornou! \n");
        key=0;
    }
    //return
}


void ctrl_c(int signo) {
    char c;
    sigset_t sigset, oldset; //criação das máscaras
    sigfillset(&sigset); //alimenta a mascara com todos os sinais de sistema
    sigprocmask(SIG_SETMASK, &sigset, &oldset); //substitui a máscara do sistema pela nova máscara e armazena a antiga em oldset
    signal(signo, SIG_IGN); // ignora um possivel control c novo
    prompt(); //exibe o prompt de comando
    printf("Não adianta me enviar um sinal por Ctrl-c, não vou morrer!\n Você quer"
           "suspender meu filho que está rodando em foreground? S/n:\n");
    c = getchar(); // captura resposta
    if (c == 'S' || c == 's')
        kill(pidfilho, 20);
    getchar(); // ignora o enter
    sigprocmask(SIG_SETMASK, &oldset, NULL);//reseta a mascara do sistema para a mascara guardada em oldset
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
           printf("[INFO] O processo %d foi recebeu o sinal %d e foi finalizado\n", pid, WTERMSIG(status));
        else if(WIFEXITED(status))
                printf("[INFO]) O processo finalizou sem recebimento de sinais\n");
    }
    if(filhos)
        printf("Filhos zumbis liberados\n");
    else
        printf("Sem filhos zumbi\n");
}

void clear(){
    if(!fork())
        execlp("clear","clear",NULL);
}

int testFork(pid_t pid){
    if(pid < 0){
        fprintf(stderr, "Fork failed! \n");
        return -1;
    }
    return 0;
}
