#ifndef ESTOQUE_MOCK_H
#define ESTOQUE_MOCK_H

#include "IEstoque.h"
#include <iostream>
#include <map>
#include <vector>
#include <sstream>

/*
 * Implementação simulada do módulo de estoque.
 * Usado para demonstrar a integração com o módulo de compras.
 * Em um sistema real, seria substituído por um módulo de estoque completo.
 */
class EstoqueMock : public IEstoque {
private:
    struct ItemEstoque {
        std::string nome;
        int quantidade;
        int idUltimaOrdem;
    };
    
    std::map<int, ItemEstoque> inventario; // idMaterial -> dados do item

public:
    EstoqueMock() {
        std::cout << "[EstoqueMock] Módulo de estoque inicializado (simulado).\n";
        // Inicializa com alguns itens exemplo
        inventario[1] = {"Aço Inox", 100, 0};
        inventario[2] = {"Parafusos M10", 500, 0};
        inventario[3] = {"Borracha Industrial", 50, 0};
    }

    ~EstoqueMock() override {
        std::cout << "[EstoqueMock] Módulo de estoque finalizado.\n";
    }

    bool registrarEntradaCompra(int idMaterial, int quantidade, int idOrdemCompra) override {
        std::cout << "[EstoqueMock] Registrando entrada de compra no estoque:\n"
                  << "             Material ID " << idMaterial << ", Quantidade: " 
                  << quantidade << ", Ordem: #" << idOrdemCompra << "\n";
        
        // Se o material não existe, cria com nome genérico
        if (inventario.find(idMaterial) == inventario.end()) {
            inventario[idMaterial] = {"Material " + std::to_string(idMaterial), 0, 0};
        }
        
        // Adiciona a quantidade
        inventario[idMaterial].quantidade += quantidade;
        inventario[idMaterial].idUltimaOrdem = idOrdemCompra;
        
        std::cout << "[EstoqueMock] Entrada registrada com sucesso. "
                  << "Quantidade total: " << inventario[idMaterial].quantidade << "\n";
        return true;
    }

    int consultarItem(int idMaterial) override {
        auto it = inventario.find(idMaterial);
        if (it != inventario.end()) {
            std::cout << "[EstoqueMock] Consulta - Material ID " << idMaterial 
                      << " (" << it->second.nome << "): " 
                      << it->second.quantidade << " unidades\n";
            return it->second.quantidade;
        }
        
        std::cout << "[EstoqueMock] Material ID " << idMaterial 
                  << " não encontrado no estoque\n";
        return -1;
    }

    std::vector<std::string> listarTodosItens() override {
        std::vector<std::string> lista;
        
        std::cout << "[EstoqueMock] Listando todos os itens do estoque:\n";
        
        if (inventario.empty()) {
            std::cout << "[EstoqueMock] Estoque vazio.\n";
            lista.push_back("Estoque vazio");
        } else {
            for (const auto& item : inventario) {
                std::ostringstream oss;
                oss << "ID: " << item.first << " | Nome: " << item.second.nome 
                    << " | Quantidade: " << item.second.quantidade;
                lista.push_back(oss.str());
                std::cout << "[EstoqueMock] " << oss.str() << "\n";
            }
        }
        
        return lista;
    }

    int verificarDisponibilidade(int idMaterial) override {
        auto it = inventario.find(idMaterial);
        if (it != inventario.end()) {
            std::cout << "[EstoqueMock] Material ID " << idMaterial 
                      << " disponível: " << it->second.quantidade << " unidades\n";
            return it->second.quantidade;
        }
        
        std::cout << "[EstoqueMock] Material ID " << idMaterial 
                  << " não encontrado no estoque (0 unidades)\n";
        return 0;
    }

    bool reservarMaterial(int idMaterial, int quantidade) override {
        auto it = inventario.find(idMaterial);
        
        if (it == inventario.end() || it->second.quantidade < quantidade) {
            std::cout << "[EstoqueMock] FALHA: Quantidade insuficiente para reservar. "
                      << "Material ID " << idMaterial << ", Solicitado: " << quantidade 
                      << ", Disponível: " << (it != inventario.end() ? it->second.quantidade : 0) << "\n";
            return false;
        }
        
        // Reduz a quantidade disponível
        inventario[idMaterial].quantidade -= quantidade;
        
        std::cout << "[EstoqueMock] Material reservado com sucesso. "
                  << "Material ID " << idMaterial << ", Quantidade: " << quantidade 
                  << ", Restante: " << inventario[idMaterial].quantidade << "\n";
        return true;
    }

    // Método auxiliar para exibir o inventário completo (não faz parte da interface)
    void exibirInventario() const {
        std::cout << "\n=== INVENTÁRIO DO ESTOQUE ===\n";
        if (inventario.empty()) {
            std::cout << "Estoque vazio.\n";
        } else {
            for (const auto& item : inventario) {
                std::cout << "Material ID " << item.first 
                          << " (" << item.second.nome << "): " 
                          << item.second.quantidade << " unidades\n";
            }
        }
        std::cout << "============================\n\n";
    }
};

#endif // ESTOQUE_MOCK_H
