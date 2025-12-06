#ifndef GERENCIADOR_ORDENS_H
#define GERENCIADOR_ORDENS_H

#include <mutex>
#include <memory>
#include "OrdemCompra.h"
#include "ListaGenerica.h"
#include "ComprasException.h"
#include "FinanceiroMock.h"
#include "ProducaoMock.h"
#include "EstoqueMock.h"
#include <thread>

/*
 * Gerenciador de ordens de compra.
 * Responsável por criar, listar e buscar ordens, implementar concorrência
 * com threads/mutex e integrar com módulos de financeiro, produção e estoque.
 */
class GerenciadorOrdens {
private:
    ListaGenerica<OrdemCompra> ordens;
    int proximoId;
    mutable std::mutex mutex;
    
    std::unique_ptr<FinanceiroMock> modulo_financeiro;
    std::unique_ptr<ProducaoMock> modulo_producao;
    std::unique_ptr<EstoqueMock> modulo_estoque;

    void threadVerificarVerba(double valor, bool* resultado);

public:
    GerenciadorOrdens();
    ~GerenciadorOrdens();

    int criar(int idItem, int quantidade, double valorUnitario, int idFornecedor);
    void listar() const;
    OrdemCompra* buscarPorId(int id);
    size_t obterQuantidade() const;
    void exibirEstatisticas() const;
    
    // Acesso para persistencia
    const ListaGenerica<OrdemCompra>& obterLista() const;
    void carregarDeLista(const ListaGenerica<OrdemCompra>& lista, int proximoIdArmazenado);
    
    // Acesso aos modulos
    FinanceiroMock* getModuloFinanceiro() { return modulo_financeiro.get(); }
    ProducaoMock* getModuloProducao() { return modulo_producao.get(); }
    EstoqueMock* getModuloEstoque() { return modulo_estoque.get(); }
};

#endif // GERENCIADOR_ORDENS_H
