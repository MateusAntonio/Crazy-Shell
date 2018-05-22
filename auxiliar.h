#ifndef AUXILIAR_H
#define AUXILIAR_H

int key=0;
pid_t pidfilho; // global pq é necessária no handler ctrl_c

void crazy_exit(void);
void crazy_wait(void);
void prompt(void);
void ctrl_c(int signo);
void exec(char* exe, char* argv[]);
void clear();
int testFork(pid_t pid);

#endif
