#ifndef IPRODUCAO_H
#define IPRODUCAO_H

/*
 * Interface para o módulo de produção (simulado).
 * Define as operações que o módulo de produção deve realizar quando
 * um material é comprado com sucesso.
 */
class IProducao {
public:
    virtual ~IProducao() = default;

    // Notifica o módulo de produção sobre a compra de um material
    // idMaterial: ID do material comprado
    // retorna true se a notificação foi processada com sucesso
    virtual bool notificarMaterialComprado(int idMaterial) = 0;
};

#endif // IPRODUCAO_H
