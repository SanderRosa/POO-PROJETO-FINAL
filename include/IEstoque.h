/**
 * @file IEstoque.h
 * @brief Interface para o Estoque - gerenciamento de produtos
 * @author Projeto Final POO
 */

#ifndef IESTOQUE_H
#define IESTOQUE_H

#include <vector>
#include <memory>
#include "IProduto.h"

/**
 * @class IEstoque
 * @brief Interface abstrata que define o contrato para operações de estoque
 * 
 * Esta interface define os métodos para gerenciamento de estoque,
 * incluindo operações CRUD e funcionalidades de busca e relatórios.
 */
class IEstoque {
public:
    /**
     * @brief Destrutor virtual para permitir polimorfismo
     */
    virtual ~IEstoque() = default;

    // Operações CRUD
    /**
     * @brief Adiciona um novo produto ao estoque
     * @param produto Ponteiro para o produto a ser adicionado
     * @return true se adicionado com sucesso, false caso contrário
     */
    virtual bool adicionarProduto(std::shared_ptr<IProduto> produto) = 0;

    /**
     * @brief Remove um produto do estoque pelo ID
     * @param id ID do produto a ser removido
     * @return true se removido com sucesso, false caso contrário
     */
    virtual bool removerProduto(int id) = 0;

    /**
     * @brief Atualiza um produto existente no estoque
     * @param id ID do produto a ser atualizado
     * @param produto Novo produto com dados atualizados
     * @return true se atualizado com sucesso, false caso contrário
     */
    virtual bool atualizarProduto(int id, std::shared_ptr<IProduto> produto) = 0;

    /**
     * @brief Busca um produto pelo ID
     * @param id ID do produto
     * @return Ponteiro para o produto ou nullptr se não encontrado
     */
    virtual std::shared_ptr<IProduto> buscarPorId(int id) const = 0;

    // Operações de busca
    /**
     * @brief Busca produtos pelo nome
     * @param nome Nome ou parte do nome do produto
     * @return Vetor de produtos encontrados
     */
    virtual std::vector<std::shared_ptr<IProduto>> buscarPorNome(const std::string& nome) const = 0;

    /**
     * @brief Busca produtos por categoria
     * @param categoria Categoria dos produtos
     * @return Vetor de produtos encontrados
     */
    virtual std::vector<std::shared_ptr<IProduto>> buscarPorCategoria(const std::string& categoria) const = 0;

    // Operações de estoque
    /**
     * @brief Adiciona quantidade a um produto
     * @param id ID do produto
     * @param quantidade Quantidade a adicionar
     * @return true se adicionado com sucesso, false caso contrário
     */
    virtual bool adicionarQuantidade(int id, int quantidade) = 0;

    /**
     * @brief Remove quantidade de um produto
     * @param id ID do produto
     * @param quantidade Quantidade a remover
     * @return true se removido com sucesso, false caso contrário
     */
    virtual bool removerQuantidade(int id, int quantidade) = 0;

    // Relatórios
    /**
     * @brief Lista todos os produtos do estoque
     */
    virtual void listarTodosProdutos() const = 0;

    /**
     * @brief Lista produtos com estoque baixo
     * @param limiteMinimo Quantidade mínima considerada baixa
     */
    virtual void listarEstoqueBaixo(int limiteMinimo) const = 0;

    /**
     * @brief Calcula o valor total do estoque
     * @return Valor total em reais
     */
    virtual double calcularValorTotalEstoque() const = 0;

    /**
     * @brief Retorna a quantidade total de produtos no estoque
     * @return Número total de produtos diferentes
     */
    virtual int getTotalProdutos() const = 0;

    /**
     * @brief Retorna a quantidade total de itens no estoque
     * @return Soma de todas as quantidades
     */
    virtual int getTotalItens() const = 0;

    // Persistência
    /**
     * @brief Salva os dados do estoque em arquivo
     * @param nomeArquivo Nome do arquivo para salvar
     * @return true se salvo com sucesso, false caso contrário
     */
    virtual bool salvarEmArquivo(const std::string& nomeArquivo) const = 0;

    /**
     * @brief Carrega os dados do estoque de um arquivo
     * @param nomeArquivo Nome do arquivo para carregar
     * @return true se carregado com sucesso, false caso contrário
     */
    virtual bool carregarDeArquivo(const std::string& nomeArquivo) = 0;

    /**
     * @brief Gera um relatório do estoque
     * @return String com o relatório formatado
     */
    virtual std::string gerarRelatorio() const = 0;

    /**
     * @brief Limpa todos os produtos do estoque
     */
    virtual void limparEstoque() = 0;
};

#endif // IESTOQUE_H
