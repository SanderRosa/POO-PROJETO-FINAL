#ifndef MODULO_COMPRAS_H
#define MODULO_COMPRAS_H

#include <memory>
#include <iostream>

#include "GerenciadorFornecedores.h"
#include "GerenciadorOrdens.h"
#include "PersistenciaCompras.h"
#include "ComprasException.h"

/*
 * Classe coordenadora do módulo de compras.
 * Coordena GerenciadorFornecedores, GerenciadorOrdens e PersistenciaCompras.
 * Cada responsabilidade está em seu próprio módulo.
 */
class ModuloCompras {
private:
    std::unique_ptr<GerenciadorFornecedores> gerenciadorFornecedores;
    std::unique_ptr<GerenciadorOrdens> gerenciadorOrdens;
    std::unique_ptr<PersistenciaCompras> persistencia;

public:
    // Construtor: inicializa os módulos internos
    ModuloCompras();

    // Destrutor: libera recursos
    ~ModuloCompras();

    // ========== OPERACOES COM FORNECEDORES ==========

    int adicionarFornecedor(const std::string& nome, const std::string& endereco,
                           const std::string& cnpj, const std::string& produto, double precoProduto) {
        return gerenciadorFornecedores->adicionar(nome, endereco, cnpj, produto, precoProduto);
    }

    void listarFornecedoresPorProduto(const std::string& produto) const {
        gerenciadorFornecedores->listarPorProduto(produto);
    }

    void listarFornecedoresOrdenadoPorPreco() const {
        gerenciadorFornecedores->listarOrdenadoPorPreco();
    }

    void listarFornecedores() const {
        gerenciadorFornecedores->listar();
    }

    Fornecedor* buscarFornecedorPorId(int id) {
        return gerenciadorFornecedores->buscarPorId(id);
    }

    void removerFornecedor(int id) {
        gerenciadorFornecedores->remover(id);
    }

    size_t obterQuantidadeFornecedores() const {
        return gerenciadorFornecedores->obterQuantidade();
    }

    // ========== OPERACOES COM ORDENS DE COMPRA ==========

    int criarOrdemCompra(int idItem, int quantidade, double valorUnitario, int idFornecedor) {
        if (buscarFornecedorPorId(idFornecedor) == nullptr) {
            throw ComprasException("Fornecedor nao encontrado!");
        }
        return gerenciadorOrdens->criar(idItem, quantidade, valorUnitario, idFornecedor);
    }

    void listarOrdens() const {
        gerenciadorOrdens->listar();
    }

    OrdemCompra* buscarOrdenPorId(int id) {
        return gerenciadorOrdens->buscarPorId(id);
    }

    size_t obterQuantidadeOrdens() const {
        return gerenciadorOrdens->obterQuantidade();
    }

    void exibirEstatisticas() const {
        std::cout << "\nTotal de Fornecedores: " << obterQuantidadeFornecedores() << "\n";
        std::cout << "Total de Ordens: " << obterQuantidadeOrdens() << "\n";
        gerenciadorOrdens->exibirEstatisticas();
    }

    // ========== PERSISTENCIA DE DADOS ==========

    void salvarTodosDados() {
        persistencia->salvarFornecedores(gerenciadorFornecedores->obterLista());
        persistencia->salvarOrdens(gerenciadorOrdens->obterLista());
        std::cout << "Dados salvos com sucesso!\n";
    }

    void carregarTodosDados();

    // ========== MODULOS SIMULADOS ==========

    FinanceiroMock* getModuloFinanceiro() {
        return gerenciadorOrdens->getModuloFinanceiro();
    }

    ProducaoMock* getModuloProducao() {
        return gerenciadorOrdens->getModuloProducao();
    }

    // Metodos auxiliares para menu
    auto obterTodasAsOrdens() const {
        return std::make_unique<ListaGenerica<OrdemCompra>>(gerenciadorOrdens->obterLista());
    }

    void exibirTodasAsOrdens() const {
        gerenciadorOrdens->listar();
    }
};

#endif // MODULO_COMPRAS_H
