#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>


int main() {

    pid_t pid;
    int sinal;

    //obtendo os parametros
    printf("Qual eh o numero do processo destino?\n");
    scanf(" %d", &pid);

    printf("Qual eh o numero do sinal que você deseja mandar?\n");
    scanf(" %d", &sinal);

    //checando se o processo existe
    int check = kill(pid, 0);
    if(check == -1) {
        printf("Numero de processo invalido, tente novamente\n");
        return 0;
    }
    
    //mandando o sinal
    int resultado = kill(pid, sinal);
    if (resultado == 0) {
        printf("Sinal mandado com sucesso ao processo %d\n", pid);
        return 0;
    } else {
        printf("Nao foi possivel mandar o sinal ao processo %d\n", pid);
        return 0;
    }
}