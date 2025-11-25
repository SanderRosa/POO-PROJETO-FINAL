#include "GerenciadorFornecedores.h"
#include <iostream>

// Construtor da classe GerenciadorFornecedores.
// Inicializa o contador de IDs (proximoId) com 1.
GerenciadorFornecedores::GerenciadorFornecedores() : proximoId(1) {}

// Destrutor da classe.
// Não há alocação dinâmica manual aqui que exija limpeza explícita, então está vazio.
GerenciadorFornecedores::~GerenciadorFornecedores() {}

// Método para adicionar um novo fornecedor ao sistema.
// Recebe nome, endereço, CNPJ, produto e preço.
int GerenciadorFornecedores::adicionar(const std::string& nome,
                                       const std::string& endereco,
                                       const std::string& cnpj,
                                       const std::string& produto,
                                       double precoProduto) {
    // Validação dos dados de entrada: verifica se campos obrigatórios estão vazios.
    if (nome.empty() || cnpj.empty() || produto.empty()) {
        // Lança uma exceção personalizada se a validação falhar.
        throw ComprasException("Nome, CNPJ e Produto nao podem estar vazios!");
    }

    // Adquire o lock do mutex para garantir segurança em ambiente multithread.
    // Impede que dois fornecedores sejam adicionados simultaneamente, o que corromperia a lista.
    std::lock_guard<std::mutex> lock(mutex);

    // Cria o objeto Fornecedor com os dados fornecidos e o ID atual.
    Fornecedor novoFornecedor(nome, endereco, cnpj, proximoId, produto, precoProduto);
    // Adiciona o objeto à lista genérica de fornecedores.
    fornecedores.adicionar(novoFornecedor);

    // Armazena o ID que acabou de ser usado para retorná-lo.
    int idAtribuido = proximoId;
    // Incrementa o contador para o próximo cadastro.
    proximoId++;

    // Exibe confirmação no console.
    std::cout << "Fornecedor #" << idAtribuido << " adicionado com sucesso!\n";
    // Retorna o ID gerado.
    return idAtribuido;
}

// Método para listar apenas fornecedores que vendem um determinado produto.
void GerenciadorFornecedores::listarPorProduto(const std::string& produto) const {
    // Protege a leitura da lista com mutex.
    std::lock_guard<std::mutex> lock(mutex);
    bool encontrou = false; // Flag para saber se achamos pelo menos um.

    // Percorre toda a lista de fornecedores.
    for (size_t i = 0; i < fornecedores.obterTamanho(); i++) {
        // Compara o produto do fornecedor atual com o produto buscado.
        if (fornecedores.obter(i).getProduto() == produto) {
            // Se bater, exibe os detalhes.
            std::cout << fornecedores.obter(i).exibirDetalhes() << "\n";
            encontrou = true;
        }
    }
    // Se percorreu tudo e não achou nada, avisa o usuário.
    if (!encontrou) {
        std::cout << "Nenhum fornecedor cadastrado para o produto: " << produto << "\n";
    }
}

// Método para listar fornecedores ordenados do mais caro para o mais barato (decrescente).
void GerenciadorFornecedores::listarOrdenadoPorPreco() const {
    // Protege a operação com mutex.
    std::lock_guard<std::mutex> lock(mutex);

    // Cria um vetor temporário (std::vector) para fazer a ordenação.
    // Isso é necessário porque a ListaGenerica pode não ter método de ordenação nativo exposto.
    std::vector<Fornecedor> copia;

    // Copia todos os elementos da lista original para o vetor temporário.
    for (size_t i = 0; i < fornecedores.obterTamanho(); i++) {
        copia.push_back(fornecedores.obter(i));
    }

    // Usa std::sort da biblioteca padrão para ordenar o vetor.
    // Usa uma função lambda [](...) para definir o critério de comparação (preço maior vem primeiro).
    std::sort(copia.begin(), copia.end(), [](const Fornecedor& a, const Fornecedor& b) {
        return a.getPrecoProduto() > b.getPrecoProduto();
    });

    // Exibe os fornecedores já na ordem correta.
    for (const auto& f : copia) {
        std::cout << f.exibirDetalhes() << "\n";
    }
}

// Método padrão para listar todos os fornecedores na ordem de cadastro.
void GerenciadorFornecedores::listar() const {
    // Protege o acesso à lista.
    std::lock_guard<std::mutex> lock(mutex);

    // Verifica se a lista está vazia para dar feedback rápido.
    if (fornecedores.estaVazia()) {
        std::cout << "Nenhum fornecedor cadastrado.\n";
        return;
    }

    std::cout << "\nLISTA DE FORNECEDORES CADASTRADOS\n";
    std::cout << "==================================\n";

    // Itera e exibe cada fornecedor.
    for (size_t i = 0; i < fornecedores.obterTamanho(); i++) {
        std::cout << "\n" << fornecedores.obter(i).exibirDetalhes() << "\n";
    }
    std::cout << "\n";
}

// Busca um fornecedor pelo ID e retorna um ponteiro para ele.
Fornecedor* GerenciadorFornecedores::buscarPorId(int id) {
    // Protege o acesso à lista.
    std::lock_guard<std::mutex> lock(mutex);

    // Itera sobre a lista procurando o ID.
    for (size_t i = 0; i < fornecedores.obterTamanho(); i++) {
        if (fornecedores.obter(i).getId() == id) {
            // Retorna o endereço do objeto na lista, permitindo modificação (obterMutavel).
            return &fornecedores.obterMutavel(i);
        }
    }
    // Retorna nullptr se não encontrar.
    return nullptr;
}

// Remove um fornecedor da lista com base no ID.
void GerenciadorFornecedores::remover(int id) {
    // Protege a operação de escrita na lista.
    std::lock_guard<std::mutex> lock(mutex);

    // Procura o fornecedor na lista.
    for (size_t i = 0; i < fornecedores.obterTamanho(); i++) {
        if (fornecedores.obter(i).getId() == id) {
            // Se encontrar, remove o item na posição 'i'.
            fornecedores.remover(i);
            std::cout << "Fornecedor #" << id << " removido com sucesso!\n";
            return;
        }
    }

    // Se terminar o loop sem encontrar, lança erro.
    throw ComprasException("Fornecedor #" + std::to_string(id) + " nao encontrado!");
}

// Retorna a quantidade total de fornecedores cadastrados.
size_t GerenciadorFornecedores::obterQuantidade() const {
    // Protege a leitura.
    std::lock_guard<std::mutex> lock(mutex);
    return fornecedores.obterTamanho();
}

// Retorna uma referência constante para a lista completa.
const ListaGenerica<Fornecedor>& GerenciadorFornecedores::obterLista() const {
    return fornecedores;
}

// Método usado para carregar dados do disco (Persistência).
// Substitui a lista atual pela lista carregada e atualiza o contador de IDs.
void GerenciadorFornecedores::carregarDeLista(const ListaGenerica<Fornecedor>& lista,
                                              int proximoIdArmazenado) {
    // Protege a escrita total da lista.
    std::lock_guard<std::mutex> lock(mutex);

    // Substituição direta da lista.
    fornecedores = lista;
    // Atualiza o ID para continuar a contagem corretamente.
    proximoId = proximoIdArmazenado;
}