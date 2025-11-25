/**
 * @file ProdutoNaoPerecivel.h
 * @brief Classe ProdutoNaoPerecivel - herda de Produto
 * @author Projeto Final POO
 */

#ifndef PRODUTO_NAO_PERECIVEL_H
#define PRODUTO_NAO_PERECIVEL_H

#include "Produto.h"
#include <string>

/**
 * @class ProdutoNaoPerecivel
 * @brief Classe que representa produtos não perecíveis
 * 
 * Esta classe demonstra o conceito de herança em POO,
 * estendendo a classe Produto com atributos específicos.
 */
class ProdutoNaoPerecivel : public Produto {
private:
    std::string material;
    double peso;
    std::string garantia;

public:
    /**
     * @brief Construtor padrão
     */
    ProdutoNaoPerecivel();

    /**
     * @brief Construtor com parâmetros
     */
    ProdutoNaoPerecivel(const std::string& nome, const std::string& descricao, 
                        double preco, int quantidade, const std::string& categoria,
                        const std::string& material, double peso, const std::string& garantia);

    /**
     * @brief Construtor com ID específico (para carregar de arquivo)
     */
    ProdutoNaoPerecivel(int id, const std::string& nome, const std::string& descricao, 
                        double preco, int quantidade, const std::string& categoria,
                        const std::string& material, double peso, const std::string& garantia);

    /**
     * @brief Destrutor virtual
     */
    virtual ~ProdutoNaoPerecivel() = default;

    // Getters específicos
    std::string getMaterial() const;
    double getPeso() const;
    std::string getGarantia() const;

    // Setters específicos
    void setMaterial(const std::string& material);
    void setPeso(double peso);
    void setGarantia(const std::string& garantia);

    // Métodos específicos
    /**
     * @brief Calcula o custo de frete baseado no peso
     * @param taxaKg Taxa por quilograma
     * @return Valor do frete
     */
    double calcularFrete(double taxaKg) const;

    // Sobrescrita de métodos da classe base (polimorfismo)
    void exibirInfo() const override;
    std::string getTipo() const override;
    std::string serializar() const override;
};

#endif // PRODUTO_NAO_PERECIVEL_H
