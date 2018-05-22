#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// Variavel global, para passar PID do filho para a fun��o trata_c
pid_t filho = 1;

// Funcao para printar logo da crazyshell
void prompt() {
    printf("crazyshell@user:~> ");
    return;
}

// Fun��o para tratamento de ctrl-c [DESENVOLMIMENTO]
void trata_c(){
    char resposta[5];
    sigset_t newmask, oldmask;
    sigfillset(&newmask);
    sigprocmask(SIG_SETMASK, &newmask, &oldmask);

    printf("\n");
    prompt();
    printf("N�o adianta me enviar um sinal por Ctrl-c, n�o vou morrer!\n");
    printf("Voc� quer suspender meu filho que est� rodando em foreground? S/n:");

    scanf(" %s",resposta);

    resposta[1]='\0';

    if((strcmp(resposta, "s") == 0)||(strcmp(resposta, "S") == 0)){
      if(filho != 1){
    	   kill(filho,20);
       }
    }
    filho = 1;
    //prompt();
    setbuf(stdin, NULL);
    sigprocmask(SIG_SETMASK, &oldmask, NULL);
    return;
}

void trata_wait(){
  int personalizar = 0, aux;
  pid_t pid;
  while((pid = waitpid(-1,&aux,WNOHANG)) > 0){
      personalizar++;
      printf("O processo de PID: %d", pid);
      if(WIFEXITED(aux)){
        printf(" terminou normalmente\n");
      }else{
        if(WIFSIGNALED(aux)){
          printf(" terminou devido ao sinal %d\n", WTERMSIG(aux));
        }
      }
      //printf("Futuro LOG aqui!\n");
  }
  if(personalizar == 0)
    printf("N�o ha filhos zumbis\n");
  else
    printf("N�o ha mais filhos zumbis\n");
}

void trata_exit(){
    int personalizar = 0, aux;
    pid_t pid;
    while (waitpid(-1,&aux,0) > 0) {
      printf("Soltando filho...\n");
    }
    exit(0);
}

// Fun��o para limpar a tela, antes de iniciar o programa
void limpa_Tela(){
  if(fork() == 0){
    execlp("clear","clear",NULL);
  }
  return;
}

// Fun��o para contar tamanho de uma string
int tamanhoPalavra(char* aux){
    int i = 0;
    int tam = 1;
    while(aux[i]!='\0'){
      tam++;i++;
    }
    return tam;
}


//Main logica da shell por completo aqui [DESENVOLMIMENTO]
int main(int argc, char** argv) {

    char entrada[500], comando[50], *token, waitt[] = "wait", exitt[] = "exit";
    char* argvv[6]={NULL}; // definindo vetor para passar a fun��o exec
    int aux,i, retorno_EXEC;
    pid_t pai, neto = -1; //iniciar neto com um PID falso, para ele n�o entrar em lugares indevidos

    signal(SIGINT,trata_c);

    pai = getpid(); // recebe PID do pai
    limpa_Tela();
    aux = wait(&aux); // espera voltar da fun��o limpa_Tela

    while (1) { //cria um loop infinito, para funcionamento constante da crazyshell
        if (getpid() == pai) { //entrada s� do pai
            prompt();
            setbuf(stdin, NULL);
            fgets(entrada, 500, stdin); //le entrada

            // Tratamento da entrada **********************************************
            if ((entrada[0] != '\n')&&(entrada[0] != ' ')) {
                argvv[0] = comando;
                token = strtok(entrada, " \n");

                strcpy(comando, token);
                strcpy(argvv[0], token);
                i = 1;
                while (token != NULL) {
                    token = strtok(NULL, " \n");
                    if (token != NULL) {
                        int tamanho = tamanhoPalavra(token);
                        argvv[i] = (char*)malloc((tamanho+1)*sizeof(char));
                        strcpy(argvv[i], token);
                        i++;
                    }
                }

                for (; i < 6; i++) {
                    argvv[i] = NULL;
                }
            // FIM do tratamento da entrada ***************************************

            // Tratamento do WAIT *************************************************
                if (strcmp(comando, waitt) == 0) {
                    trata_wait();
                }else{
            // Tratamento do EXIT *************************************************
                  if (strcmp(comando, exitt) == 0) {
                    trata_exit();
                  }else{
                      filho = fork();
                      if(filho < 0){
                        printf("ERRO: 'fork'\n");
                      }
                  }
                }


            }
        }

        // Pai espera pelo seu filho em foreground
        if(getpid() == pai){
          aux = waitpid(filho,&aux,WUNTRACED);
        }

        //Filho come�a sua jornada
        if (filho == 0) {
          neto = fork(); // Filho cria neto

          if(neto == 0){
            //setsid();
            daemon(0,0); // Neto entra em modo background
          }

          signal(SIGINT,SIG_IGN); // Ignoramos o sinal do ctrl-c

          retorno_EXEC = 1; // criamos um ponto de verifica��o de retorno do exec
          retorno_EXEC = execvp(comando, argvv); // Chamamos o execvp que procura no PATCH padr�o, mas tbm consideramos poder executar algo do diretorio atual.
          if ( retorno_EXEC < 0){
              retorno_EXEC = 1; // setamos o ponto de verifica��o de retorno do exec
              retorno_EXEC = execv(comando, argvv); // agora tentamos executar pelo diretorio passado no argumento
              if( retorno_EXEC < 0){ // se o ponto de verifica��o ainda for menor que 0, logo o arquivo n�o n�o existe.
                  printf("%s: comando n�o encontrado\n",comando);
                  exit(1); // mata o filho e o neto consequentemente
              }
          }
        }

        if(pai == getpid()){ // O pai tem que liberar a memoria alocada.
            for(aux=1;aux<6;aux++){
              if((entrada[0]!='\n')&&(entrada[0]!=' ')){
              	free(argvv[aux]);
              }
            }
        }
    }

    return (EXIT_SUCCESS);
}