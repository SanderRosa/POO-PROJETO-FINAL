/**
 * @file ProdutoNaoPerecivel.cpp
 * @brief Implementação da classe ProdutoNaoPerecivel
 * @author Projeto Final POO
 */

#include "ProdutoNaoPerecivel.h"
#include <iostream>
#include <sstream>
#include <iomanip>

ProdutoNaoPerecivel::ProdutoNaoPerecivel()
    : Produto(), material(""), peso(0.0), garantia("") {
}

ProdutoNaoPerecivel::ProdutoNaoPerecivel(const std::string& nome, const std::string& descricao, 
                                         double preco, int quantidade, const std::string& categoria,
                                         const std::string& material, double peso, const std::string& garantia)
    : Produto(nome, descricao, preco, quantidade, categoria),
      material(material), peso(peso), garantia(garantia) {
}

ProdutoNaoPerecivel::ProdutoNaoPerecivel(int id, const std::string& nome, const std::string& descricao, 
                                         double preco, int quantidade, const std::string& categoria,
                                         const std::string& material, double peso, const std::string& garantia)
    : Produto(id, nome, descricao, preco, quantidade, categoria),
      material(material), peso(peso), garantia(garantia) {
}

// Getters específicos
std::string ProdutoNaoPerecivel::getMaterial() const {
    return material;
}

double ProdutoNaoPerecivel::getPeso() const {
    return peso;
}

std::string ProdutoNaoPerecivel::getGarantia() const {
    return garantia;
}

// Setters específicos
void ProdutoNaoPerecivel::setMaterial(const std::string& material) {
    this->material = material;
}

void ProdutoNaoPerecivel::setPeso(double peso) {
    if (peso >= 0) {
        this->peso = peso;
    } else {
        std::cerr << "Erro: Peso não pode ser negativo!" << std::endl;
    }
}

void ProdutoNaoPerecivel::setGarantia(const std::string& garantia) {
    this->garantia = garantia;
}

// Métodos específicos
double ProdutoNaoPerecivel::calcularFrete(double taxaKg) const {
    return peso * taxaKg * getQuantidade();
}

// Sobrescrita de métodos (polimorfismo)
void ProdutoNaoPerecivel::exibirInfo() const {
    std::cout << "========================================" << std::endl;
    std::cout << "ID: " << getId() << std::endl;
    std::cout << "Tipo: " << getTipo() << std::endl;
    std::cout << "Nome: " << getNome() << std::endl;
    std::cout << "Descrição: " << getDescricao() << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Preço: R$ " << getPreco() << std::endl;
    std::cout << "Quantidade: " << getQuantidade() << std::endl;
    std::cout << "Categoria: " << getCategoria() << std::endl;
    std::cout << "Valor Total: R$ " << calcularValorTotal() << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Material: " << material << std::endl;
    std::cout << "Peso: " << peso << " kg" << std::endl;
    std::cout << "Garantia: " << garantia << std::endl;
    std::cout << "========================================" << std::endl;
}

std::string ProdutoNaoPerecivel::getTipo() const {
    return "ProdutoNaoPerecivel";
}

std::string ProdutoNaoPerecivel::serializar() const {
    std::stringstream ss;
    ss << getTipo() << ";"
       << getId() << ";"
       << getNome() << ";"
       << getDescricao() << ";"
       << getPreco() << ";"
       << getQuantidade() << ";"
       << getCategoria() << ";"
       << material << ";"
       << peso << ";"
       << garantia;
    return ss.str();
}
