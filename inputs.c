#include <stdio.h>

int main() {
    FILE *file;

    // Criando o arquivo pedido_0.txt
    file = fopen("pedido_0.txt", "w");
    if (file != NULL) {
        fprintf(file, "João 101 2\n");
        fprintf(file, "Maria 202 1\n");
        fprintf(file, "Carlos 303 3\n");
        fclose(file);
    }

    // Criando o arquivo pedido_1.txt
    file = fopen("pedido_1.txt", "w");
    if (file != NULL) {
        fprintf(file, "Ana 101 1\n");
        fprintf(file, "Pedro 204 2\n");
        fprintf(file, "Luana 303 4\n");
        fclose(file);
    }

    // Criando o arquivo pedido_2.txt
    file = fopen("pedido_2.txt", "w");
    if (file != NULL) {
        fprintf(file, "Fernanda 405 1\n");
        fprintf(file, "Thiago 101 3\n");
        fprintf(file, "Juliana 202 2\n");
        fclose(file);
    }

    // Criando o arquivo pedido_3.txt
    file = fopen("pedido_3.txt", "w");
    if (file != NULL) {
        fprintf(file, "Ricardo 303 5\n");
        fprintf(file, "Marta 204 1\n");
        fprintf(file, "Lucas 101 2\n");
        fclose(file);
    }

    printf("Arquivos de pedido com 3 ordens cada foram criados com sucesso!\n");

    return 0;
}
