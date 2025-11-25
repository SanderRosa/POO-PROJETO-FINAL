#include "PersistenciaCompras.h"

// Construtor da classe: responsável por inicializar a instância com os caminhos dos arquivos.
PersistenciaCompras::PersistenciaCompras(const std::string& caminhoForn,
                                       const std::string& caminhoOrd)
    // Lista de inicialização: atribui os argumentos recebidos diretamente aos atributos da classe.
    : caminhoFornecedores(caminhoForn), caminhoOrdens(caminhoOrd) {}

// Método para salvar a lista de fornecedores no arquivo físico.
void PersistenciaCompras::salvarFornecedores(const ListaGenerica<Fornecedor>& lista) {
    // Define uma lista de caminhos possíveis para tentar salvar o arquivo.
    // Tenta na pasta atual, na pasta pai ("../") e na pasta avô ("../../").
    // Isso garante que o arquivo seja encontrado mesmo rodando de pastas diferentes (ex: build/ ou bin/).
    std::vector<std::string> candidatos = {
        caminhoFornecedores,
        std::string("../") + caminhoFornecedores,
        std::string("../../") + caminhoFornecedores
    };

    std::ofstream arquivo; // Objeto para escrita de arquivo.
    std::string abertoEm;  // Variável auxiliar para saber onde o arquivo foi aberto (opcional).

    // Itera sobre os candidatos tentando abrir (ou criar) o arquivo no primeiro caminho válido.
    for (const auto& c : candidatos) {
        arquivo.open(c);
        // Se conseguiu abrir, salva o caminho e para o loop.
        if (arquivo.is_open()) { abertoEm = c; break; }
    }

    // Se nenhum caminho funcionou, lança uma exceção personalizada.
    if (!arquivo.is_open()) {
        throw ComprasException("Erro ao abrir arquivo de fornecedores em caminhos candidatos!");
    }

    // Escreve o cabeçalho do arquivo CSV/Pipe-delimited com as colunas, incluindo as novas (Produto e Preço).
    arquivo << "ID|Nome|Endereco|CNPJ|Produto|Preco\n";

    // Loop para percorrer todos os itens da lista genérica.
    for (size_t i = 0; i < lista.obterTamanho(); i++) {
        // Obtém o objeto Fornecedor na posição 'i'.
        const auto& forn = lista.obter(i);

        // Escreve os dados do fornecedor separados por '|'.
        arquivo << forn.getId() << "|"
                << forn.getNome() << "|"
                << forn.getEndereco() << "|"
                << forn.getCNPJ() << "|"
                << forn.getProduto() << "|"
                // Formata o preço com 2 casas decimais (ex: 10.50) e fixa a notação de ponto flutuante.
                << std::fixed << std::setprecision(2) << forn.getPrecoProduto() << "\n";
    }

    // Fecha o arquivo após terminar a escrita.
    arquivo.close();
}

// Método para carregar os dados do arquivo para a memória (Lista).
void PersistenciaCompras::carregarFornecedores(ListaGenerica<Fornecedor>& lista, int& proximoId) {
    // Novamente, define os caminhos possíveis para tentar ler o arquivo.
    std::vector<std::string> candidatos = {
        caminhoFornecedores,
        std::string("../") + caminhoFornecedores,
        std::string("../../") + caminhoFornecedores
    };

    std::ifstream arquivo; // Objeto para leitura de arquivo.
    std::string abertoEm;

    // Tenta abrir o arquivo de leitura em um dos caminhos candidatos.
    for (const auto& c : candidatos) {
        arquivo.open(c);
        if (arquivo.is_open()) { abertoEm = c; break; }
    }

    // Se não conseguir abrir (arquivo não existe), avisa o usuário e encerra a função.
    // Não é um erro crítico, pois pode ser a primeira vez que o programa roda.
    if (!arquivo.is_open()) {
        std::cout << "Arquivo de fornecedores nao existe (sera criado na proxima gravacao).\n";
        return;
    }

    std::string linha;
    bool primeiraLinha = true; // Flag para controlar o pular do cabeçalho.

    // Lê o arquivo linha por linha até o fim.
    while (std::getline(arquivo, linha)) {
        // Se for a primeira linha (cabeçalho), ignora e continua para a próxima.
        if (primeiraLinha) {
            primeiraLinha = false;
            continue;
        }

        // Transforma a linha em um stream para facilitar a separação dos dados.
        std::istringstream iss(linha);
        // Variáveis temporárias para armazenar os dados lidos como strings.
        std::string id_str, nome, endereco, cnpj, produto, preco_str;

        // Tenta ler os 4 primeiros campos obrigatórios separados por '|'.
        if (std::getline(iss, id_str, '|') &&
            std::getline(iss, nome, '|') &&
            std::getline(iss, endereco, '|') &&
            std::getline(iss, cnpj)) {

            // Converte o ID de string para inteiro.
            int id = std::stoi(id_str);

            // Lógica de compatibilidade: Tenta ler os campos novos (produto e preço).
            if (std::getline(iss, produto, '|') && std::getline(iss, preco_str)) {
                double preco = 0.0;
                // Tenta converter o preço para double, com tratamento de erro básico.
                try { preco = std::stod(preco_str); } catch (...) { preco = 0.0; }

                // Cria o objeto Fornecedor usando o construtor completo (novo formato).
                Fornecedor forn(nome, endereco, cnpj, id, produto, preco);
                lista.adicionar(forn);
            } else {
                // Se não conseguir ler produto/preço, assume formato antigo (legado) e usa construtor antigo.
                Fornecedor forn(nome, endereco, cnpj, id);
                lista.adicionar(forn);
            }

            // Atualiza o controle de IDs para garantir que o próximo ID gerado seja maior que o atual.
            if (id >= proximoId) {
                proximoId = id + 1;
            }
        }
    }

    arquivo.close(); // Fecha o arquivo de leitura.
}

