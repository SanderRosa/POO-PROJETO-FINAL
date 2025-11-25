#ifndef COMPRAS_EXCEPTION_H
#define COMPRAS_EXCEPTION_H

#include <exception>
#include <string>

// Classe de exceção customizada para o módulo de compras.
// Herda de std::exception e armazena uma mensagem descritiva do erro.
class ComprasException : public std::exception {
private:
    std::string mensagem; ///< Armazena a mensagem de erro

public:
    // Construtor que recebe uma mensagem de erro
    explicit ComprasException(const std::string& msg) : mensagem(msg) {}

    // Retorna a mensagem de erro em formato C-string
    const char* what() const noexcept override {
        return mensagem.c_str();
    }

    // Obtém a mensagem de erro completa como std::string
    std::string getMensagem() const {
        return mensagem;
    }
};

#endif // COMPRAS_EXCEPTION_H
