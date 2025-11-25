# POO-PROJETO-FINAL
## Sistema de Gerenciamento de Estoque

Projeto Final da disciplina de Programação Orientada a Objetos (POO).

---

## 📋 Descrição

Este sistema demonstra os principais conceitos de Programação Orientada a Objetos através de um sistema completo de gerenciamento de estoque. O projeto inclui:

- **Encapsulamento**: Atributos privados com getters e setters
- **Herança**: Classes derivadas de uma classe base
- **Polimorfismo**: Uso de interfaces e métodos virtuais
- **Abstração**: Interfaces definem contratos sem implementação

---

## 🏗️ Estrutura do Projeto

```
POO-PROJETO-FINAL/
├── include/                 # Arquivos de cabeçalho (.h)
│   ├── IProduto.h          # Interface para produtos
│   ├── IEstoque.h          # Interface para o estoque
│   ├── Produto.h           # Classe base Produto
│   ├── ProdutoPerecivel.h  # Produto com validade (herança)
│   ├── ProdutoNaoPerecivel.h # Produto sem validade (herança)
│   └── Estoque.h           # Gerenciador de estoque
├── src/                     # Arquivos fonte (.cpp)
│   ├── Produto.cpp
│   ├── ProdutoPerecivel.cpp
│   ├── ProdutoNaoPerecivel.cpp
│   ├── Estoque.cpp
│   └── main.cpp            # Menu interativo
├── Makefile                 # Script de compilação
└── README.md
```

---

## 🔧 Compilação e Execução

### Requisitos
- Compilador C++ com suporte a C++17 (g++ recomendado)
- Make

### Comandos

```bash
# Compilar o projeto
make

# Compilar e executar
make run

# Compilar com debug
make debug

# Compilar com otimizações
make release

# Limpar arquivos compilados
make clean

# Ver todos os comandos disponíveis
make help
```

---

## 📦 Classes e Interfaces

### Interface IProduto
Define o contrato base para todos os produtos:
- Getters e Setters para atributos
- Métodos de operação (adicionar/remover quantidade)
- Métodos de exibição e serialização

### Interface IEstoque
Define o contrato para operações de estoque:
- Operações CRUD (Criar, Ler, Atualizar, Deletar)
- Buscas por ID, nome e categoria
- Relatórios e estatísticas
- Persistência em arquivo

### Classe Produto (Base)
Implementação base de um produto genérico com:
- ID (auto-incremento)
- Nome, descrição, preço
- Quantidade em estoque
- Categoria

### Classe ProdutoPerecivel (Herança)
Estende Produto com:
- Data de fabricação
- Data de validade
- Temperatura de armazenamento
- Verificação de vencimento

### Classe ProdutoNaoPerecivel (Herança)
Estende Produto com:
- Material
- Peso
- Garantia
- Cálculo de frete

### Classe Estoque
Implementa IEstoque com:
- Lista de produtos (polimórfica)
- Persistência em arquivo
- Relatórios e estatísticas

---

## 🖥️ Funcionalidades do Sistema

1. **Cadastrar Produtos**
   - Produto Genérico
   - Produto Perecível (com validade)
   - Produto Não Perecível (com garantia)

2. **Listar Produtos**
   - Exibe todos os produtos com detalhes

3. **Buscar Produtos**
   - Por ID
   - Por Nome
   - Por Categoria

4. **Atualizar Produtos**
   - Modificar nome, descrição, preço, categoria

5. **Remover Produtos**
   - Remove produto por ID

6. **Movimentar Estoque**
   - Entrada (adicionar quantidade)
   - Saída (remover quantidade)

7. **Relatórios**
   - Relatório geral
   - Produtos por categoria
   - Estoque baixo
   - Produtos vencidos
   - Próximos a vencer

8. **Persistência**
   - Salvar dados em arquivo
   - Carregar dados de arquivo

---

## 🎯 Conceitos POO Demonstrados

| Conceito | Implementação |
|----------|---------------|
| **Encapsulamento** | Atributos private/protected com getters/setters |
| **Herança** | ProdutoPerecivel e ProdutoNaoPerecivel herdam de Produto |
| **Polimorfismo** | Interfaces IProduto e IEstoque com métodos virtuais |
| **Abstração** | Classes abstratas com métodos pure virtual |
| **Composição** | Estoque contém coleção de Produtos |

---

## 📝 Autor

Projeto desenvolvido como trabalho final da disciplina de Programação Orientada a Objetos.

---

## 📄 Licença

MIT License
