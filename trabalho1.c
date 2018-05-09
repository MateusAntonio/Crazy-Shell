#include <stdio.h>
#include <unistd.h>

int testPID(pid_t pid);

int main(int argc, const char* argv[]){
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
            printf("print do filho/pai do neto\n");
            execv("./hello", NULL);
        }
    
    } else {
        wait();
        printf("filho retornou! \n");
    }
    //printf("[PARENT:] ")
return 0;
}


// int testPID(pid_t pid){ //test error
//     if (pid < 0){ //error forking
//         fprintf(stderr, "Fork failed! \n");
//         return 1;
//     }
// }