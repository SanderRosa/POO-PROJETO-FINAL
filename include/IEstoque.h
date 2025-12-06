#ifndef IESTOQUE_H
#define IESTOQUE_H

#include <string>
#include <vector>

/*
 * Interface para o módulo de estoque (simulado).
 * Define as operações que o módulo de estoque deve realizar quando
 * materiais são comprados e precisam ser registrados no inventário.
 */
class IEstoque {
public:
    virtual ~IEstoque() = default;

    // Registra a entrada de um material no estoque quando uma compra chega
    // idMaterial: ID do material
    // quantidade: quantidade recebida
    // idOrdemCompra: ID da ordem de compra relacionada
    // retorna true se o registro foi bem-sucedido
    virtual bool registrarEntradaCompra(int idMaterial, int quantidade, int idOrdemCompra) = 0;

    // Consulta um item específico do estoque
    // idMaterial: ID do material a consultar
    // retorna a quantidade disponível no estoque (-1 se não encontrado)
    virtual int consultarItem(int idMaterial) = 0;

    // Lista todos os itens do estoque
    // retorna vetor com informações: "ID: X | Nome: Y | Quantidade: Z"
    virtual std::vector<std::string> listarTodosItens() = 0;

    // Verifica a disponibilidade de um material no estoque
    // idMaterial: ID do material a verificar
    // retorna a quantidade disponível no estoque
    virtual int verificarDisponibilidade(int idMaterial) = 0;

    // Reserva uma quantidade de material no estoque para produção
    // idMaterial: ID do material
    // quantidade: quantidade a reservar
    // retorna true se a reserva foi bem-sucedida
    virtual bool reservarMaterial(int idMaterial, int quantidade) = 0;
};

#endif // IESTOQUE_H
