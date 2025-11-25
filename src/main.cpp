/**
 * @file main.cpp
 * @brief Sistema de Gerenciamento de Estoque - Projeto Final POO
 * @author Projeto Final POO
 * 
 * Este programa demonstra os conceitos de Programação Orientada a Objetos:
 * - Encapsulamento: atributos privados com getters/setters
 * - Herança: ProdutoPerecivel e ProdutoNaoPerecivel herdam de Produto
 * - Polimorfismo: uso de interfaces (IProduto, IEstoque) e métodos virtuais
 * - Abstração: interfaces definem contratos sem implementação
 */

#include <iostream>
#include <memory>
#include <limits>
#include <cstdlib>
#include <fstream>
#include "Produto.h"
#include "ProdutoPerecivel.h"
#include "ProdutoNaoPerecivel.h"
#include "Estoque.h"

// Cores para terminal (ANSI)
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

// Nome do arquivo de dados
const std::string ARQUIVO_DADOS = "estoque.dat";

/**
 * @brief Limpa o buffer de entrada
 */
void limparBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 * @brief Limpa a tela do terminal usando códigos ANSI
 */
void limparTela() {
    // Usa códigos ANSI de escape para limpar a tela
    // \033[2J - Limpa toda a tela
    // \033[H - Move cursor para posição inicial (1,1)
    std::cout << "\033[2J\033[H" << std::flush;
}

/**
 * @brief Aguarda o usuário pressionar Enter
 */
void pausar() {
    std::cout << "\nPressione ENTER para continuar...";
    limparBuffer();
}

/**
 * @brief Exibe o cabeçalho do sistema
 */
void exibirCabecalho() {
    std::cout << BOLD << CYAN;
    std::cout << "╔══════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║      SISTEMA DE GERENCIAMENTO DE ESTOQUE - POO           ║" << std::endl;
    std::cout << "║                  PROJETO FINAL                           ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════╝" << RESET << std::endl;
    std::cout << std::endl;
}

/**
 * @brief Exibe o menu principal
 */
void exibirMenuPrincipal() {
    std::cout << BOLD << YELLOW << "\n═══════════════ MENU PRINCIPAL ═══════════════" << RESET << std::endl;
    std::cout << GREEN << " 1." << RESET << " Cadastrar Produto" << std::endl;
    std::cout << GREEN << " 2." << RESET << " Listar Todos os Produtos" << std::endl;
    std::cout << GREEN << " 3." << RESET << " Buscar Produto" << std::endl;
    std::cout << GREEN << " 4." << RESET << " Atualizar Produto" << std::endl;
    std::cout << GREEN << " 5." << RESET << " Remover Produto" << std::endl;
    std::cout << GREEN << " 6." << RESET << " Movimentar Estoque" << std::endl;
    std::cout << GREEN << " 7." << RESET << " Relatórios" << std::endl;
    std::cout << GREEN << " 8." << RESET << " Salvar Dados" << std::endl;
    std::cout << GREEN << " 9." << RESET << " Carregar Dados" << std::endl;
    std::cout << RED << " 0." << RESET << " Sair" << std::endl;
    std::cout << YELLOW << "═══════════════════════════════════════════════" << RESET << std::endl;
    std::cout << "Escolha uma opção: ";
}

/**
 * @brief Exibe o submenu de cadastro
 */
void exibirMenuCadastro() {
    std::cout << BOLD << CYAN << "\n═══════════ CADASTRAR PRODUTO ═══════════" << RESET << std::endl;
    std::cout << GREEN << " 1." << RESET << " Produto Genérico" << std::endl;
    std::cout << GREEN << " 2." << RESET << " Produto Perecível" << std::endl;
    std::cout << GREEN << " 3." << RESET << " Produto Não Perecível" << std::endl;
    std::cout << RED << " 0." << RESET << " Voltar" << std::endl;
    std::cout << CYAN << "═══════════════════════════════════════════" << RESET << std::endl;
    std::cout << "Escolha uma opção: ";
}

/**
 * @brief Exibe o submenu de busca
 */
void exibirMenuBusca() {
    std::cout << BOLD << CYAN << "\n═══════════ BUSCAR PRODUTO ═══════════" << RESET << std::endl;
    std::cout << GREEN << " 1." << RESET << " Buscar por ID" << std::endl;
    std::cout << GREEN << " 2." << RESET << " Buscar por Nome" << std::endl;
    std::cout << GREEN << " 3." << RESET << " Buscar por Categoria" << std::endl;
    std::cout << RED << " 0." << RESET << " Voltar" << std::endl;
    std::cout << CYAN << "═════════════════════════════════════════" << RESET << std::endl;
    std::cout << "Escolha uma opção: ";
}

