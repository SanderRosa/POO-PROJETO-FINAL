#ifndef FINANCEIRO_MOCK_H
#define FINANCEIRO_MOCK_H

#include "IFinanceiro.h"
#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>
#include <thread>
#include <vector>
#include <sstream>
#include <map>

/*
 * Implementação mock do módulo financeiro.
 * Simula latência, decisões e logs para testes locais.
 */
class FinanceiroMock : public IFinanceiro {
private:
    struct ContaPagar {
        int idOrdemCompra;
        double valorTotal;
        std::string fornecedor;
        std::string dataVencimento;
        bool paga;
    };
    
    double saldoDisponivel;     ///< Saldo simulado do módulo
    bool estaOperacional;       ///< Se o módulo está operacional
    std::map<int, ContaPagar> contasPagar; ///< Contas a pagar registradas

public:
    // Construtor: inicializa saldo padrão e estado operacional
    FinanceiroMock() : saldoDisponivel(100000.0), estaOperacional(true) {}

    // Define o saldo disponível (uso em testes)
    void setSaldoDisponivel(double saldo) {
        saldoDisponivel = saldo;
    }

    // Verifica disponibilidade simulada: imprime log, dorme 2-4s e compara saldo
    bool verificarDisponibilidade(double valor) override {
        if (!estaOperacional) {
            std::cout << "[FINANCEIRO] Modulo indisponivel!\n";
            return false;
        }

        std::cout << "[FINANCEIRO] Verificando disponibilidade de R$ " 
                  << std::fixed << std::setprecision(2) << valor << "...\n";
        
        // Simula latencia de comunicacao (2-4 segundos)
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(2000, 4000); // 2-4 segundos
        int latencia_ms = dis(gen);
        
        std::cout << "[FINANCEIRO] Aguardando resposta (latencia: " 
                  << latencia_ms << "ms)...\n";
        
        // Dorme na thread atual para simular latencia
        std::this_thread::sleep_for(std::chrono::milliseconds(latencia_ms));

        bool resultado = (valor <= saldoDisponivel);
        
        if (resultado) {
            std::cout << "[FINANCEIRO] Verba DISPONIVEL!\n";
        } else {
            std::cout << "[FINANCEIRO] Verba INSUFICIENTE!\n";
            std::cout << "[FINANCEIRO]    Saldo: R$ " << saldoDisponivel 
                      << " | Solicitado: R$ " << valor << "\n";
        }

        return resultado;
    }

    // Autoriza pagamento (simulado): log, espera 1-2s e retorna sucesso
    bool autorizarPagamento(int idOrdem) override {
        if (!estaOperacional) {
            std::cout << "[FINANCEIRO] Modulo indisponivel!\n";
            return false;
        }

        std::cout << "[FINANCEIRO] Autorizando pagamento para ordem #" 
                  << idOrdem << "...\n";
        
        // Simula processamento (1-2 segundos)
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1000, 2000); // 1-2 segundos
        int latencia_ms = dis(gen);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(latencia_ms));

        std::cout << "[FINANCEIRO] Pagamento AUTORIZADO!\n";
        return true;
    }

    bool registrarContaPagar(int idOrdemCompra, double valorTotal, 
                            const std::string& fornecedor, 
                            const std::string& dataVencimento) override {
        if (!estaOperacional) {
            std::cout << "[FINANCEIRO] Modulo indisponivel!\n";
            return false;
        }

        contasPagar[idOrdemCompra] = {idOrdemCompra, valorTotal, fornecedor, dataVencimento, false};

        std::cout << "[FINANCEIRO] Conta a pagar registrada!\n"
                  << "[FINANCEIRO]    Ordem de Compra: #" << idOrdemCompra << "\n"
                  << "[FINANCEIRO]    Fornecedor: " << fornecedor << "\n"
                  << "[FINANCEIRO]    Valor: R$ " << std::fixed << std::setprecision(2) 
                  << valorTotal << "\n"
                  << "[FINANCEIRO]    Vencimento: " << dataVencimento << "\n"
                  << "[FINANCEIRO] Registro concluido com sucesso!\n";

        return true;
    }

    std::vector<std::string> listarContasPagar() override {
        std::vector<std::string> lista;

        std::cout << "[FINANCEIRO] Listando contas a pagar:\n";

        if (contasPagar.empty()) {
            std::cout << "[FINANCEIRO] Nenhuma conta a pagar registrada.\n";
            lista.push_back("Nenhuma conta a pagar");
        } else {
            for (const auto& c : contasPagar) {
                std::ostringstream oss;
                oss << "Ordem #" << c.second.idOrdemCompra 
                    << " | Fornecedor: " << c.second.fornecedor
                    << " | Valor: R$ " << std::fixed << std::setprecision(2) << c.second.valorTotal
                    << " | Venc: " << c.second.dataVencimento
                    << " | Status: " << (c.second.paga ? "PAGA" : "PENDENTE");
                
                lista.push_back(oss.str());
                std::cout << "[FINANCEIRO] " << oss.str() << "\n";
            }
        }

        return lista;
    }

    // Define se o módulo está operacional (teste)
    void setOperacional(bool estado) {
        estaOperacional = estado;
    }

    // Retorna o saldo simulado
    double getSaldo() const {
        return saldoDisponivel;
    }
};

#endif // FINANCEIRO_MOCK_H
