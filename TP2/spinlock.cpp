#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <vector>
#include <atomic>
#include <chrono>


using namespace std;

int N;                     //numero de valores (10⁷, 10⁸, 10⁹)
int K;                     //numero de threads (1, 2, 4, 8, 16, 32, 64, 128, 256)
int parcela;
vector<char> valores;             //vetor que vai guardar números entre -100 e 100
int soma = 0;

//implementando as funções acquire() e release()
class Spinlock {

    std::atomic_flag locked = ATOMIC_FLAG_INIT;

public:
    void acquire() {
        while(locked.test_and_set()){
            //busy wait
        }
    }
    void release(){
        locked.clear();
    }
};

Spinlock spinlock;

//função que é executada quando uma nova thread é criada
void *nova_thread(void* arg) {

    long tid = (long)arg;
    int soma_parcelas = 0;
    int y = parcela * tid;

    for(int i = 0; i < parcela; i++) {
        if((i + y) >= N) {
            break;
        }

        soma_parcelas += valores[i + y];
    }

    spinlock.acquire();
    soma += soma_parcelas;
    spinlock.release();

    pthread_exit(NULL);
}



int main (int argc, char** argv) {

    if (argc!= 3){
        cout << "Número incorreto de argumentos" << endl;
        return 0;
    } 

    //passando os argumentos da linha de comando pra as variaveis
    K = atoi(argv[1]);
    N = atoi(argv[2]);

    srand(time(NULL));

    //como o vetor "valores" tem que ter tamanho N:
    valores.resize(N * 1);

    //entrando com os N valores no vetor "valores"
    for(int i = 0; i < N; i++) {
        int n_aleatorio = rand() % 201 - 100;
        valores[i] = char(n_aleatorio);
    }

    //calculando a parcela N/K que cada thread vai ser responsável
    parcela = trunc(N/K);
    float resto = N % K;
    if(resto != 0) {
        parcela += 1;
    }

    //criando uma array com as threads
    pthread_t threads[K];

    //criando as threads e começando a contagem do tempo
    chrono::time_point<std::chrono::high_resolution_clock> inicio, final;

    inicio = chrono::high_resolution_clock::now();

    for(int i = 0; i < K; i++) {
        int thread_criada = pthread_create(&threads[i], NULL, &nova_thread, (void *)i);
        if (thread_criada != 0) {
            cout << "Falha ao criar a thread." << endl;
        }
    }

    //agora esperamos a finalização de todas as threads
    for(int i = 0; i < K; i++) {
        pthread_join(threads[i], NULL);
    }

    //finalizando a contagem de tempo
    final = chrono::high_resolution_clock::now();

    //calculando o tempo total do programa
    long tempo_total = std::chrono::duration_cast<std::chrono::milliseconds> (final - inicio).count(); 
    
    //imprimindo os resultados
    cout << "O total da soma foi: " << soma << endl;
    cout << "O tempo total do programa foi de " << tempo_total << "mS." << endl;

    return 0;
}
