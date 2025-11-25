/**
 * @file IProduto.h
 * @brief Interface para produtos - demonstra polimorfismo em POO
 * @author Projeto Final POO
 */

#ifndef IPRODUTO_H
#define IPRODUTO_H

#include <string>

/**
 * @class IProduto
 * @brief Interface abstrata que define o contrato para produtos
 * 
 * Esta interface define os métodos que todo produto deve implementar,
 * demonstrando o conceito de polimorfismo em Programação Orientada a Objetos.
 */
class IProduto {
public:
    /**
     * @brief Destrutor virtual para permitir polimorfismo
     */
    virtual ~IProduto() = default;

    // Getters
    virtual int getId() const = 0;
    virtual std::string getNome() const = 0;
    virtual std::string getDescricao() const = 0;
    virtual double getPreco() const = 0;
    virtual int getQuantidade() const = 0;
    virtual std::string getCategoria() const = 0;

    // Setters
    virtual void setNome(const std::string& nome) = 0;
    virtual void setDescricao(const std::string& descricao) = 0;
    virtual void setPreco(double preco) = 0;
    virtual void setQuantidade(int quantidade) = 0;
    virtual void setCategoria(const std::string& categoria) = 0;

    // Métodos de operação
    virtual void adicionarQuantidade(int qtd) = 0;
    virtual bool removerQuantidade(int qtd) = 0;
    virtual double calcularValorTotal() const = 0;
    
    // Métodos de exibição
    virtual void exibirInfo() const = 0;
    virtual std::string getTipo() const = 0;
    
    // Método para serialização
    virtual std::string serializar() const = 0;
};

#endif // IPRODUTO_H
