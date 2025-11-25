#include "ModuloCompras.h"
#include <iostream>

// Construtor da classe ModuloCompras.
// É chamado automaticamente quando um objeto desta classe é criado.
ModuloCompras::ModuloCompras() {
    // Inicializa o ponteiro único (unique_ptr) para o GerenciadorFornecedores.
    // std::make_unique cria uma nova instância da classe na memória heap de forma segura.
    gerenciadorFornecedores = std::make_unique<GerenciadorFornecedores>();

    // Inicializa o ponteiro único para o GerenciadorOrdens da mesma forma.
    gerenciadorOrdens = std::make_unique<GerenciadorOrdens>();

    // Inicializa o ponteiro único para a classe de Persistência (responsável por salvar/carregar arquivos).
    persistencia = std::make_unique<PersistenciaCompras>();

    // Exibe uma mensagem no console confirmando que o módulo iniciou corretamente.
    std::cout << "Modulo de Compras inicializado com sucesso!\n";
}

// Destrutor da classe ModuloCompras.
// É chamado automaticamente quando o objeto é destruído (ex: ao fechar o programa).
ModuloCompras::~ModuloCompras() {
    // Avisa o usuário que o processo de salvamento automático iniciou.
    std::cout << "Salvando dados antes de encerrar...\n";

    // Chama o método (provavelmente definido no .h ou em outra parte não mostrada aqui)
    // para salvar todos os dados atuais nos arquivos antes de limpar a memória.
    salvarTodosDados();
}

// Método responsável por orquestrar o carregamento de dados dos arquivos para a memória do sistema.
void ModuloCompras::carregarTodosDados() {
    // Cria uma lista temporária para armazenar os fornecedores que virão do arquivo.
    ListaGenerica<Fornecedor> listaFornecedores;
    // Inicializa o contador de IDs de fornecedores (começa em 1 se o arquivo estiver vazio).
    int proximoIdFornecedor = 1;

    // Cria uma lista temporária para armazenar as ordens de compra.
    ListaGenerica<OrdemCompra> listaOrdens;
    // Inicializa o contador de IDs de ordens.
    int proximoIdOrdem = 1;

    // Usa o objeto de persistência para ler o arquivo físico e preencher a lista temporária de fornecedores.
    // Também atualiza a variável 'proximoIdFornecedor' com base no maior ID encontrado no arquivo.
    persistencia->carregarFornecedores(listaFornecedores, proximoIdFornecedor);

    // Usa o objeto de persistência para ler o arquivo e preencher a lista temporária de ordens.
    // Atualiza 'proximoIdOrdem'.
    persistencia->carregarOrdens(listaOrdens, proximoIdOrdem);

    // Transfere os dados carregados na lista temporária para o gerenciador oficial de fornecedores.
    // O gerenciador passará a deter esses dados na memória durante a execução.
    gerenciadorFornecedores->carregarDeLista(listaFornecedores, proximoIdFornecedor);

    // Transfere os dados carregados para o gerenciador oficial de ordens.
    gerenciadorOrdens->carregarDeLista(listaOrdens, proximoIdOrdem);

    // Informa ao usuário que todo o processo de carga foi concluído.
    std::cout << "Dados carregados com sucesso!\n";
}