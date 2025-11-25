/**
 * @file Estoque.h
 * @brief Classe Estoque - implementa a interface IEstoque
 * @author Projeto Final POO
 */

#ifndef ESTOQUE_H
#define ESTOQUE_H

#include "IEstoque.h"
#include "IProduto.h"
#include <vector>
#include <memory>
#include <string>
#include <map>

/**
 * @class Estoque
 * @brief Classe que implementa o gerenciamento de estoque
 * 
 * Esta classe implementa a interface IEstoque e fornece
 * funcionalidades completas para gerenciamento de produtos.
 */
class Estoque : public IEstoque {
private:
    std::vector<std::shared_ptr<IProduto>> produtos;
    std::string nomeEstoque;
    std::string localizacao;

    /**
     * @brief Encontra o índice de um produto pelo ID
     * @param id ID do produto
     * @return Índice do produto ou -1 se não encontrado
     */
    int encontrarIndicePorId(int id) const;

public:
    /**
     * @brief Construtor padrão
     */
    Estoque();

    /**
     * @brief Construtor com nome e localização
     * @param nome Nome do estoque
     * @param localizacao Localização do estoque
     */
    Estoque(const std::string& nome, const std::string& localizacao);

    /**
     * @brief Destrutor virtual
     */
    virtual ~Estoque() = default;

    // Getters
    std::string getNomeEstoque() const;
    std::string getLocalizacao() const;

    // Setters
    void setNomeEstoque(const std::string& nome);
    void setLocalizacao(const std::string& localizacao);

    // Implementação das operações CRUD
    bool adicionarProduto(std::shared_ptr<IProduto> produto) override;
    bool removerProduto(int id) override;
    bool atualizarProduto(int id, std::shared_ptr<IProduto> produto) override;
    std::shared_ptr<IProduto> buscarPorId(int id) const override;

    // Implementação das operações de busca
    std::vector<std::shared_ptr<IProduto>> buscarPorNome(const std::string& nome) const override;
    std::vector<std::shared_ptr<IProduto>> buscarPorCategoria(const std::string& categoria) const override;

    // Implementação das operações de estoque
    bool adicionarQuantidade(int id, int quantidade) override;
    bool removerQuantidade(int id, int quantidade) override;

    // Implementação dos relatórios
    void listarTodosProdutos() const override;
    void listarEstoqueBaixo(int limiteMinimo) const override;
    double calcularValorTotalEstoque() const override;
    int getTotalProdutos() const override;
    int getTotalItens() const override;

    // Implementação da persistência
    bool salvarEmArquivo(const std::string& nomeArquivo) const override;
    bool carregarDeArquivo(const std::string& nomeArquivo) override;
    std::string gerarRelatorio() const override;
    void limparEstoque() override;

    // Métodos adicionais
    /**
     * @brief Lista produtos por categoria
     */
    void listarPorCategoria() const;

    /**
     * @brief Obtém estatísticas do estoque
     */
    std::map<std::string, int> obterEstatisticasPorCategoria() const;

    /**
     * @brief Lista produtos vencidos (para produtos perecíveis)
     */
    void listarProdutosVencidos() const;

    /**
     * @brief Lista produtos próximos do vencimento
     * @param dias Número de dias para considerar
     */
    void listarProximosVencer(int dias) const;
};

#endif // ESTOQUE_H
