/**
 * @file Produto.h
 * @brief Classe base Produto - implementa a interface IProduto
 * @author Projeto Final POO
 */

#ifndef PRODUTO_H
#define PRODUTO_H

#include "IProduto.h"
#include <string>

/**
 * @class Produto
 * @brief Classe base que implementa um produto genérico
 * 
 * Esta classe implementa a interface IProduto e serve como base
 * para classes derivadas (herança).
 */
class Produto : public IProduto {
protected:
    int id;
    std::string nome;
    std::string descricao;
    double preco;
    int quantidade;
    std::string categoria;
    
    static int proximoId; // Para auto-incremento de ID

public:
    /**
     * @brief Construtor padrão
     */
    Produto();

    /**
     * @brief Construtor com parâmetros
     * @param nome Nome do produto
     * @param descricao Descrição do produto
     * @param preco Preço unitário
     * @param quantidade Quantidade em estoque
     * @param categoria Categoria do produto
     */
    Produto(const std::string& nome, const std::string& descricao, 
            double preco, int quantidade, const std::string& categoria);

    /**
     * @brief Construtor com ID específico (para carregar de arquivo)
     * @param id ID do produto
     * @param nome Nome do produto
     * @param descricao Descrição do produto
     * @param preco Preço unitário
     * @param quantidade Quantidade em estoque
     * @param categoria Categoria do produto
     */
    Produto(int id, const std::string& nome, const std::string& descricao, 
            double preco, int quantidade, const std::string& categoria);

    /**
     * @brief Destrutor virtual
     */
    virtual ~Produto() = default;

    // Implementação dos getters da interface
    int getId() const override;
    std::string getNome() const override;
    std::string getDescricao() const override;
    double getPreco() const override;
    int getQuantidade() const override;
    std::string getCategoria() const override;

    // Implementação dos setters da interface
    void setNome(const std::string& nome) override;
    void setDescricao(const std::string& descricao) override;
    void setPreco(double preco) override;
    void setQuantidade(int quantidade) override;
    void setCategoria(const std::string& categoria) override;

    // Implementação dos métodos de operação
    void adicionarQuantidade(int qtd) override;
    bool removerQuantidade(int qtd) override;
    double calcularValorTotal() const override;
    
    // Implementação dos métodos de exibição
    void exibirInfo() const override;
    std::string getTipo() const override;
    
    // Implementação do método de serialização
    std::string serializar() const override;

    // Método estático para resetar o contador de IDs
    static void resetarContadorId();
    static void setProximoId(int id);
    static int getProximoId();
};

#endif // PRODUTO_H
