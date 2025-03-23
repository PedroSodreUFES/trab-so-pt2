#include <stdio.h>
#include <stdlib.h>

#define NUM_PEDIDOS_ARQUIVO 6
#define NUM_ARQUIVOS 10

int main() {
    FILE *file;

    for (int i = 0; i < NUM_ARQUIVOS; i++) {
        char filename[50];
        sprintf(filename, "pedido_%d.txt", i);
        file = fopen(filename, "w");

        if (file == NULL) {
            perror("Erro ao criar arquivo");
            exit(1);
        }

        for (int j = 0; j < NUM_PEDIDOS_ARQUIVO; j++) {
            int idMedicamento = 100 + rand() % 350; // entre 100 e 449
            int quantidade = 1 + rand() % 10;        // entre 1 e 10

            fprintf(file, "Paciente_%d %d %d\n",
                    j, idMedicamento, quantidade);
        }
        fclose(file);
        printf("Arquivo %s criado com %d pedidos.\n", filename, NUM_PEDIDOS_ARQUIVO);
    }

    printf("Todos os arquivos de pedido foram criados com sucesso!\n");
    return 0;
}