#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define BUFFER 20
#define READ 0
#define WRITE 1

int sorteiaDelta() {

    time_t tempo;
    int delta = 0;
    srand((unsigned) time(&tempo));

    delta = (rand() %100) + 1;

    return delta;
}

int geraNumero(int ni) {
    
    int delta = sorteiaDelta();
    int nf = ni + delta;

    return nf;
}

int analisaPrimo(int nRecebido) {
     
    int check = 0;
    
    for(int i = 2; i <= nRecebido / 2; i++) {
        if(nRecebido % i == 0) {
            check = 1;
            break;            
        }
    }

    if(nRecebido == 1) {
        return 1;
    } else if(check == 0) {
        return 1;
    } else {
        return 0;
    }
}


int main() {

    int ni = 0;
    int i = 0;
    int nGerados = 0;
    int final = 1;
    pid_t pid; 

    printf("Quantos números serão gerados?\n");
    scanf("%d", &nGerados);

    //abrindo o pipe
    int fdPipe[2];

    //criando o pipe
    if (pipe(fdPipe) == -1) {
        printf("Erro, pipe falhou.\n");
        return (0);
    }

    //fazendo o fork pra criar o filho
    pid = fork();
    if (pid < 0) {
        perror("Erro, fork falhou.\n");
        return (0);
    }

    //processo pai    
    if(pid > 0) {                               
        close(fdPipe[READ]);

        char stringEnviada[BUFFER];

        while (i < nGerados) {
            
            int nf = geraNumero(ni);
            ni = nf;

            sprintf(stringEnviada,"%d\n",nf);            
            write(fdPipe[WRITE], stringEnviada, BUFFER);    //escrevendo a string no pipe
            sleep(1);
            i++;
        }

        if(i == nGerados) {
            final = 0;

            sprintf(stringEnviada,"%d\n",final);            
            write(fdPipe[WRITE], stringEnviada, BUFFER);    //escrevendo a string no pipe
            exit(0);          
        }
           

    } else {                        //processo filho
        close(fdPipe[WRITE]);
        
        char stringRecebida[BUFFER];

        while(i <= nGerados) {
            
            read(fdPipe[READ], stringRecebida, BUFFER);     //lendo a string no pipe
            int zero = atoi(stringRecebida);

            if(zero == 0) {
                
                printf("Cheguei no final.\n");
                return 0;
            } else {
                
                int check = analisaPrimo(zero);
                if(check == 1) {
                    printf("O numero %d eh primo.\n", zero);
                } else {
                    printf("O numero %d nao eh primo.\n", zero);
                }
                sleep(1);
                i++;
            }
        } 
    }

    return(0);
}