/**
 * @brief Exibe o submenu de relatórios
 */
void exibirMenuRelatorios() {
    std::cout << BOLD << CYAN << "\n═══════════ RELATÓRIOS ═══════════" << RESET << std::endl;
    std::cout << GREEN << " 1." << RESET << " Relatório Geral" << std::endl;
    std::cout << GREEN << " 2." << RESET << " Produtos por Categoria" << std::endl;
    std::cout << GREEN << " 3." << RESET << " Estoque Baixo" << std::endl;
    std::cout << GREEN << " 4." << RESET << " Produtos Vencidos" << std::endl;
    std::cout << GREEN << " 5." << RESET << " Próximos a Vencer" << std::endl;
    std::cout << RED << " 0." << RESET << " Voltar" << std::endl;
    std::cout << CYAN << "═══════════════════════════════════════" << RESET << std::endl;
    std::cout << "Escolha uma opção: ";
}

/**
 * @brief Cadastra um produto genérico
 * @param estoque Referência para o estoque
 */
void cadastrarProdutoGenerico(Estoque& estoque) {
    std::string nome, descricao, categoria;
    double preco;
    int quantidade;
    
    limparBuffer();
    
    std::cout << "\n--- Cadastro de Produto Genérico ---" << std::endl;
    
    std::cout << "Nome: ";
    std::getline(std::cin, nome);
    
    std::cout << "Descrição: ";
    std::getline(std::cin, descricao);
    
    std::cout << "Preço: R$ ";
    std::cin >> preco;
    
    std::cout << "Quantidade: ";
    std::cin >> quantidade;
    
    limparBuffer();
    std::cout << "Categoria: ";
    std::getline(std::cin, categoria);
    
    auto produto = std::make_shared<Produto>(nome, descricao, preco, quantidade, categoria);
    estoque.adicionarProduto(produto);
}

/**
 * @brief Cadastra um produto perecível
 * @param estoque Referência para o estoque
 */
void cadastrarProdutoPerecivel(Estoque& estoque) {
    std::string nome, descricao, categoria, dataFab, dataVal;
    double preco, temp;
    int quantidade;
    
    limparBuffer();
    
    std::cout << "\n--- Cadastro de Produto Perecível ---" << std::endl;
    
    std::cout << "Nome: ";
    std::getline(std::cin, nome);
    
    std::cout << "Descrição: ";
    std::getline(std::cin, descricao);
    
    std::cout << "Preço: R$ ";
    std::cin >> preco;
    
    std::cout << "Quantidade: ";
    std::cin >> quantidade;
    
    limparBuffer();
    std::cout << "Categoria: ";
    std::getline(std::cin, categoria);
    
    std::cout << "Data de Fabricação (YYYY-MM-DD): ";
    std::getline(std::cin, dataFab);
    
    std::cout << "Data de Validade (YYYY-MM-DD): ";
    std::getline(std::cin, dataVal);
    
    std::cout << "Temperatura de Armazenamento (°C): ";
    std::cin >> temp;
    
    auto produto = std::make_shared<ProdutoPerecivel>(
        nome, descricao, preco, quantidade, categoria,
        dataFab, dataVal, temp
    );
    estoque.adicionarProduto(produto);
}

/**
 * @brief Cadastra um produto não perecível
 * @param estoque Referência para o estoque
 */
void cadastrarProdutoNaoPerecivel(Estoque& estoque) {
    std::string nome, descricao, categoria, material, garantia;
    double preco, peso;
    int quantidade;
    
    limparBuffer();
    
    std::cout << "\n--- Cadastro de Produto Não Perecível ---" << std::endl;
    
    std::cout << "Nome: ";
    std::getline(std::cin, nome);
    
    std::cout << "Descrição: ";
    std::getline(std::cin, descricao);
    
    std::cout << "Preço: R$ ";
    std::cin >> preco;
    
    std::cout << "Quantidade: ";
    std::cin >> quantidade;
    
    limparBuffer();
    std::cout << "Categoria: ";
    std::getline(std::cin, categoria);
    
    std::cout << "Material: ";
    std::getline(std::cin, material);
    
    std::cout << "Peso (kg): ";
    std::cin >> peso;
    
    limparBuffer();
    std::cout << "Garantia: ";
    std::getline(std::cin, garantia);
    
    auto produto = std::make_shared<ProdutoNaoPerecivel>(
        nome, descricao, preco, quantidade, categoria,
        material, peso, garantia
    );
    estoque.adicionarProduto(produto);
}

/**
 * @brief Menu de cadastro de produtos
 * @param estoque Referência para o estoque
 */
