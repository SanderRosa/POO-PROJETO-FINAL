#include "GerenciadorOrdens.h"
#include <iomanip>
#include <iostream>

// Construtor da classe GerenciadorOrdens.
// Inicializa o contador de IDs (proximoId) começando em 1.
GerenciadorOrdens::GerenciadorOrdens() : proximoId(1) {
    // Inicializa o módulo financeiro simulado (Mock) usando ponteiro inteligente (unique_ptr).
    modulo_financeiro = std::make_unique<FinanceiroMock>();
    // Inicializa o módulo de produção simulado (Mock).
    modulo_producao = std::make_unique<ProducaoMock>();
}

// Destrutor: Não precisa fazer nada manual pois os unique_ptr limpam a memória automaticamente.
GerenciadorOrdens::~GerenciadorOrdens() {}

// Método principal para criar uma nova ordem de compra.
// Recebe os dados do item, quantidade, valor e fornecedor.
int GerenciadorOrdens::criar(int idItem, int quantidade, double valorUnitario, int idFornecedor) {
    // Validação básica: não permite criar pedidos com quantidade zero ou valor negativo.
    if (quantidade <= 0 || valorUnitario < 0) {
        // Lança exceção se os dados forem inválidos.
        throw ComprasException("Quantidade e valor devem ser positivos!");
    }

    // Log para indicar o início da seção crítica.
    std::cout << "Adquirindo lock para criar ordem...\n";

    // Mecanismo de proteção de Thread (Mutex).
    // O std::lock_guard tranca o 'mutex' aqui e o destranca automaticamente quando a função terminar.
    // Isso impede que duas ordens sejam criadas exatamente ao mesmo tempo e corrompam a lista ou o ID.
    std::lock_guard<std::mutex> lock(mutex);
    std::cout << "Lock adquirido com sucesso.\n";

    // Calcula o valor total do pedido.
    double valorTotal = valorUnitario * quantidade;

    // Cria o objeto OrdemCompra na memória temporária com status inicial PENDENTE.
    OrdemCompra novaOrdem(proximoId, idItem, quantidade, valorUnitario, idFornecedor);
    // Guarda o ID atual para uso posterior.
    int idOrdemAtribuido = proximoId;

    // Exibe detalhes da tentativa de criação no console.
    std::cout << "\nCriando Ordem de Compra #" << idOrdemAtribuido << "\n";
    std::cout << "   Item ID: " << idItem << " | Quantidade: " << quantidade
              << " | Valor Total: R$ " << std::fixed << std::setprecision(2) << valorTotal << "\n\n";

    // Variável para armazenar o resultado da verificação financeira.
    bool verbaAprovada = false;

    // Criação de uma THREAD secundária.
    // A verificação de verba roda em paralelo para simular um processo externo demorado.
    // Passa a função 'threadVerificarVerba', o ponteiro 'this' (para acessar métodos da classe),
    // o valor a verificar e o endereço da variável onde o resultado será guardado.
    std::thread threadFinanceiro(&GerenciadorOrdens::threadVerificarVerba, this,
                                 valorTotal, &verbaAprovada);

    // .join() força a thread principal a esperar a threadFinanceiro terminar antes de continuar.
    // Isso sincroniza as threads.
    threadFinanceiro.join();

    std::cout << "\n";

    // Verifica se a thread financeira retornou falso (verba negada).
    if (!verbaAprovada) {
        // Atualiza o status da ordem para REJEITADO.
        novaOrdem.setStatus(StatusOrdem::REJEITADO);
        // Salva na lista mesmo rejeitada, para histórico.
        ordens.adicionar(novaOrdem);
        // Incrementa o ID para a próxima ordem.
        proximoId++;

        // Log de rejeição.
        std::cout << "Ordem #" << idOrdemAtribuido << " REJEITADA - Verba insuficiente.\n";
        std::cout << "Lock liberado.\n\n";

        // Retorna -1 indicando falha na criação.
        return -1;
    }

    // Se passou pela verba, chama o módulo financeiro para autorizar o pagamento efetivo.
    std::cout << "\n------ FINANCEIRO ------\n";
    bool pagamentoAutorizado = modulo_financeiro->autorizarPagamento(idOrdemAtribuido);
    std::cout << "------------------------\n\n";

    // Se o pagamento não foi autorizado (ex: cartão recusado, erro no banco).
    if (!pagamentoAutorizado) {
        // Marca como REJEITADO.
        novaOrdem.setStatus(StatusOrdem::REJEITADO);
        // Salva histórico.
        ordens.adicionar(novaOrdem);
        proximoId++;

        std::cout << "Ordem #" << idOrdemAtribuido << " REJEITADA - Falha na autorizacao.\n";
        std::cout << "Lock liberado.\n\n";

        return -1;
    }

    // Se tudo deu certo no financeiro, notifica a produção que o material vai chegar.
    std::cout << "------ PRODUCAO --------\n";
    modulo_producao->notificarMaterialComprado(idItem);
    std::cout << "------------------------\n\n";

    // Finalmente, marca a ordem como APROVADA.
    novaOrdem.setStatus(StatusOrdem::APROVADO);
    // Adiciona à lista oficial.
    ordens.adicionar(novaOrdem);
    // Prepara o ID para a próxima chamada.
    proximoId++;

    // Exibe sucesso.
    std::cout << "Ordem #" << idOrdemAtribuido << " APROVADA COM SUCESSO!\n";
    std::cout << "   Status: APROVADO\n";
    std::cout << "   Valor Total: R$ " << std::fixed << std::setprecision(2) << valorTotal << "\n";
    std::cout << "Lock liberado.\n\n";

    // Retorna o ID da ordem criada com sucesso.
    return idOrdemAtribuido;
}

