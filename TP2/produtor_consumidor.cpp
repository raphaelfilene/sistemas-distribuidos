#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <mutex>
#include <unistd.h>
#include <condition_variable>
#include <thread>
#include <chrono>

using namespace std;

int qtdProdutos = 10000;
int tamanhoMemoriaCompartilhada = 32;
vector<int> memoriaCompartilhada(tamanhoMemoriaCompartilhada);
vector<int> consumirLocalmemoria(qtdProdutos);

int qtdProduzida = 0;
int qtdProdutosConsumed = 0;

//Some variable needed by the semaforo
mutex semaforoMutex;
condition_variable semaforoCheio;
condition_variable semaforoVazio;

bool temporizador = false;

int primeiraPosicaoLivre(vector<int> memoria)
{
    for (int i = 0; i<memoria.size(); ++i)
    {
        if (memoria[i] == 0) {
            return i;
        }
    }
    return -1;
}

int primeiraPosicaoCheia(vector<int> memoria)
{
    for (int i=0; i<memoria.size(); ++i)
    {
        if (memoria[i]!= 0) {
            return i;
        }
    }
    return -1;
}

bool totalmenteLivre(vector<int> memoria)
{
    if (primeiraPosicaoCheia(memoria) == -1) {
        return true;
    }
    return false;
}

bool totalmenteCheio(vector<int> memoria)
{

    if (primeiraPosicaoLivre(memoria) == -1) {
        return true;
    }
    return false;
}

int ehPrimo(int numero) // verifica se o numero passado no parâmetro eh primo
{
    int i;
    for (i=2; i < numero; i++) {
        if (numero % i == 0) {
            return 0;
        }
    }
    return 1;
}

int gerarRandomico(int minimo, int maximo) // retorna um numero randomico entre o minimo e o maximo
{
    if (!temporizador) {
        srand(time(NULL));
        temporizador = true;
    }
    return minimo+rand()%((maximo+1)-minimo);
}

void produzir()
{
    while(qtdProduzida <= qtdProdutos) {
        int produzirProduto = gerarRandomico(1, 10000000);
        unique_lock<mutex> lock(semaforoMutex);
        if(semaforoVazio.wait_for(lock, chrono::milliseconds(200), [] {return !totalmenteCheio(memoriaCompartilhada);})){
            if (qtdProduzida <= qtdProdutos) { // Verificando novamente porque a condição não pode ser verdadeira
                int memoriaCompartilhadaFreePosition = primeiraPosicaoLivre(memoriaCompartilhada);
                memoriaCompartilhada[memoriaCompartilhadaFreePosition] = produzirProduto;
                qtdProduzida += 1;
                semaforoCheio.notify_all();
            }
        }
    }
}

void consumir()
{
    while (qtdProdutosConsumed <= qtdProdutos) {
        int consumirProduto;
        unique_lock<mutex> lock(semaforoMutex);
        if(semaforoCheio.wait_for(lock, chrono::milliseconds(200), [] {return !totalmenteLivre(memoriaCompartilhada);})){
            if (qtdProdutosConsumed <= qtdProdutos) { // Verificando novamente porque a condição não pode ser verdadeira
                int memoriaCompartilhadaFullPosition = primeiraPosicaoCheia(memoriaCompartilhada);
                int consumirLocalmemoriaFreePosition = primeiraPosicaoLivre(consumirLocalmemoria);
                consumirProduto = memoriaCompartilhada[memoriaCompartilhadaFullPosition];
                consumirLocalmemoria[consumirLocalmemoriaFreePosition] = consumirProduto;
                memoriaCompartilhada[memoriaCompartilhadaFullPosition] = 0;
                qtdProdutosConsumed += 1;
                semaforoVazio.notify_all();
            }
        }
        int ehPrimonumero = ehPrimo(consumirProduto);
    }
}

int main(int argc, char* argv[])
{
    int Nc, Np;
    Np = atoi(argv[1]);
    Nc = atoi(argv[2]);

    int numeroThreadsProd = Np;
    int numeroThreadsCons = Nc;
    int numeroTotalDeThreads = numeroThreadsProd + numeroThreadsCons;

    thread conjuntoThreads[numeroTotalDeThreads];

    //Setando todos os elementos de "MemoriaCompartilhada" para zero
    for (int i = 0; i < memoriaCompartilhada.size(); ++i)
    { memoriaCompartilhada[i] = 0; }

    //Registra as threads de "produzir"
    for (int i = 0; i < numeroThreadsProd; ++i)
    { conjuntoThreads[i] = thread(produzir); }

    //Registra as threads de "consumir"
    for (int i = numeroThreadsProd; i < numeroTotalDeThreads; ++i)
    { conjuntoThreads[i] = thread(consumir); }

    //Inicia a contagem do tempo total
    chrono::time_point<std::chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    
    for (int i=0; i<numeroTotalDeThreads; ++i)
    { conjuntoThreads[i].join(); } //Join todas as threads com a thread principal

    //Termina a contagem do tempo total
    end = chrono::system_clock::now();
    long tempoTotal = std::chrono::duration_cast<std::chrono::milliseconds> (end-start).count();
    cout << tempoTotal << endl;

    return 0;
}