void menuCadastro(Estoque& estoque) {
    int opcao;
    
    do {
        exibirMenuCadastro();
        std::cin >> opcao;
        
        switch (opcao) {
            case 1:
                cadastrarProdutoGenerico(estoque);
                pausar();
                break;
            case 2:
                cadastrarProdutoPerecivel(estoque);
                pausar();
                break;
            case 3:
                cadastrarProdutoNaoPerecivel(estoque);
                pausar();
                break;
            case 0:
                break;
            default:
                std::cout << RED << "Opção inválida!" << RESET << std::endl;
        }
    } while (opcao != 0);
}

/**
 * @brief Menu de busca de produtos
 * @param estoque Referência para o estoque
 */
void menuBusca(Estoque& estoque) {
    int opcao;
    
    do {
        exibirMenuBusca();
        std::cin >> opcao;
        
        switch (opcao) {
            case 1: {
                int id;
                std::cout << "Digite o ID do produto: ";
                std::cin >> id;
                auto produto = estoque.buscarPorId(id);
                if (produto) {
                    produto->exibirInfo();
                } else {
                    std::cout << RED << "Produto não encontrado!" << RESET << std::endl;
                }
                pausar();
                break;
            }
            case 2: {
                std::string nome;
                limparBuffer();
                std::cout << "Digite o nome (ou parte): ";
                std::getline(std::cin, nome);
                auto produtos = estoque.buscarPorNome(nome);
                if (produtos.empty()) {
                    std::cout << RED << "Nenhum produto encontrado!" << RESET << std::endl;
                } else {
                    std::cout << GREEN << "\nEncontrados " << produtos.size() << " produto(s):" << RESET << std::endl;
                    for (const auto& p : produtos) {
                        p->exibirInfo();
                    }
                }
                pausar();
                break;
            }
            case 3: {
                std::string categoria;
                limparBuffer();
                std::cout << "Digite a categoria: ";
                std::getline(std::cin, categoria);
                auto produtos = estoque.buscarPorCategoria(categoria);
                if (produtos.empty()) {
                    std::cout << RED << "Nenhum produto encontrado!" << RESET << std::endl;
                } else {
                    std::cout << GREEN << "\nEncontrados " << produtos.size() << " produto(s):" << RESET << std::endl;
                    for (const auto& p : produtos) {
                        p->exibirInfo();
                    }
                }
                pausar();
                break;
            }
            case 0:
                break;
            default:
                std::cout << RED << "Opção inválida!" << RESET << std::endl;
        }
    } while (opcao != 0);
}

/**
 * @brief Atualiza um produto existente
 * @param estoque Referência para o estoque
 */
void atualizarProduto(Estoque& estoque) {
    int id;
    std::cout << "\nDigite o ID do produto a atualizar: ";
    std::cin >> id;
    
    auto produto = estoque.buscarPorId(id);
    if (!produto) {
        std::cout << RED << "Produto não encontrado!" << RESET << std::endl;
        return;
    }
    
    produto->exibirInfo();
    
    std::cout << "\nO que deseja atualizar?" << std::endl;
    std::cout << "1. Nome" << std::endl;
    std::cout << "2. Descrição" << std::endl;
    std::cout << "3. Preço" << std::endl;
    std::cout << "4. Categoria" << std::endl;
    std::cout << "0. Cancelar" << std::endl;
    std::cout << "Opção: ";
    
    int opcao;
    std::cin >> opcao;
    limparBuffer();
    
    switch (opcao) {
        case 1: {
            std::string nome;
            std::cout << "Novo nome: ";
            std::getline(std::cin, nome);
            produto->setNome(nome);
            std::cout << GREEN << "Nome atualizado com sucesso!" << RESET << std::endl;
            break;
        }
        case 2: {
            std::string descricao;
            std::cout << "Nova descrição: ";
            std::getline(std::cin, descricao);
            produto->setDescricao(descricao);
            std::cout << GREEN << "Descrição atualizada com sucesso!" << RESET << std::endl;
            break;
        }
        case 3: {
            double preco;
            std::cout << "Novo preço: R$ ";
            std::cin >> preco;
            produto->setPreco(preco);
            std::cout << GREEN << "Preço atualizado com sucesso!" << RESET << std::endl;
            break;
        }
        case 4: {
            std::string categoria;
            std::cout << "Nova categoria: ";
            std::getline(std::cin, categoria);
            produto->setCategoria(categoria);
            std::cout << GREEN << "Categoria atualizada com sucesso!" << RESET << std::endl;
            break;
        }
        case 0:
            std::cout << "Operação cancelada." << std::endl;
            break;
        default:
            std::cout << RED << "Opção inválida!" << RESET << std::endl;
    }
}

/**
 * @brief Remove um produto do estoque
 * @param estoque Referência para o estoque
 */
