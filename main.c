#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#define N 4  // Número de produtores
#define MAX_ORDERS 12  // Número total de ordens (4 produtores x 3 ordens por produtor)

typedef struct {
    char nome_paciente[100];
    int id_medicamento;
    int quantidade;
} Pedido;

// Buffer circular de ordens
Pedido buffer[MAX_ORDERS];
int writepos = 0;  // Posição de escrita no buffer
int readpos = 0;   // Posição de leitura no buffer
int producers_done = 0; // Contador para saber quando todos os produtores terminaram
int total_orders = 0; // Total de ordens para garantir que todos os pedidos sejam consumidos

// Semáforos
sem_t empty;  // Contador de posições livres no buffer
sem_t full;   // Contador de posições ocupadas no buffer
sem_t lock;   // Semáforo de exclusão mútua para garantir atomicidade

typedef struct {
    char filename[100]; // Nome do arquivo
} ParametroProdutor;

// Função para ler os pedidos do arquivo
int ler_pedido(FILE *file, Pedido *pedido) {
    // Leitura das ordens no arquivo
    if (fscanf(file, "%s %d %d", pedido->nome_paciente, &pedido->id_medicamento, &pedido->quantidade) == 3) {
        return 1;  // Leitura bem-sucedida de um pedido
    }
    return 0;  // Não há mais pedidos no arquivo
}

void *produtor(void *arg) {
    ParametroProdutor *param = (ParametroProdutor *)arg;  // Recebe a estrutura com o nome do arquivo
    char *filename = param->filename;  // Obtém o nome do arquivo
    Pedido pedido;

    // Abre o arquivo uma única vez
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    // Processa as ordens do arquivo
    while (ler_pedido(file, &pedido)) {
        sem_wait(&empty);  // Espera por uma posição livre no buffer
        sem_wait(&lock);   // Garante que a escrita no buffer seja atômica

        buffer[writepos] = pedido;  // Coloca a ordem no buffer
        writepos = (writepos + 1) % MAX_ORDERS;

        sem_post(&lock);   // Libera o bloqueio da escrita
        sem_post(&full);   // Informa que há um pedido pronto para ser consumido
        total_orders++;    // Incrementa o total de ordens
    }

    // Enviar uma ordem de término
    Pedido fim = {"", -1, 0};  // Pedido com nome vazio, id_medicamento = -1, quantidade = 0
    sem_wait(&empty);
    sem_wait(&lock);
    buffer[writepos] = fim;  // Coloca a ordem de término no buffer
    writepos = (writepos + 1) % MAX_ORDERS;
    sem_post(&lock);
    sem_post(&full);

    producers_done++;  // Marca que este produtor terminou

    fclose(file);  // Fecha o arquivo apenas depois de processar todos os pedidos
    return NULL;
}


// Função do consumidor
void *consumidor(void *arg) {
    int pedidos_processados = 0;
    while (1) {
        sem_wait(&full);  // Espera por um pedido para consumir
        sem_wait(&lock);  // Garante que a leitura do buffer seja atômica

        Pedido pedido = buffer[readpos];
        readpos = (readpos + 1) % MAX_ORDERS;

        sem_post(&lock);
        sem_post(&empty);  // Libera o slot para os produtores

        // Verifica se é uma ordem de término
        if (pedido.id_medicamento == -1) {
            if (producers_done == N && pedidos_processados == total_orders) {
                break;  // Finaliza o consumidor se todos os produtores terminaram e todas as ordens foram processadas
            }
        } else {
            printf("Pedido: %s, Medicamento: %d, Quantidade: %d\n",
                   pedido.nome_paciente, pedido.id_medicamento, pedido.quantidade);
            pedidos_processados++;
        }
    }

    printf("Consumidor finalizado. Processou %d pedidos.\n", pedidos_processados);
    return NULL;
}

int main() {
    pthread_t produtores[N];
    pthread_t cons;

    // Lista de arquivos para os produtores
    ParametroProdutor parametros[N] = {
        {"pedido_0.txt"},
        {"pedido_1.txt"},
        {"pedido_2.txt"},
        {"pedido_3.txt"}
    };

    // Inicialização dos semáforos
    sem_init(&empty, 0, N);  // N slots livres
    sem_init(&full, 0, 0);    // Nenhum pedido processado inicialmente
    sem_init(&lock, 0, 1);    // Exclusão mútua para escrever no buffer

    // Criação da thread consumidora
    pthread_create(&cons, NULL, consumidor, NULL);

    // Criação das threads produtoras
    for (int i = 0; i < N; i++) {
        // Passando a estrutura com o nome do arquivo para a thread do produtor
        pthread_create(&produtores[i], NULL, produtor, (void *)&parametros[i]);
    }

    // Espera as threads produtoras terminarem
    for (int i = 0; i < N; i++) {
        pthread_join(produtores[i], NULL);
    }

    // Espera a thread consumidora terminar
    pthread_join(cons, NULL);

    // Destruição dos semáforos
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&lock);

    return 0;
}
