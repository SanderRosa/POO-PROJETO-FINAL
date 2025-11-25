#ifndef IFINANCEIRO_H
#define IFINANCEIRO_H

// Interface para o módulo financeiro (simulado).
// Define operações para verificar disponibilidade de verba e autorizar pagamentos.
class IFinanceiro {
public:
    virtual ~IFinanceiro() = default;

    // Verifica se há verba disponível para uma compra (simulado).
    // Recebe o valor a ser verificado e retorna true se houver orçamento.
    virtual bool verificarDisponibilidade(double valor) = 0;

    // Autoriza o pagamento de uma ordem de compra (simulado).
    // Deve ser chamado após verificação de disponibilidade.
    virtual bool autorizarPagamento(int idOrdem) = 0;
};

#endif // IFINANCEIRO_H
