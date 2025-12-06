#ifndef PRODUCAO_MOCK_H
#define PRODUCAO_MOCK_H

#include "IProducao.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <sstream>

/*
 * Implementação mock do módulo de produção.
 * Simula notificações, pedidos de materiais e previsões de entrega.
 */
class ProducaoMock : public IProducao {
private:
    struct PedidoMaterial {
        int idMaterial;
        int quantidade;
        int prioridade;
        bool atendido;
    };
    
    int notificacoesEnviadas;  ///< Contador de notificações enviadas
    bool estaOperacional;      ///< Se o módulo está operacional
    int proximoIdPedido;       ///< Próximo ID de pedido
    std::map<int, PedidoMaterial> pedidos; ///< Mapa de pedidos
    std::map<int, std::string> previsoesEntrega; ///< Ordem -> Data previsão

public:
    // Construtor: inicializa contador e estado operacional
    ProducaoMock() : notificacoesEnviadas(0), estaOperacional(true), proximoIdPedido(1) {
        // Cria alguns pedidos exemplo
        pedidos[1] = {1, 50, 3, false};  // Material 1, alta prioridade
        pedidos[2] = {2, 200, 2, false}; // Material 2, média prioridade
        proximoIdPedido = 3;
    }

    // Notifica o módulo de produção sobre material comprado (simulado)
    bool notificarMaterialComprado(int idMaterial) override {
        if (!estaOperacional) {
            std::cout << "[PRODUCAO] Modulo indisponivel!\n";
            return false;
        }

        notificacoesEnviadas++;

        std::cout << "[PRODUCAO] Notificacao enviada!\n"
                  << "[PRODUCAO]    Material ID: " << idMaterial << "\n"
                  << "[PRODUCAO]    Preparando linha de producao...\n"
                  << "[PRODUCAO] Notificacao recebida com sucesso!\n";

        // Marca pedidos relacionados como atendidos
        for (auto& p : pedidos) {
            if (p.second.idMaterial == idMaterial && !p.second.atendido) {
                p.second.atendido = true;
                std::cout << "[PRODUCAO] Pedido #" << p.first << " marcado como atendido.\n";
            }
        }

        return true;
    }

    int receberPedidoMaterial(int idMaterial, int quantidade, int prioridade) override {
        if (!estaOperacional) {
            std::cout << "[PRODUCAO] Modulo indisponivel!\n";
            return -1;
        }

        int idPedido = proximoIdPedido++;
        pedidos[idPedido] = {idMaterial, quantidade, prioridade, false};

        std::string nivelPrioridade = (prioridade == 3) ? "ALTA" : 
                                      (prioridade == 2) ? "MEDIA" : "BAIXA";

        std::cout << "[PRODUCAO] Pedido de material recebido!\n"
                  << "[PRODUCAO]    Pedido ID: #" << idPedido << "\n"
                  << "[PRODUCAO]    Material ID: " << idMaterial << "\n"
                  << "[PRODUCAO]    Quantidade: " << quantidade << "\n"
                  << "[PRODUCAO]    Prioridade: " << nivelPrioridade << "\n";

        return idPedido;
    }

    bool atualizarPrevisaoEntrega(int idOrdemCompra, const std::string& dataPrevisao) override {
        if (!estaOperacional) {
            std::cout << "[PRODUCAO] Modulo indisponivel!\n";
            return false;
        }

        previsoesEntrega[idOrdemCompra] = dataPrevisao;

        std::cout << "[PRODUCAO] Previsao de entrega atualizada!\n"
                  << "[PRODUCAO]    Ordem de Compra: #" << idOrdemCompra << "\n"
                  << "[PRODUCAO]    Data Prevista: " << dataPrevisao << "\n"
                  << "[PRODUCAO] Linha de producao informada com sucesso!\n";

        return true;
    }

    std::vector<std::string> listarPedidosPendentes() override {
        std::vector<std::string> lista;

        std::cout << "[PRODUCAO] Listando pedidos pendentes:\n";

        bool temPendentes = false;
        for (const auto& p : pedidos) {
            if (!p.second.atendido) {
                temPendentes = true;
                std::string nivelPrioridade = (p.second.prioridade == 3) ? "ALTA" : 
                                              (p.second.prioridade == 2) ? "MEDIA" : "BAIXA";
                
                std::ostringstream oss;
                oss << "Pedido #" << p.first << " | Material: " << p.second.idMaterial
                    << " | Qtd: " << p.second.quantidade 
                    << " | Prioridade: " << nivelPrioridade;
                
                lista.push_back(oss.str());
                std::cout << "[PRODUCAO] " << oss.str() << "\n";
            }
        }

        if (!temPendentes) {
            std::cout << "[PRODUCAO] Nenhum pedido pendente.\n";
            lista.push_back("Nenhum pedido pendente");
        }

        return lista;
    }

    // Define se o módulo está operacional (teste)
    void setOperacional(bool estado) {
        estaOperacional = estado;
    }

    // Retorna o número de notificações enviadas
    int getNotificacoesEnviadas() const {
        return notificacoesEnviadas;
    }

    // Reseta o contador de notificações
    void resetarContador() {
        notificacoesEnviadas = 0;
    }
};

#endif // PRODUCAO_MOCK_H
