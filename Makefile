# Variáveis
CC = gcc
CFLAGS = -pthread -Wall
TARGET = prog  # Nome do binário final (do main.c)
INPUTS = inputs  # Nome do binário gerado de inputs.c

# Definição das regras

# Regra padrão: compilar, rodar inputs.c, limpar e compilar main.c
all: $(TARGET)

# Compilar e rodar inputs.c
generate_inputs:
	$(CC) -o $(INPUTS) inputs.c
	./$(INPUTS)  # Rodar o programa inputs para gerar os arquivos de entrada
	rm -f $(INPUTS)  # Remover o executável inputs após rodar

# Compilar o programa principal (main.c)
$(TARGET): generate_inputs main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c

# Limpar os arquivos gerados
clean:
	rm -f $(TARGET) pedido_0.txt pedido_1.txt pedido_2.txt pedido_3.txt

# Regra para compilar, gerar os arquivos de entrada e rodar o programa
full: all
