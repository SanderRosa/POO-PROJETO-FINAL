#ifndef FORNECEDOR_H
#define FORNECEDOR_H

#include <string>
#include "Pessoa.h"

/*
 * Classe Fornecedor que herda de Pessoa.
 * Representa um fornecedor no sistema. Fornecedores são pessoas (física ou jurídica)
 * que vendem materiais/serviços à empresa.
 * Demonstra herança (nome e endereço vêm de Pessoa) e polimorfismo
 * (implementa o método exibirDetalhes()).
 */
class Fornecedor : public Pessoa {
private:
    std::string cnpj;  // CNPJ do fornecedor
    int id;            // ID único do fornecedor no sistema
    std::string produto; // Produto fornecido
    double precoProduto; // Preço do produto fornecido

public:
    // Construtor padrão
    Fornecedor() : Pessoa(), cnpj(""), id(0), produto(""), precoProduto(0.0) {}

    // Construtor parametrizado: recebe nome, endereço, CNPJ, identificador, produto e preço
    Fornecedor(const std::string& n, const std::string& e, 
               const std::string& c, int identificador, const std::string& prod, double preco)
        : Pessoa(n, e), cnpj(c), id(identificador), produto(prod), precoProduto(preco) {}

    // Construtor legado usado pela persistencia antiga (sem produto/preco)
    Fornecedor(const std::string& n, const std::string& e, const std::string& c, int identificador)
        : Pessoa(n, e), cnpj(c), id(identificador), produto(""), precoProduto(0.0) {}

    // Destrutor padrão
    ~Fornecedor() override = default;

    // Getters
    std::string getCNPJ() const { return cnpj; }
    int getId() const { return id; }
    std::string getProduto() const { return produto; }
    double getPrecoProduto() const { return precoProduto; }

    // Setters
    void setCNPJ(const std::string& c) { cnpj = c; }
    void setId(int identificador) { id = identificador; }
    void setProduto(const std::string& prod) { produto = prod; }
    void setPrecoProduto(double preco) { precoProduto = preco; }

    // Implementação de exibirDetalhes() que formata e retorna as informações do fornecedor
    std::string exibirDetalhes() const override {
        return "ID: " + std::to_string(id) + 
               "  Nome: " + nome +
               "  CNPJ: " + cnpj +
               "  Endereco: " + endereco +
               "  Produto: " + produto +
               " | Preco: R$ " + std::to_string(precoProduto);
    }

    // Compara dois fornecedores pelo ID (útil para buscas e operações)
    bool operator==(const Fornecedor& outro) const {
        return this->id == outro.id;
    }
};

#endif // FORNECEDOR_H
