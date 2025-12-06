#ifndef ORDEM_COMPRA_H
#define ORDEM_COMPRA_H

#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "IExibivel.h"

/*
 * Enum que representa os possíveis status de uma ordem de compra.
 * Estados possíveis:
 * - PENDENTE: Aguardando análise de verba
 * - APROVADO: Verba autorizada
 * - REJEITADO: Verba não disponível ou erro
 * - ENVIADO: Ordem enviada ao fornecedor
 * - ENTREGUE: Material recebido
 */
enum class StatusOrdem {
    PENDENTE = 0,
    APROVADO = 1,
    REJEITADO = 2,
    ENVIADO = 3,
    ENTREGUE = 4
};

/*
 * Classe que representa uma Ordem de Compra.
 * Contém id do item, quantidade, status, data da solicitação e valor unitário.
 * Implementa IExibivel para exibição formatada.
 */
class OrdemCompra : public IExibivel {
private:
    int idTransacao;               ///< ID único da transação/ordem
    int idItem;                    ///< ID do item/material a ser comprado
    int quantidade;                ///< Quantidade a ser comprada
    StatusOrdem status;            ///< Status atual da ordem
    std::string dataSolicitacao;   ///< Data em que foi solicitada
    std::string dataChegadaPrevista; ///< Data prevista de chegada
    double valorUnitario;          ///< Valor unitário do item
    int idFornecedor;              ///< ID do fornecedor

public:
    // Construtor padrão
    OrdemCompra() 
        : idTransacao(0), idItem(0), quantidade(0), 
          status(StatusOrdem::PENDENTE), dataSolicitacao(""), 
          dataChegadaPrevista(""), valorUnitario(0.0), idFornecedor(0) {}

    // Construtor parametrizado: inicializa campos e define a data atual como solicitação
    OrdemCompra(int idTx, int idI, int qtd, double valorUnit, int idForn, const std::string& dataChegada = "")
        : idTransacao(idTx), idItem(idI), quantidade(qtd),
          status(StatusOrdem::PENDENTE), dataChegadaPrevista(dataChegada), valorUnitario(valorUnit),
          idFornecedor(idForn) {
        
        // Define a data atual da solicitação
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time), "%d/%m/%Y %H:%M:%S");
        dataSolicitacao = oss.str();
    }

    // Destrutor padrão
    ~OrdemCompra() override = default;

    // ========== GETTERS ==========
    int getIdTransacao() const { return idTransacao; }
    int getIdItem() const { return idItem; }
    int getQuantidade() const { return quantidade; }
    StatusOrdem getStatus() const { return status; }
    std::string getDataSolicitacao() const { return dataSolicitacao; }
    std::string getDataChegadaPrevista() const { return dataChegadaPrevista; }
    double getValorUnitario() const { return valorUnitario; }
    int getIdFornecedor() const { return idFornecedor; }
    
    double getValorTotal() const {
        return valorUnitario * quantidade;
    }

    // ========== SETTERS ==========
    void setStatus(StatusOrdem novoStatus) { status = novoStatus; }
    void setQuantidade(int qtd) { quantidade = qtd; }
    void setValorUnitario(double valor) { valorUnitario = valor; }
    void setDataSolicitacao(const std::string& data) { dataSolicitacao = data; }
    void setDataChegadaPrevista(const std::string& data) { dataChegadaPrevista = data; }

    // Retorna o status como string para exibição
    std::string getStatusString() const {
        switch(status) {
            case StatusOrdem::PENDENTE:
                return "PENDENTE";
            case StatusOrdem::APROVADO:
                return "APROVADO";
            case StatusOrdem::REJEITADO:
                return "REJEITADO";
            case StatusOrdem::ENVIADO:
                return "ENVIADO";
            case StatusOrdem::ENTREGUE:
                return "ENTREGUE";
            default:
                return "DESCONHECIDO";
        }
    }

    // Implementação de exibirDetalhes() que formata e retorna as informações da ordem
    std::string exibirDetalhes() const override {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        
        oss << "Ordem de Compra \n"
            << " ID Transacao: " << idTransacao << "\n"
            << " ID Item: " << idItem << "\n"
            << " Quantidade: " << quantidade << "\n"
            << " Valor Unitario: R$ " << valorUnitario << "\n"
            << " Valor Total: R$ " << getValorTotal() << "\n"
            << " Status: " << getStatusString() << "\n"
            << " Fornecedor ID: " << idFornecedor << "\n"
            << " Data Solicitacao: " << dataSolicitacao << "\n"
            << " Data Prevista de Chegada: " << (dataChegadaPrevista.empty() ? "Nao informada" : dataChegadaPrevista) << "\n";

        
        return oss.str();
    }

    // Compara duas ordens pelo ID de transação
    bool operator==(const OrdemCompra& outro) const {
        return this->idTransacao == outro.idTransacao;
    }
};

#endif // ORDEM_COMPRA_H