// Função executada pela thread secundária para verificar verba.
void GerenciadorOrdens::threadVerificarVerba(double valor, bool* resultado) {
    std::cout << "\n------ FINANCEIRO ------\n";
    // Exibe o ID da thread atual para demonstrar que é diferente da thread principal.
    std::cout << "Thread de Financeiro iniciada (ID: " << std::this_thread::get_id() << ")\n";

    // Chama a lógica do mock financeiro e salva o retorno no ponteiro 'resultado'.
    *resultado = modulo_financeiro->verificarDisponibilidade(valor);

    std::cout << "Thread de Financeiro finalizada\n";
    std::cout << "------------------------\n";
}

// Método para listar todas as ordens cadastradas.
void GerenciadorOrdens::listar() const {
    // Protege a leitura da lista com mutex para evitar leitura enquanto outra thread escreve.
    std::lock_guard<std::mutex> lock(mutex);

    // Verifica se a lista está vazia.
    if (ordens.estaVazia()) {
        std::cout << "Nenhuma ordem de compra cadastrada.\n";
        return;
    }

    std::cout << "\nLISTA DE ORDENS DE COMPRA\n";
    std::cout << "========================\n";

    // Itera sobre a lista e exibe os detalhes de cada ordem.
    for (size_t i = 0; i < ordens.obterTamanho(); i++) {
        std::cout << "\n" << ordens.obter(i).exibirDetalhes() << "\n";
    }
    std::cout << "\n";
}

// Busca uma ordem específica pelo ID.
OrdemCompra* GerenciadorOrdens::buscarPorId(int id) {
    // Protege o acesso à lista.
    std::lock_guard<std::mutex> lock(mutex);

    // Percorre a lista linearmente.
    for (size_t i = 0; i < ordens.obterTamanho(); i++) {
        // Se encontrar o ID correspondente...
        if (ordens.obter(i).getIdTransacao() == id) {
            // Retorna o endereço do objeto na lista (permitindo modificação se necessário).
            return &ordens.obterMutavel(i);
        }
    }
    // Retorna nulo se não encontrar.
    return nullptr;
}

// Retorna o total de ordens cadastradas.
size_t GerenciadorOrdens::obterQuantidade() const {
    // Protege a leitura do tamanho.
    std::lock_guard<std::mutex> lock(mutex);
    return ordens.obterTamanho();
}

// Calcula e exibe estatísticas gerais das compras.
void GerenciadorOrdens::exibirEstatisticas() const {
    // Protege o acesso para garantir consistência dos dados durante o cálculo.
    std::lock_guard<std::mutex> lock(mutex);

    std::cout << "\nESTATISTICAS DO MODULO DE COMPRAS\n";
    std::cout << "==================================\n\n";

    // Variáveis acumuladoras.
    int aprovadas = 0, rejeitadas = 0, pendentes = 0;
    double valorTotalAprovado = 0.0;

    // Itera sobre todas as ordens para contabilizar.
    for (size_t i = 0; i < ordens.obterTamanho(); i++) {
        const auto& ordem = ordens.obter(i);
        // Verifica o status de cada ordem.
        switch (ordem.getStatus()) {
            case StatusOrdem::APROVADO:
                aprovadas++;
                // Soma o valor apenas das aprovadas.
                valorTotalAprovado += ordem.getValorTotal();
                break;
            case StatusOrdem::REJEITADO:
                rejeitadas++;
                break;
            case StatusOrdem::PENDENTE:
                pendentes++;
                break;
            default:
                break;
        }
    }

    // Exibe os resultados calculados.
    std::cout << "Detalhes das Ordens:\n";
    std::cout << "  Aprovadas: " << aprovadas << "\n";
    std::cout << "  Rejeitadas: " << rejeitadas << "\n";
    std::cout << "  Pendentes: " << pendentes << "\n";
    std::cout << "  Valor Total Aprovado: R$ " << std::fixed
              << std::setprecision(2) << valorTotalAprovado << "\n\n";
}

// Retorna a lista completa (somente leitura).
// Nota: Retorna uma referência constante, mas cuidado deve ser tomado se a lista for alterada externamente.
const ListaGenerica<OrdemCompra>& GerenciadorOrdens::obterLista() const {
    return ordens;
}

// Método usado para recarregar dados vindos do arquivo (Persistência).
void GerenciadorOrdens::carregarDeLista(const ListaGenerica<OrdemCompra>& lista,
                                        int proximoIdArmazenado) {
    // Bloqueia o acesso durante a substituição completa dos dados.
    std::lock_guard<std::mutex> lock(mutex);

    // Substitui a lista atual pela lista carregada do arquivo.
    ordens = lista;
    // Restaura o contador de IDs para continuar de onde parou.
    proximoId = proximoIdArmazenado;
}