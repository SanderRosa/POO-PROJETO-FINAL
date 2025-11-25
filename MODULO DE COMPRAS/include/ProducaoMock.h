#ifndef PRODUCAO_MOCK_H
#define PRODUCAO_MOCK_H

#include "IProducao.h"
#include <iostream>
#include <iomanip>

/*
 * Implementação mock do módulo de produção.
 * Simula notificações para o módulo de produção quando um material é comprado.
 */
class ProducaoMock : public IProducao {
private:
    int notificacoesEnviadas;  ///< Contador de notificações enviadas
    bool estaOperacional;      ///< Se o módulo está operacional

public:
    // Construtor: inicializa contador e estado operacional
    ProducaoMock() : notificacoesEnviadas(0), estaOperacional(true) {}

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

        return true;
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
