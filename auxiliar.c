#include "auxiliar.h"

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
    main();
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
