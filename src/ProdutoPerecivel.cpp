/**
 * @file ProdutoPerecivel.cpp
 * @brief Implementação da classe ProdutoPerecivel
 * @author Projeto Final POO
 */

#include "ProdutoPerecivel.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

ProdutoPerecivel::ProdutoPerecivel()
    : Produto(), dataFabricacao(""), dataValidade(""), temperaturaArmazenamento(0.0) {
}

ProdutoPerecivel::ProdutoPerecivel(const std::string& nome, const std::string& descricao, 
                                   double preco, int quantidade, const std::string& categoria,
                                   const std::string& dataFabricacao, const std::string& dataValidade,
                                   double temperaturaArmazenamento)
    : Produto(nome, descricao, preco, quantidade, categoria),
      dataFabricacao(dataFabricacao), dataValidade(dataValidade),
      temperaturaArmazenamento(temperaturaArmazenamento) {
}

ProdutoPerecivel::ProdutoPerecivel(int id, const std::string& nome, const std::string& descricao, 
                                   double preco, int quantidade, const std::string& categoria,
                                   const std::string& dataFabricacao, const std::string& dataValidade,
                                   double temperaturaArmazenamento)
    : Produto(id, nome, descricao, preco, quantidade, categoria),
      dataFabricacao(dataFabricacao), dataValidade(dataValidade),
      temperaturaArmazenamento(temperaturaArmazenamento) {
}

// Getters específicos
std::string ProdutoPerecivel::getDataFabricacao() const {
    return dataFabricacao;
}

std::string ProdutoPerecivel::getDataValidade() const {
    return dataValidade;
}

double ProdutoPerecivel::getTemperaturaArmazenamento() const {
    return temperaturaArmazenamento;
}

// Setters específicos
void ProdutoPerecivel::setDataFabricacao(const std::string& data) {
    dataFabricacao = data;
}

void ProdutoPerecivel::setDataValidade(const std::string& data) {
    dataValidade = data;
}

void ProdutoPerecivel::setTemperaturaArmazenamento(double temp) {
    temperaturaArmazenamento = temp;
}

// Métodos específicos
bool ProdutoPerecivel::estaVencido() const {
    // Obtém data atual
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    
    char dataAtual[11];
    strftime(dataAtual, sizeof(dataAtual), "%Y-%m-%d", now);
    
    // Compara strings de data (formato YYYY-MM-DD)
    return std::string(dataAtual) > dataValidade;
}

int ProdutoPerecivel::diasParaVencer() const {
    // Parse da data de validade (formato: YYYY-MM-DD)
    int anoVal, mesVal, diaVal;
    if (sscanf(dataValidade.c_str(), "%d-%d-%d", &anoVal, &mesVal, &diaVal) != 3) {
        return -1; // Erro no formato
    }
    
    // Data atual
    time_t t = time(nullptr);
    
    // Cria estrutura tm para data de validade
    tm validade = {};
    validade.tm_year = anoVal - 1900;
    validade.tm_mon = mesVal - 1;
    validade.tm_mday = diaVal;
    
    // Calcula diferença em segundos e converte para dias
    time_t validadeTime = mktime(&validade);
    double diff = difftime(validadeTime, t);
    
    return static_cast<int>(diff / (60 * 60 * 24));
}

// Sobrescrita de métodos (polimorfismo)
void ProdutoPerecivel::exibirInfo() const {
    std::cout << "========================================" << std::endl;
    std::cout << "ID: " << getId() << std::endl;
    std::cout << "Tipo: " << getTipo() << std::endl;
    std::cout << "Nome: " << getNome() << std::endl;
    std::cout << "Descrição: " << getDescricao() << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Preço: R$ " << getPreco() << std::endl;
    std::cout << "Quantidade: " << getQuantidade() << std::endl;
    std::cout << "Categoria: " << getCategoria() << std::endl;
    std::cout << "Valor Total: R$ " << calcularValorTotal() << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Data de Fabricação: " << dataFabricacao << std::endl;
    std::cout << "Data de Validade: " << dataValidade << std::endl;
    std::cout << "Temperatura de Armazenamento: " << temperaturaArmazenamento << "°C" << std::endl;
    
    if (estaVencido()) {
        std::cout << "*** PRODUTO VENCIDO! ***" << std::endl;
    } else {
        int dias = diasParaVencer();
        if (dias <= 7) {
            std::cout << "*** ATENÇÃO: Vence em " << dias << " dias! ***" << std::endl;
        }
    }
    
    std::cout << "========================================" << std::endl;
}

std::string ProdutoPerecivel::getTipo() const {
    return "ProdutoPerecivel";
}

std::string ProdutoPerecivel::serializar() const {
    std::stringstream ss;
    ss << getTipo() << ";"
       << getId() << ";"
       << getNome() << ";"
       << getDescricao() << ";"
       << getPreco() << ";"
       << getQuantidade() << ";"
       << getCategoria() << ";"
       << dataFabricacao << ";"
       << dataValidade << ";"
       << temperaturaArmazenamento;
    return ss.str();
}
