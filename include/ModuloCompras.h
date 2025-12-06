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

    int criarOrdemCompra(int idItem, int quantidade, double valorUnitario, int idFornecedor, const std::string& dataChegada = "") {
        if (buscarFornecedorPorId(idFornecedor) == nullptr) {
            throw ComprasException("Fornecedor nao encontrado!");
        }
        return gerenciadorOrdens->criar(idItem, quantidade, valorUnitario, idFornecedor, dataChegada);
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

    EstoqueMock* getModuloEstoque() {
        return gerenciadorOrdens->getModuloEstoque();
    }

    // ========== OPERACOES COM ESTOQUE ==========

    int consultarEstoque(int idMaterial) {
        return gerenciadorOrdens->getModuloEstoque()->consultarItem(idMaterial);
    }

    void listarTodosItensEstoque() {
        auto lista = gerenciadorOrdens->getModuloEstoque()->listarTodosItens();
        std::cout << "\n=== ITENS DO ESTOQUE ===\n";
        for (const auto& item : lista) {
            std::cout << item << "\n";
        }
        std::cout << "========================\n";
    }

    bool reservarMaterial(int idMaterial, int quantidade) {
        return gerenciadorOrdens->getModuloEstoque()->reservarMaterial(idMaterial, quantidade);
    }

    void exibirInventarioEstoque() {
        gerenciadorOrdens->getModuloEstoque()->exibirInventario();
    }

    // ========== OPERACOES COM PRODUCAO ==========

    int criarPedidoMaterial(int idMaterial, int quantidade, int prioridade) {
        return gerenciadorOrdens->getModuloProducao()->receberPedidoMaterial(idMaterial, quantidade, prioridade);
    }

    void listarPedidosProducaoPendentes() {
        auto lista = gerenciadorOrdens->getModuloProducao()->listarPedidosPendentes();
        std::cout << "\n=== PEDIDOS DE PRODUCAO PENDENTES ===\n";
        for (const auto& pedido : lista) {
            std::cout << pedido << "\n";
        }
        std::cout << "=====================================\n";
    }

    // ========== OPERACOES COM FINANCEIRO ==========

    void listarContasPagar() {
        auto lista = gerenciadorOrdens->getModuloFinanceiro()->listarContasPagar();
        std::cout << "\n=== CONTAS A PAGAR ===\n";
        for (const auto& conta : lista) {
            std::cout << conta << "\n";
        }
        std::cout << "======================\n";
    }

    double consultarSaldoFinanceiro() {
        return gerenciadorOrdens->getModuloFinanceiro()->getSaldo();
    }

    // Metodos auxiliares para menu
    auto obterTodasAsOrdens() const {
        return std::make_unique<ListaGenerica<OrdemCompra>>(gerenciadorOrdens->obterLista());
    }

    const ListaGenerica<OrdemCompra>& obterListaOrdens() const {
        return gerenciadorOrdens->obterLista();
    }

    const ListaGenerica<Fornecedor>& obterListaFornecedores() const {
        return gerenciadorFornecedores->obterLista();
    }

    void exibirTodasAsOrdens() const {
        gerenciadorOrdens->listar();
    }
};

#endif // MODULO_COMPRAS_H
