/**
 * @file ProdutoPerecivel.h
 * @brief Classe ProdutoPerecivel - herda de Produto
 * @author Projeto Final POO
 */

#ifndef PRODUTO_PERECIVEL_H
#define PRODUTO_PERECIVEL_H

#include "Produto.h"
#include <string>

/**
 * @class ProdutoPerecivel
 * @brief Classe que representa produtos perecíveis com data de validade
 * 
 * Esta classe demonstra o conceito de herança em POO,
 * estendendo a classe Produto com atributos específicos.
 */
class ProdutoPerecivel : public Produto {
private:
    std::string dataFabricacao;
    std::string dataValidade;
    double temperaturaArmazenamento;

public:
    /**
     * @brief Construtor padrão
     */
    ProdutoPerecivel();

    /**
     * @brief Construtor com parâmetros
     */
    ProdutoPerecivel(const std::string& nome, const std::string& descricao, 
                     double preco, int quantidade, const std::string& categoria,
                     const std::string& dataFabricacao, const std::string& dataValidade,
                     double temperaturaArmazenamento);

    /**
     * @brief Construtor com ID específico (para carregar de arquivo)
     */
    ProdutoPerecivel(int id, const std::string& nome, const std::string& descricao, 
                     double preco, int quantidade, const std::string& categoria,
                     const std::string& dataFabricacao, const std::string& dataValidade,
                     double temperaturaArmazenamento);

    /**
     * @brief Destrutor virtual
     */
    virtual ~ProdutoPerecivel() = default;

    // Getters específicos
    std::string getDataFabricacao() const;
    std::string getDataValidade() const;
    double getTemperaturaArmazenamento() const;

    // Setters específicos
    void setDataFabricacao(const std::string& data);
    void setDataValidade(const std::string& data);
    void setTemperaturaArmazenamento(double temp);

    // Métodos específicos
    /**
     * @brief Verifica se o produto está vencido
     * @return true se vencido, false caso contrário
     */
    bool estaVencido() const;

    /**
     * @brief Calcula os dias até o vencimento
     * @return Número de dias (negativo se já venceu)
     */
    int diasParaVencer() const;

    // Sobrescrita de métodos da classe base (polimorfismo)
    void exibirInfo() const override;
    std::string getTipo() const override;
    std::string serializar() const override;
};

#endif // PRODUTO_PERECIVEL_H
