#ifndef IEXIBIVEL_H
#define IEXIBIVEL_H

#include <string>

// Interface para objetos que podem ser exibidos.
// Classes que implementam esta interface devem fornecer um método
// para exibir seus detalhes de forma legível.
class IExibivel {
public:
    virtual ~IExibivel() = default;

    // Exibe os detalhes do objeto.
    // Implementações devem retornar uma string formatada com os dados relevantes.
    virtual std::string exibirDetalhes() const = 0;
};

#endif // IEXIBIVEL_H
