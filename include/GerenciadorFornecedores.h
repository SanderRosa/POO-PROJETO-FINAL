#ifndef GERENCIADOR_FORNECEDORES_H
#define GERENCIADOR_FORNECEDORES_H

#include <mutex>
#include <memory>
#include "Fornecedor.h"
#include "ListaGenerica.h"
#include "ComprasException.h"

/*
 * Gerenciador de fornecedores.
 * Responsável por adicionar, listar, buscar e remover fornecedores.
 * Opera de forma thread-safe usando mutex para proteger acesso concorrente.
 */
class GerenciadorFornecedores {
private:
    ListaGenerica<Fornecedor> fornecedores;
    int proximoId;
    mutable std::mutex mutex;

public:
    GerenciadorFornecedores();
    ~GerenciadorFornecedores();

    int adicionar(const std::string& nome, const std::string& endereco, const std::string& cnpj, const std::string& produto, double precoProduto);

    // Lista fornecedores de um determinado produto
    void listarPorProduto(const std::string& produto) const;

    // Lista fornecedores ordenados por preço do produto (maior para menor)
    void listarOrdenadoPorPreco() const;
    void listar() const;
    Fornecedor* buscarPorId(int id);
    void remover(int id);
    size_t obterQuantidade() const;
    
    // Acesso para persistencia
    const ListaGenerica<Fornecedor>& obterLista() const;
    void carregarDeLista(const ListaGenerica<Fornecedor>& lista, int proximoIdArmazenado);
};

#endif // GERENCIADOR_FORNECEDORES_H
