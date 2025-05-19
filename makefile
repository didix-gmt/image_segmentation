CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
SRC_DIR = sources
INC_DIR = include
EXTERN_DIR = $(INC_DIR)/extern
TARGET = segmentation

DOXYGEN = doxygen
DOXYFILE = Doxyfile

# Récupérer tous les fichiers .c
SRC = $(wildcard $(SRC_DIR)/*.c)

# Récupérer tous les fichiers .o
OBJ = $(SRC:$(SRC_DIR)/%.c=$(SRC_DIR)/%.o)

# Inclure les fichiers d'entête
INCLUDE_DIRS = -I$(INC_DIR) -I$(EXTERN_DIR)

# Exécutable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Compilation des fichiers objets
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# Créer la documentation
doc:
	$(DOXYGEN) $(DOXYFILE)
	@echo "=== Documentation rédigée ==="

# Règle par défaut
all: $(TARGET) doc

# Nettoyer les fichiers générés
clean:
	rm -rf ./docs/documentation
	rm -rf *png *jpg
	rm -f $(SRC_DIR)/*.o $(TARGET)
	@echo "=== Fichiers nettoyés ==="

# Règles supplémentaires
.PHONY: doc all clean