// Método para salvar as Ordens de Compra (lógica muito similar ao salvarFornecedores).
void PersistenciaCompras::salvarOrdens(const ListaGenerica<OrdemCompra>& lista) {
    // Define os caminhos candidatos.
    std::vector<std::string> candidatos = {
        caminhoOrdens,
        std::string("../") + caminhoOrdens,
        std::string("../../") + caminhoOrdens
    };

    std::ofstream arquivo;
    std::string abertoEm;
    // Tenta abrir o arquivo para escrita.
    for (const auto& c : candidatos) {
        arquivo.open(c);
        if (arquivo.is_open()) { abertoEm = c; break; }
    }
    // Lança exceção se falhar.
    if (!arquivo.is_open()) {
        throw ComprasException("Erro ao abrir arquivo de ordens em caminhos candidatos!");
    }

    // Escreve o cabeçalho das ordens.
    arquivo << "ID|IdItem|Quantidade|ValorUnitario|IdFornecedor|Status|DataSolicitacao\n";

    // Itera sobre a lista de ordens.
    for (size_t i = 0; i < lista.obterTamanho(); i++) {
        const auto& ordem = lista.obter(i);
        // Escreve os dados. Nota: Status é um Enum, convertemos para int (static_cast) para salvar.
        arquivo << ordem.getIdTransacao() << "|"
                << ordem.getIdItem() << "|"
                << ordem.getQuantidade() << "|"
                << std::fixed << std::setprecision(2) << ordem.getValorUnitario() << "|"
                << ordem.getIdFornecedor() << "|"
                << static_cast<int>(ordem.getStatus()) << "|"
                << ordem.getDataSolicitacao() << "\n";
    }

    arquivo.close();
}

// Método para carregar as Ordens de Compra.
void PersistenciaCompras::carregarOrdens(ListaGenerica<OrdemCompra>& lista, int& proximoId) {
    // Define caminhos candidatos.
    std::vector<std::string> candidatos = {
        caminhoOrdens,
        std::string("../") + caminhoOrdens,
        std::string("../../") + caminhoOrdens
    };

    std::ifstream arquivo;
    std::string abertoEm;
    // Tenta abrir arquivo para leitura.
    for (const auto& c : candidatos) {
        arquivo.open(c);
        if (arquivo.is_open()) { abertoEm = c; break; }
    }
    // Se não existir, avisa e retorna.
    if (!arquivo.is_open()) {
        std::cout << "Arquivo de ordens nao existe (sera criado na proxima gravacao).\n";
        return;
    }

    std::string linha;
    bool primeiraLinha = true;

    // Lê linha a linha.
    while (std::getline(arquivo, linha)) {
        // Pula cabeçalho.
        if (primeiraLinha) {
            primeiraLinha = false;
            continue;
        }

        std::istringstream iss(linha);
        // Strings temporárias para leitura.
        std::string id_str, idItem_str, quantidade_str, valor_str,
                   idForn_str, status_str, data;

        // Lê todos os campos sequencialmente separados por '|'.
        if (std::getline(iss, id_str, '|') &&
            std::getline(iss, idItem_str, '|') &&
            std::getline(iss, quantidade_str, '|') &&
            std::getline(iss, valor_str, '|') &&
            std::getline(iss, idForn_str, '|') &&
            std::getline(iss, status_str, '|') &&
            std::getline(iss, data)) {

            // Converte as strings para os tipos numéricos corretos.
            int id = std::stoi(id_str);
            int idItem = std::stoi(idItem_str);
            int quantidade = std::stoi(quantidade_str);
            double valor = std::stod(valor_str);
            int idForn = std::stoi(idForn_str);
            int status = std::stoi(status_str);

            // Cria o objeto OrdemCompra com os dados lidos.
            OrdemCompra ordem(id, idItem, quantidade, valor, idForn);

            // Restaura o Status convertendo de int de volta para o Enum StatusOrdem.
            ordem.setStatus(static_cast<StatusOrdem>(status));

            // Define a data de solicitação original (lida do arquivo) para não sobrescrever com a data atual.
            ordem.setDataSolicitacao(data);

            // Adiciona na lista em memória.
            lista.adicionar(ordem);

            // Atualiza o contador de IDs.
            if (id >= proximoId) {
                proximoId = id + 1;
            }
        }
    }

    arquivo.close();
}