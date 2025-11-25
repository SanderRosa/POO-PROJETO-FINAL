#ifndef PERSISTENCIA_COMPRAS_H
#define PERSISTENCIA_COMPRAS_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Fornecedor.h"
#include "OrdemCompra.h"
#include "ListaGenerica.h"
#include "ComprasException.h"

/*
 * Gerenciador de persistência de dados.
 * Responsável por carregar e salvar fornecedores e ordens em arquivos
 * no formato pipe-delimitado.
 */
class PersistenciaCompras {
private:
    std::string caminhoFornecedores;
    std::string caminhoOrdens;

public:
    PersistenciaCompras(const std::string& caminhoForn = "data/fornecedores.txt",
                       const std::string& caminhoOrd = "data/ordens.txt");

    // Fornecedores
    void salvarFornecedores(const ListaGenerica<Fornecedor>& lista);
    void carregarFornecedores(ListaGenerica<Fornecedor>& lista, int& proximoId);

    // Ordens
    void salvarOrdens(const ListaGenerica<OrdemCompra>& lista);
    void carregarOrdens(ListaGenerica<OrdemCompra>& lista, int& proximoId);
};

#endif // PERSISTENCIA_COMPRAS_H
