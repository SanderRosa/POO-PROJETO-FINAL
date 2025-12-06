#ifndef PESSOA_H
#define PESSOA_H

#include <string>
#include "IExibivel.h"

/* Classe abstrata base para Pessoas
     Define atributos comuns a todas as pessoas do sistema (fornecedores, clientes, etc).
     Esta classe eh abstrata e nao deve ser instanciada diretamente.
 */
class Pessoa : public IExibivel {
protected:
    std::string nome;      ///< Nome da pessoa
    std::string endereco;  ///< Endereço de contato

public:
    // Construtor padrao
    Pessoa() : nome(""), endereco("") {}

    // Construtor parametrizado: inicializa nome e endereco
    Pessoa(const std::string& n, const std::string& e) 
        : nome(n), endereco(e) {}

    // Destrutor virtual para permitir polimorfismo
    virtual ~Pessoa() = default;

    // Getters
    std::string getNome() const { return nome; }
    std::string getEndereco() const { return endereco; }

    // Setters
    void setNome(const std::string& n) { nome = n; }
    void setEndereco(const std::string& e) { endereco = e; }

    // Metodo puro: subclasses implementam exibirDetalhes retornando detalhes formatados
    virtual std::string exibirDetalhes() const override = 0;
};

#endif // PESSOA_H
