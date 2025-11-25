#ifndef IPRODUCAO_H
#define IPRODUCAO_H

#include <string>
#include <vector>

/*
 * Interface para o módulo de produção (simulado).
 * Define as operações que o módulo de produção deve realizar quando
 * um material é comprado com sucesso e para solicitar materiais.
 */
class IProducao {
public:
    virtual ~IProducao() = default;

    // Notifica o módulo de produção sobre a compra de um material
    // idMaterial: ID do material comprado
    // retorna true se a notificação foi processada com sucesso
    virtual bool notificarMaterialComprado(int idMaterial) = 0;

    // Recebe um pedido de material da produção
    // idMaterial: ID do material solicitado
    // quantidade: quantidade necessária
    // prioridade: nível de prioridade (1=baixa, 2=média, 3=alta)
    // retorna ID do pedido gerado
    virtual int receberPedidoMaterial(int idMaterial, int quantidade, int prioridade) = 0;

    // Informa a previsão de entrega de um material
    // idOrdemCompra: ID da ordem de compra
    // dataPrevisao: data prevista de entrega
    // retorna true se a atualização foi bem-sucedida
    virtual bool atualizarPrevisaoEntrega(int idOrdemCompra, const std::string& dataPrevisao) = 0;

    // Lista todos os pedidos de materiais pendentes
    // retorna vetor com informações dos pedidos
    virtual std::vector<std::string> listarPedidosPendentes() = 0;
};

#endif // IPRODUCAO_H
