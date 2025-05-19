CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
SRC_DIR = ../sources
INC_DIR = ../include
EXTERN_DIR = $(INC_DIR)/extern
TARGET = tests_fonctions

# Récupérer tous les fichiers .c (sauf main.c)
SRC = $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c))

# Récupérer tous les fichiers .o
OBJ = $(SRC:$(SRC_DIR)/%.c=%.o) tests.o

# Inclure les fichiers d'entête
INCLUDE_DIRS = -I$(INC_DIR) -I$(EXTERN_DIR)

# Exécutable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Compilation des fichiers objets
%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# Règle par défaut
all: $(TARGET)
	doxygen Doxyfile

# Règle pour tests.o
tests.o: tests.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# Nettoyer les fichiers générés
clean:
	rm -f *.o $(TARGET)
	@echo "=== Fichiers nettoyés ==="

# Règles supplémentaires
.PHONY: all clean