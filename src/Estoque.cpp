/**
 * @file Estoque.cpp
 * @brief Implementação da classe Estoque
 * @author Projeto Final POO
 */

#include "Estoque.h"
#include "Produto.h"
#include "ProdutoPerecivel.h"
#include "ProdutoNaoPerecivel.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

Estoque::Estoque() 
    : nomeEstoque("Estoque Principal"), localizacao("Não definida") {
}

Estoque::Estoque(const std::string& nome, const std::string& localizacao)
    : nomeEstoque(nome), localizacao(localizacao) {
}

// Getters
std::string Estoque::getNomeEstoque() const {
    return nomeEstoque;
}

std::string Estoque::getLocalizacao() const {
    return localizacao;
}

// Setters
void Estoque::setNomeEstoque(const std::string& nome) {
    nomeEstoque = nome;
}

void Estoque::setLocalizacao(const std::string& localizacao) {
    this->localizacao = localizacao;
}

// Método auxiliar privado
int Estoque::encontrarIndicePorId(int id) const {
    for (size_t i = 0; i < produtos.size(); ++i) {
        if (produtos[i]->getId() == id) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// Operações CRUD
bool Estoque::adicionarProduto(std::shared_ptr<IProduto> produto) {
    if (!produto) {
        std::cerr << "Erro: Produto inválido!" << std::endl;
        return false;
    }
    
    // Verifica se já existe produto com mesmo ID
    if (buscarPorId(produto->getId()) != nullptr) {
        std::cerr << "Erro: Já existe um produto com ID " << produto->getId() << std::endl;
        return false;
    }
    
    produtos.push_back(produto);
    std::cout << "Produto '" << produto->getNome() << "' adicionado com sucesso! ID: " << produto->getId() << std::endl;
    return true;
}

bool Estoque::removerProduto(int id) {
    int indice = encontrarIndicePorId(id);
    if (indice == -1) {
        std::cerr << "Erro: Produto com ID " << id << " não encontrado!" << std::endl;
        return false;
    }
    
    std::string nome = produtos[indice]->getNome();
    produtos.erase(produtos.begin() + indice);
    std::cout << "Produto '" << nome << "' removido com sucesso!" << std::endl;
    return true;
}

bool Estoque::atualizarProduto(int id, std::shared_ptr<IProduto> produto) {
    int indice = encontrarIndicePorId(id);
    if (indice == -1) {
        std::cerr << "Erro: Produto com ID " << id << " não encontrado!" << std::endl;
        return false;
    }
    
    produtos[indice] = produto;
    std::cout << "Produto atualizado com sucesso!" << std::endl;
    return true;
}

std::shared_ptr<IProduto> Estoque::buscarPorId(int id) const {
    int indice = encontrarIndicePorId(id);
    if (indice != -1) {
        return produtos[indice];
    }
    return nullptr;
}

// Operações de busca
std::vector<std::shared_ptr<IProduto>> Estoque::buscarPorNome(const std::string& nome) const {
    std::vector<std::shared_ptr<IProduto>> resultado;
    
    std::string nomeLower = nome;
    std::transform(nomeLower.begin(), nomeLower.end(), nomeLower.begin(), ::tolower);
    
    for (const auto& produto : produtos) {
        std::string produtoNomeLower = produto->getNome();
        std::transform(produtoNomeLower.begin(), produtoNomeLower.end(), produtoNomeLower.begin(), ::tolower);
        
        if (produtoNomeLower.find(nomeLower) != std::string::npos) {
            resultado.push_back(produto);
        }
    }
    
    return resultado;
}

std::vector<std::shared_ptr<IProduto>> Estoque::buscarPorCategoria(const std::string& categoria) const {
    std::vector<std::shared_ptr<IProduto>> resultado;
    
    std::string categoriaLower = categoria;
    std::transform(categoriaLower.begin(), categoriaLower.end(), categoriaLower.begin(), ::tolower);
    
    for (const auto& produto : produtos) {
        std::string produtoCategoriaLower = produto->getCategoria();
        std::transform(produtoCategoriaLower.begin(), produtoCategoriaLower.end(), 
                       produtoCategoriaLower.begin(), ::tolower);
        
        if (produtoCategoriaLower.find(categoriaLower) != std::string::npos) {
            resultado.push_back(produto);
        }
    }
    
    return resultado;
}

// Operações de estoque
bool Estoque::adicionarQuantidade(int id, int quantidade) {
    auto produto = buscarPorId(id);
    if (!produto) {
        std::cerr << "Erro: Produto com ID " << id << " não encontrado!" << std::endl;
        return false;
    }
    
    produto->adicionarQuantidade(quantidade);
    return true;
}

bool Estoque::removerQuantidade(int id, int quantidade) {
    auto produto = buscarPorId(id);
    if (!produto) {
        std::cerr << "Erro: Produto com ID " << id << " não encontrado!" << std::endl;
        return false;
    }
    
    return produto->removerQuantidade(quantidade);
}

// Relatórios
void Estoque::listarTodosProdutos() const {
    if (produtos.empty()) {
        std::cout << "\n=== ESTOQUE VAZIO ===" << std::endl;
        return;
    }
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "        LISTA DE TODOS OS PRODUTOS" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Total de produtos: " << getTotalProdutos() << std::endl;
    std::cout << "Total de itens: " << getTotalItens() << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Valor total: R$ " << calcularValorTotalEstoque() << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    for (const auto& produto : produtos) {
        produto->exibirInfo();
        std::cout << std::endl;
    }
}

void Estoque::listarEstoqueBaixo(int limiteMinimo) const {
    std::cout << "\n========================================" << std::endl;
    std::cout << "    PRODUTOS COM ESTOQUE BAIXO (<" << limiteMinimo << ")" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    bool encontrou = false;
    for (const auto& produto : produtos) {
        if (produto->getQuantidade() < limiteMinimo) {
            produto->exibirInfo();
            std::cout << std::endl;
            encontrou = true;
        }
    }
    
    if (!encontrou) {
        std::cout << "Nenhum produto com estoque baixo." << std::endl;
    }
}

double Estoque::calcularValorTotalEstoque() const {
    double total = 0.0;
    for (const auto& produto : produtos) {
        total += produto->calcularValorTotal();
    }
    return total;
}

int Estoque::getTotalProdutos() const {
    return static_cast<int>(produtos.size());
}

int Estoque::getTotalItens() const {
    int total = 0;
    for (const auto& produto : produtos) {
        total += produto->getQuantidade();
    }
    return total;
}

// Persistência
bool Estoque::salvarEmArquivo(const std::string& nomeArquivo) const {
    std::ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir arquivo para escrita: " << nomeArquivo << std::endl;
        return false;
    }
    
    // Salva informações do estoque
    arquivo << "ESTOQUE;" << nomeEstoque << ";" << localizacao << std::endl;
    arquivo << "PROXIMO_ID;" << Produto::getProximoId() << std::endl;
    
    // Salva cada produto
    for (const auto& produto : produtos) {
        arquivo << produto->serializar() << std::endl;
    }
    
    arquivo.close();
    std::cout << "Estoque salvo com sucesso em '" << nomeArquivo << "'!" << std::endl;
    return true;
}

