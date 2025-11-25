# Makefile para o Projeto Final POO - Sistema de Gerenciamento de Estoque
# Autor: Projeto Final POO

# Compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -I$(INC_DIR)

# Flags de debug
DEBUG_FLAGS = -g -DDEBUG

# Flags de release
RELEASE_FLAGS = -O2

# Diretórios
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Nome do executável
TARGET = $(BIN_DIR)/estoque

# Arquivos fonte
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Arquivos objeto
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Regra padrão
all: directories $(TARGET)

# Cria os diretórios necessários
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Linking
$(TARGET): $(OBJECTS)
	@echo "Linking..."
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build concluído: $(TARGET)"

# Compilação
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compilando $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build de debug
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: clean all

# Build de release
release: CXXFLAGS += $(RELEASE_FLAGS)
release: clean all

# Executa o programa
run: all
	@echo "Executando..."
	@./$(TARGET)

# Limpeza
clean:
	@echo "Limpando..."
	@rm -rf $(OBJ_DIR) $(BIN_DIR) *.dat
	@echo "Limpeza concluída."

# Limpeza parcial (mantém executável)
clean-obj:
	@rm -rf $(OBJ_DIR)

# Reinstala
rebuild: clean all

# Informações
info:
	@echo "===== Informações do Projeto ====="
	@echo "Compilador: $(CXX)"
	@echo "Flags: $(CXXFLAGS)"
	@echo "Fontes: $(SOURCES)"
	@echo "Objetos: $(OBJECTS)"
	@echo "Executável: $(TARGET)"

# Ajuda
help:
	@echo "===== Makefile - Sistema de Estoque ====="
	@echo "Comandos disponíveis:"
	@echo "  make         - Compila o projeto"
	@echo "  make all     - Compila o projeto"
	@echo "  make debug   - Compila com símbolos de debug"
	@echo "  make release - Compila com otimizações"
	@echo "  make run     - Compila e executa"
	@echo "  make clean   - Remove arquivos compilados"
	@echo "  make rebuild - Limpa e recompila"
	@echo "  make info    - Mostra informações do projeto"
	@echo "  make help    - Mostra esta ajuda"

.PHONY: all directories debug release run clean clean-obj rebuild info help
