#ifndef IFINANCEIRO_H
#define IFINANCEIRO_H

#include <string>
#include <vector>

// Interface para o módulo financeiro (simulado).
// Define operações para verificar disponibilidade de verba, autorizar pagamentos
// e registrar contas a pagar.
class IFinanceiro {
public:
    virtual ~IFinanceiro() = default;

    // Verifica se há verba disponível para uma compra (simulado).
    // Recebe o valor a ser verificado e retorna true se houver orçamento.
    virtual bool verificarDisponibilidade(double valor) = 0;

    // Autoriza o pagamento de uma ordem de compra (simulado).
    // Deve ser chamado após verificação de disponibilidade.
    virtual bool autorizarPagamento(int idOrdem) = 0;

    // Registra uma compra como conta a pagar no financeiro
    // idOrdemCompra: ID da ordem de compra
    // valorTotal: valor total da compra
    // fornecedor: nome do fornecedor
    // dataVencimento: data de vencimento do pagamento
    // retorna true se o registro foi bem-sucedido
    virtual bool registrarContaPagar(int idOrdemCompra, double valorTotal, 
                                     const std::string& fornecedor, 
                                     const std::string& dataVencimento) = 0;

    // Lista todas as contas a pagar registradas
    // retorna vetor com informações das contas
    virtual std::vector<std::string> listarContasPagar() = 0;
};

#endif // IFINANCEIRO_H