bool Estoque::carregarDeArquivo(const std::string& nomeArquivo) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir arquivo para leitura: " << nomeArquivo << std::endl;
        return false;
    }
    
    limparEstoque();
    
    std::string linha;
    while (std::getline(arquivo, linha)) {
        if (linha.empty()) continue;
        
        std::stringstream ss(linha);
        std::string tipo;
        std::getline(ss, tipo, ';');
        
        if (tipo == "ESTOQUE") {
            std::getline(ss, nomeEstoque, ';');
            std::getline(ss, localizacao, ';');
        }
        else if (tipo == "PROXIMO_ID") {
            int proximoId;
            ss >> proximoId;
            Produto::setProximoId(proximoId);
        }
        else if (tipo == "Produto") {
            int id;
            std::string nome, descricao, categoria;
            double preco;
            int quantidade;
            
            ss >> id;
            ss.ignore();
            std::getline(ss, nome, ';');
            std::getline(ss, descricao, ';');
            ss >> preco;
            ss.ignore();
            ss >> quantidade;
            ss.ignore();
            std::getline(ss, categoria, ';');
            
            auto produto = std::make_shared<Produto>(id, nome, descricao, preco, quantidade, categoria);
            produtos.push_back(produto);
        }
        else if (tipo == "ProdutoPerecivel") {
            int id;
            std::string nome, descricao, categoria, dataFab, dataVal;
            double preco, temp;
            int quantidade;
            
            ss >> id;
            ss.ignore();
            std::getline(ss, nome, ';');
            std::getline(ss, descricao, ';');
            ss >> preco;
            ss.ignore();
            ss >> quantidade;
            ss.ignore();
            std::getline(ss, categoria, ';');
            std::getline(ss, dataFab, ';');
            std::getline(ss, dataVal, ';');
            ss >> temp;
            
            auto produto = std::make_shared<ProdutoPerecivel>(
                id, nome, descricao, preco, quantidade, categoria,
                dataFab, dataVal, temp
            );
            produtos.push_back(produto);
        }
        else if (tipo == "ProdutoNaoPerecivel") {
            int id;
            std::string nome, descricao, categoria, material, garantia;
            double preco, peso;
            int quantidade;
            
            ss >> id;
            ss.ignore();
            std::getline(ss, nome, ';');
            std::getline(ss, descricao, ';');
            ss >> preco;
            ss.ignore();
            ss >> quantidade;
            ss.ignore();
            std::getline(ss, categoria, ';');
            std::getline(ss, material, ';');
            ss >> peso;
            ss.ignore();
            std::getline(ss, garantia, ';');
            
            auto produto = std::make_shared<ProdutoNaoPerecivel>(
                id, nome, descricao, preco, quantidade, categoria,
                material, peso, garantia
            );
            produtos.push_back(produto);
        }
    }
    
    arquivo.close();
    std::cout << "Estoque carregado com sucesso de '" << nomeArquivo << "'!" << std::endl;
    return true;
}

