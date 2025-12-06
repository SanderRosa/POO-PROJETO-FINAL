#ifndef LISTA_GENERICA_H
#define LISTA_GENERICA_H

#include <vector>
#include <algorithm>
#include <stdexcept>

/*
 * Template ListaGenerica para armazenar elementos de qualquer tipo.
 * Implementa templates em C++ e fornece operações básicas: adicionar,
 * remover, listar e buscar. Usa std::vector internamente.
 * T é o tipo de dado armazenado na lista.
 */
template <typename T>
class ListaGenerica {
private:
    std::vector<T> elementos;  ///< Vetor que armazena os elementos

public:
    // Construtor padrão - cria uma lista vazia
    ListaGenerica() {}

    // Destrutor padrão
    ~ListaGenerica() {
        elementos.clear();
    }

    // Adiciona um elemento ao final da lista
    void adicionar(const T& elemento) {
        elementos.push_back(elemento);
    }

    // Remove um elemento da lista baseado no índice (lança out_of_range se inválido)
    void remover(size_t indice) {
        if (indice >= elementos.size()) {
            throw std::out_of_range("Índice fora do intervalo da lista");
        }
        elementos.erase(elementos.begin() + indice);
    }

    // Remove o primeiro elemento igual ao fornecido; retorna true se removido
    bool removerPor(const T& elemento) {
        auto it = std::find(elementos.begin(), elementos.end(), elemento);
        if (it != elementos.end()) {
            elementos.erase(it);
            return true;
        }
        return false;
    }

    // Retorna a quantidade de elementos na lista
    size_t obterTamanho() const {
        return elementos.size();
    }

    // Obtém um elemento pelo índice (lança out_of_range se inválido)
    const T& obter(size_t indice) const {
        if (indice >= elementos.size()) {
            throw std::out_of_range("Índice fora do intervalo da lista");
        }
        return elementos[indice];
    }

    // Obtém um elemento para modificação (lança out_of_range se inválido)
    T& obterMutavel(size_t indice) {
        if (indice >= elementos.size()) {
            throw std::out_of_range("Índice fora do intervalo da lista");
        }
        return elementos[indice];
    }

    // Verifica se a lista está vazia
    bool estaVazia() const {
        return elementos.empty();
    }

    // Limpa todos os elementos da lista
    void limpar() {
        elementos.clear();
    }

    // Busca um elemento pela posição e o retorna (usa obter)
    T buscarPor(size_t indice) const {
        return obter(indice);
    }

    // Retorna o vetor interno (referência constante)
    const std::vector<T>& obterVetor() const {
        return elementos;
    }

    // Operador de acesso via índice (leitura)
    const T& operator[](size_t indice) const {
        return elementos[indice];
    }

    // Operador de acesso via índice (escrita)
    T& operator[](size_t indice) {
        return elementos[indice];
    }
};

#endif // LISTA_GENERICA_H
