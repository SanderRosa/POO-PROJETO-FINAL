/**
 * @file Produto.cpp
 * @brief Implementação da classe Produto
 * @author Projeto Final POO
 */

#include "Produto.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// Inicialização do contador estático
int Produto::proximoId = 1;

Produto::Produto() 
    : id(proximoId++), nome(""), descricao(""), preco(0.0), quantidade(0), categoria("") {
}

Produto::Produto(const std::string& nome, const std::string& descricao, 
                 double preco, int quantidade, const std::string& categoria)
    : id(proximoId++), nome(nome), descricao(descricao), 
      preco(preco), quantidade(quantidade), categoria(categoria) {
}

Produto::Produto(int id, const std::string& nome, const std::string& descricao, 
                 double preco, int quantidade, const std::string& categoria)
    : id(id), nome(nome), descricao(descricao), 
      preco(preco), quantidade(quantidade), categoria(categoria) {
    // Atualiza o próximo ID se necessário
    if (id >= proximoId) {
        proximoId = id + 1;
    }
}

// Getters
int Produto::getId() const {
    return id;
}

std::string Produto::getNome() const {
    return nome;
}

std::string Produto::getDescricao() const {
    return descricao;
}

double Produto::getPreco() const {
    return preco;
}

int Produto::getQuantidade() const {
    return quantidade;
}

std::string Produto::getCategoria() const {
    return categoria;
}

// Setters
void Produto::setNome(const std::string& nome) {
    this->nome = nome;
}

void Produto::setDescricao(const std::string& descricao) {
    this->descricao = descricao;
}

void Produto::setPreco(double preco) {
    if (preco >= 0) {
        this->preco = preco;
    } else {
        std::cerr << "Erro: Preço não pode ser negativo!" << std::endl;
    }
}

void Produto::setQuantidade(int quantidade) {
    if (quantidade >= 0) {
        this->quantidade = quantidade;
    } else {
        std::cerr << "Erro: Quantidade não pode ser negativa!" << std::endl;
    }
}

void Produto::setCategoria(const std::string& categoria) {
    this->categoria = categoria;
}

// Métodos de operação
void Produto::adicionarQuantidade(int qtd) {
    if (qtd > 0) {
        quantidade += qtd;
        std::cout << "Adicionadas " << qtd << " unidades. Nova quantidade: " << quantidade << std::endl;
    } else {
        std::cerr << "Erro: Quantidade a adicionar deve ser positiva!" << std::endl;
    }
}

bool Produto::removerQuantidade(int qtd) {
    if (qtd <= 0) {
        std::cerr << "Erro: Quantidade a remover deve ser positiva!" << std::endl;
        return false;
    }
    
    if (qtd > quantidade) {
        std::cerr << "Erro: Quantidade insuficiente em estoque! Disponível: " << quantidade << std::endl;
        return false;
    }
    
    quantidade -= qtd;
    std::cout << "Removidas " << qtd << " unidades. Nova quantidade: " << quantidade << std::endl;
    return true;
}

double Produto::calcularValorTotal() const {
    return preco * quantidade;
}

// Métodos de exibição
void Produto::exibirInfo() const {
    std::cout << "========================================" << std::endl;
    std::cout << "ID: " << id << std::endl;
    std::cout << "Tipo: " << getTipo() << std::endl;
    std::cout << "Nome: " << nome << std::endl;
    std::cout << "Descrição: " << descricao << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Preço: R$ " << preco << std::endl;
    std::cout << "Quantidade: " << quantidade << std::endl;
    std::cout << "Categoria: " << categoria << std::endl;
    std::cout << "Valor Total: R$ " << calcularValorTotal() << std::endl;
    std::cout << "========================================" << std::endl;
}

std::string Produto::getTipo() const {
    return "Produto";
}

// Serialização para arquivo
std::string Produto::serializar() const {
    std::stringstream ss;
    ss << getTipo() << ";"
       << id << ";"
       << nome << ";"
       << descricao << ";"
       << preco << ";"
       << quantidade << ";"
       << categoria;
    return ss.str();
}

// Métodos estáticos
void Produto::resetarContadorId() {
    proximoId = 1;
}

void Produto::setProximoId(int id) {
    proximoId = id;
}

int Produto::getProximoId() {
    return proximoId;
}