std::string Estoque::gerarRelatorio() const {
    std::stringstream ss;
    
    ss << "==========================================================" << std::endl;
    ss << "              RELATÓRIO DE ESTOQUE" << std::endl;
    ss << "==========================================================" << std::endl;
    ss << "Nome do Estoque: " << nomeEstoque << std::endl;
    ss << "Localização: " << localizacao << std::endl;
    ss << "----------------------------------------------------------" << std::endl;
    ss << "Total de produtos diferentes: " << getTotalProdutos() << std::endl;
    ss << "Total de itens em estoque: " << getTotalItens() << std::endl;
    ss << std::fixed << std::setprecision(2);
    ss << "Valor total do estoque: R$ " << calcularValorTotalEstoque() << std::endl;
    ss << "----------------------------------------------------------" << std::endl;
    
    // Estatísticas por categoria
    ss << "Produtos por categoria:" << std::endl;
    auto estatisticas = obterEstatisticasPorCategoria();
    for (const auto& par : estatisticas) {
        ss << "  - " << par.first << ": " << par.second << " produto(s)" << std::endl;
    }
    
    ss << "----------------------------------------------------------" << std::endl;
    ss << "Detalhes dos produtos:" << std::endl;
    ss << "----------------------------------------------------------" << std::endl;
    
    for (const auto& produto : produtos) {
        ss << "ID: " << produto->getId() 
           << " | " << produto->getNome() 
           << " | Qtd: " << produto->getQuantidade()
           << " | R$ " << produto->getPreco() << std::endl;
    }
    
    ss << "==========================================================" << std::endl;
    
    return ss.str();
}

void Estoque::limparEstoque() {
    produtos.clear();
    std::cout << "Estoque limpo com sucesso!" << std::endl;
}

// Métodos adicionais
void Estoque::listarPorCategoria() const {
    auto estatisticas = obterEstatisticasPorCategoria();
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "       PRODUTOS POR CATEGORIA" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    for (const auto& par : estatisticas) {
        std::cout << "\n--- " << par.first << " (" << par.second << " produto(s)) ---" << std::endl;
        auto produtosCategoria = buscarPorCategoria(par.first);
        for (const auto& produto : produtosCategoria) {
            std::cout << "  ID: " << produto->getId() 
                      << " | " << produto->getNome() 
                      << " | Qtd: " << produto->getQuantidade() << std::endl;
        }
    }
}

std::map<std::string, int> Estoque::obterEstatisticasPorCategoria() const {
    std::map<std::string, int> estatisticas;
    for (const auto& produto : produtos) {
        estatisticas[produto->getCategoria()]++;
    }
    return estatisticas;
}

void Estoque::listarProdutosVencidos() const {
    std::cout << "\n========================================" << std::endl;
    std::cout << "       PRODUTOS VENCIDOS" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    bool encontrou = false;
    for (const auto& produto : produtos) {
        // Usa dynamic_pointer_cast diretamente sem remover const
        auto perecivel = std::dynamic_pointer_cast<const ProdutoPerecivel>(produto);
        if (perecivel && perecivel->estaVencido()) {
            perecivel->exibirInfo();
            std::cout << std::endl;
            encontrou = true;
        }
    }
    
    if (!encontrou) {
        std::cout << "Nenhum produto vencido encontrado." << std::endl;
    }
}

void Estoque::listarProximosVencer(int dias) const {
    std::cout << "\n========================================" << std::endl;
    std::cout << "  PRODUTOS PRÓXIMOS DO VENCIMENTO (" << dias << " dias)" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    bool encontrou = false;
    for (const auto& produto : produtos) {
        // Usa dynamic_pointer_cast diretamente sem remover const
        auto perecivel = std::dynamic_pointer_cast<const ProdutoPerecivel>(produto);
        if (perecivel) {
            int diasParaVencer = perecivel->diasParaVencer();
            if (diasParaVencer >= 0 && diasParaVencer <= dias) {
                perecivel->exibirInfo();
                std::cout << std::endl;
                encontrou = true;
            }
        }
    }
    
    if (!encontrou) {
        std::cout << "Nenhum produto próximo do vencimento." << std::endl;
    }
}