void removerProduto(Estoque& estoque) {
    int id;
    std::cout << "\nDigite o ID do produto a remover: ";
    std::cin >> id;
    
    auto produto = estoque.buscarPorId(id);
    if (!produto) {
        std::cout << RED << "Produto não encontrado!" << RESET << std::endl;
        return;
    }
    
    produto->exibirInfo();
    
    char confirma;
    std::cout << "\nConfirma a remoção? (s/n): ";
    std::cin >> confirma;
    
    if (confirma == 's' || confirma == 'S') {
        estoque.removerProduto(id);
    } else {
        std::cout << "Operação cancelada." << std::endl;
    }
}

/**
 * @brief Movimenta o estoque (entrada/saída)
 * @param estoque Referência para o estoque
 */
void movimentarEstoque(Estoque& estoque) {
    int id;
    std::cout << "\nDigite o ID do produto: ";
    std::cin >> id;
    
    auto produto = estoque.buscarPorId(id);
    if (!produto) {
        std::cout << RED << "Produto não encontrado!" << RESET << std::endl;
        return;
    }
    
    produto->exibirInfo();
    
    std::cout << "\n1. Entrada (adicionar)" << std::endl;
    std::cout << "2. Saída (remover)" << std::endl;
    std::cout << "0. Cancelar" << std::endl;
    std::cout << "Opção: ";
    
    int opcao;
    std::cin >> opcao;
    
    if (opcao == 1) {
        int qtd;
        std::cout << "Quantidade a adicionar: ";
        std::cin >> qtd;
        estoque.adicionarQuantidade(id, qtd);
    } else if (opcao == 2) {
        int qtd;
        std::cout << "Quantidade a remover: ";
        std::cin >> qtd;
        estoque.removerQuantidade(id, qtd);
    } else if (opcao != 0) {
        std::cout << RED << "Opção inválida!" << RESET << std::endl;
    }
}

/**
 * @brief Menu de relatórios
 * @param estoque Referência para o estoque
 */
void menuRelatorios(Estoque& estoque) {
    int opcao;
    
    do {
        exibirMenuRelatorios();
        std::cin >> opcao;
        
        switch (opcao) {
            case 1:
                std::cout << estoque.gerarRelatorio();
                pausar();
                break;
            case 2:
                estoque.listarPorCategoria();
                pausar();
                break;
            case 3: {
                int limite;
                std::cout << "Limite mínimo de estoque: ";
                std::cin >> limite;
                estoque.listarEstoqueBaixo(limite);
                pausar();
                break;
            }
            case 4:
                estoque.listarProdutosVencidos();
                pausar();
                break;
            case 5: {
                int dias;
                std::cout << "Dias para vencimento: ";
                std::cin >> dias;
                estoque.listarProximosVencer(dias);
                pausar();
                break;
            }
            case 0:
                break;
            default:
                std::cout << RED << "Opção inválida!" << RESET << std::endl;
        }
    } while (opcao != 0);
}

/**
 * @brief Função principal do programa
 * @return Código de saída
 */
int main() {
    // Cria o estoque
    Estoque estoque("Estoque Principal", "Armazém Central");
    
    // Tenta carregar dados salvos
    std::ifstream arquivo(ARQUIVO_DADOS);
    if (arquivo.good()) {
        arquivo.close();
        estoque.carregarDeArquivo(ARQUIVO_DADOS);
    }
    
    int opcao;
    
    do {
        limparTela();
        exibirCabecalho();
        exibirMenuPrincipal();
        std::cin >> opcao;
        
        switch (opcao) {
            case 1:
                menuCadastro(estoque);
                break;
            case 2:
                estoque.listarTodosProdutos();
                pausar();
                break;
            case 3:
                menuBusca(estoque);
                break;
            case 4:
                atualizarProduto(estoque);
                pausar();
                break;
            case 5:
                removerProduto(estoque);
                pausar();
                break;
            case 6:
                movimentarEstoque(estoque);
                pausar();
                break;
            case 7:
                menuRelatorios(estoque);
                break;
            case 8:
                estoque.salvarEmArquivo(ARQUIVO_DADOS);
                pausar();
                break;
            case 9:
                estoque.carregarDeArquivo(ARQUIVO_DADOS);
                pausar();
                break;
            case 0:
                std::cout << "\n" << GREEN << "Salvando dados..." << RESET << std::endl;
                estoque.salvarEmArquivo(ARQUIVO_DADOS);
                std::cout << CYAN << "Obrigado por usar o Sistema de Estoque!" << RESET << std::endl;
                std::cout << "Até logo!" << std::endl;
                break;
            default:
                std::cout << RED << "Opção inválida!" << RESET << std::endl;
                pausar();
        }
    } while (opcao != 0);
    
    return 0;
}
