#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sig_handler_sigint (int sinal){
    if(sinal == SIGINT) {
        printf("\nRecebi um SIGINT\n");
        exit(0);
    }
}

void sig_handler_sigquit (int sinal){
    if(sinal == SIGQUIT) {
        printf("\nRecebi um SIGQUIT\n");
        exit(0);
    }
}
    
void sig_handler_sigtstp (int sinal){
    if(sinal == SIGTSTP) {
        printf("\nRecebi um SIGTSTP\n");
        exit(0);
    }
}



int main() {

    //escolhendo o modo de espera
    int espera = 0;
    printf("\nQual tipo de espera voce gostaria que o programa rodasse?\n1 - Busy Mode\n2 - Blocking Mode\n");
    scanf("%d", &espera);
    
    if(espera != 2 && espera != 1) {
        printf("Entre com um numero valido.\n");
        return 0;
    }


    //estabelecendo os signal handlers
    signal(SIGINT, sig_handler_sigint);
    signal(SIGQUIT, sig_handler_sigquit);
    signal(SIGTSTP, sig_handler_sigtstp);
    
    //em caso de erro
    if (signal(SIGINT, sig_handler_sigint) == SIG_ERR){
        printf("Nao consigo tratar SIGINT");
        return 0;
    } else if (signal(SIGQUIT, sig_handler_sigquit) == SIG_ERR){
        printf("Nao consigo tratar SIGQUIT");
        return 0;
    } else if (signal(SIGTSTP, sig_handler_sigtstp) == SIG_ERR){
        printf("Nao consigo tratar SIGTSTP");
        return 0;
    }

    //modo de espera
    if(espera == 1) {                        //busy mode: programa continua rodando
        while(1) {
            sleep(1);
        }
    } else if(espera == 2) {                 //blocking mode: programa pausa até receber o sinal
        for ( ;; ) {
            pause();
        }
    }
    return 0;
}