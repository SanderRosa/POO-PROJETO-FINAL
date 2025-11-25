#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include "ModuloCompras.h"

// Funções auxiliares para a interface do console

// Função simples para limpar o console.
void limparTela() {
    // Comando do sistema Windows para limpar a tela. Em Linux/Mac seria "clear".
    system("cls");
}

// Função para exibir o título do sistema no topo da tela.
void exibirCabecalho() {
    std::cout << "\nMODULO DE COMPRAS - ERP\n";
}

// Exibe as opções numéricas do menu principal.
void exibirMenu() {
    std::cout << "\nMENU PRINCIPAL\n";
    std::cout << "1. Cadastrar Fornecedor\n";
    std::cout << "2. Listar Fornecedores\n";
    std::cout << "3. Criar Ordem de Compra\n";
    std::cout << "4. Listar Ordens de Compra\n";
    std::cout << "5. Exibir Estatisticas\n";
    std::cout << "6. Investigar Fornecedor na Web\n";
    std::cout << "7. Salvar Dados em Arquivo\n";
    std::cout << "8. Carregar Dados do Arquivo\n";
    std::cout << "9. Sair\n";
    std::cout << "\n";
}

// Função auxiliar robusta para ler números inteiros e evitar quebras se o usuário digitar letras.
int obterInteiro() {
    int valor;
    // Loop que continua enquanto a leitura falhar (ex: usuário digita "abc" em vez de "123").
    while (!(std::cin >> valor)) {
        // Limpa o estado de erro do fluxo de entrada (cin).
        std::cin.clear();
        // Ignora o restante da linha inválida no buffer até a quebra de linha.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada invalida! Digite um numero inteiro: ";
    }
    // Ignora qualquer caractere extra que tenha sobrado no buffer (como o 'Enter').
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return valor;
}

// Função auxiliar robusta para ler números decimais (double).
double obterDouble() {
    double valor;
    // Mesma lógica do obterInteiro: tenta ler, se falhar, limpa o erro e pede de novo.
    while (!(std::cin >> valor)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada invalida! Digite um numero: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return valor;
}

// Função para ler uma linha inteira de texto, incluindo espaços.
std::string obterString() {
    std::string texto;
    // std::getline lê até encontrar um 'Enter', permitindo nomes compostos.
    std::getline(std::cin, texto);
    return texto;
}

// ========== OPERAÇÕES DO MENU ==========

// Função que gerencia a interação para cadastrar um fornecedor.
void menuCadastrarFornecedor(ModuloCompras& modulo) {
    std::cout << "\nCADASTRO DE NOVO FORNECEDOR\n";

    // Coleta os dados passo a passo usando as funções auxiliares.
    std::cout << "Nome do Fornecedor: ";
    std::string nome = obterString();

    std::cout << "Endereco: ";
    std::string endereco = obterString();

    std::cout << "CNPJ: ";
    std::string cnpj = obterString();

    std::cout << "Produto fornecido: ";
    std::string produto = obterString();

    std::cout << "Preço do produto (R$): ";
    double precoProduto = obterDouble();

    // Tenta realizar o cadastro no módulo de lógica.
    try {
        // Chama o método do backend e recebe o ID gerado.
        int id = modulo.adicionarFornecedor(nome, endereco, cnpj, produto, precoProduto);
        std::cout << "Fornecedor cadastrado com ID: " << id << "\n";
    } catch (const ComprasException& e) {
        // Se houver erro (ex: CNPJ inválido), exibe a mensagem da exceção.
        std::cout << "Erro ao cadastrar: " << e.what() << "\n";
    }

    // Pausa para o usuário ler a mensagem antes de voltar ao menu.
    std::cout << "Pressione ENTER para continuar...";
    std::cin.get();
}

// Função wrapper para chamar a listagem de fornecedores.
void menuListarFornecedores(ModuloCompras& modulo) {
    modulo.listarFornecedores();
    std::cout << "Pressione ENTER para continuar...";
    std::cin.get();
}

// Função que gerencia a criação de uma ordem de compra.
void menuCriarOrdenCompra(ModuloCompras& modulo) {
    std::cout << "\nCRIAR NOVA ORDEM DE COMPRA\n";

    std::cout << "ID do Fornecedor: ";
    int idFornecedor = obterInteiro();

    // Verifica se o fornecedor existe antes de prosseguir.
    if (modulo.buscarFornecedorPorId(idFornecedor) == nullptr) {
        std::cout << "Fornecedor nao encontrado!\n";
        std::cout << "Pressione ENTER para continuar...";
        std::cin.get();
        return; // Sai da função se o ID for inválido.
    }

    std::cout << "ID do Item/Material: ";
    int idItem = obterInteiro();

    std::cout << "Quantidade: ";
    int quantidade = obterInteiro();

    std::cout << "Valor Unitario (R$): ";
    double valorUnitario = obterDouble();

    try {
        std::cout << "\n";
        // Tenta criar a ordem. O retorno é o ID da ordem ou -1 se falhar.
        int idOrdem = modulo.criarOrdemCompra(idItem, quantidade, valorUnitario, idFornecedor);

        // Verifica se a ordem foi aprovada (ID válido) ou rejeitada (-1).
        if (idOrdem != -1) {
            std::cout << "Ordem de Compra " << idOrdem << " criada e APROVADA!\n";
        } else {
            std::cout << "Ordem foi criada como REJEITADA.\n";
        }
    } catch (const ComprasException& e) {
        std::cout << "Erro ao criar ordem: " << e.what() << "\n";
    }

    std::cout << "Pressione ENTER para continuar...";
    std::cin.get();
}

// Função wrapper para listar as ordens.
void menuListarOrdens(ModuloCompras& modulo) {
    modulo.listarOrdens();
    std::cout << "Pressione ENTER para continuar...";
    std::cin.get();
}

// Função wrapper para exibir estatísticas.
void menuExibirEstatisticas(ModuloCompras& modulo) {
    modulo.exibirEstatisticas();
    std::cout << "Pressione ENTER para continuar...";
    std::cin.get();
}

// Simulação de uma funcionalidade de integração web.
void menuInvestigarFornecedor(ModuloCompras& modulo) {
    std::cout << "\nINVESTIGAR FORNECEDOR NA WEB\n";

    std::cout << "ID do Fornecedor: ";
    int idFornecedor = obterInteiro();

    // Busca o ponteiro do fornecedor para pegar o nome e CNPJ.
    Fornecedor* forn = modulo.buscarFornecedorPorId(idFornecedor);
    if (forn == nullptr) {
        std::cout << "Fornecedor nao encontrado!\n";
        std::cout << "Pressione ENTER para continuar...";
        std::cin.get();
        return;
    }

    // STUB: Código provisório que simula o comportamento real.
    std::cout << "\nAbrindo investigacao na web...\n";
    std::cout << "   Procurando por: \"" << forn->getNome() << " " << forn->getCNPJ() << "\"\n\n";

    // Cria uma URL de busca no Google concatenando os dados do fornecedor.
    std::string url = "https://www.google.com/search?q=" + forn->getNome() +
                      "+CNPJ+" + forn->getCNPJ();

    std::cout << "URL gerada:\n   " << url << "\n\n";
    std::cout << "(STUB) Em um sistema real, isso abriria o navegador.\n";

    std::cout << "\nPressione ENTER para continuar...";
    std::cin.get();
}

// Chama a função de persistência para salvar tudo manualmente.
void menuSalvarDados(ModuloCompras& modulo) {
    std::cout << "\nSalvando dados...\n";
    try {
        modulo.salvarTodosDados();
    } catch (const ComprasException& e) {
        std::cout << "Erro ao salvar: " << e.what() << "\n";
    }
    std::cout << "\nPressione ENTER para continuar...";
    std::cin.get();
}

// Chama a função de persistência para recarregar os dados manualmente.
void menuCarregarDados(ModuloCompras& modulo) {
    std::cout << "\nCarregando dados...\n";
    try {
        modulo.carregarTodosDados();
    } catch (const ComprasException& e) {
        std::cout << "Erro ao carregar: " << e.what() << "\n";
    }
    std::cout << "\nPressione ENTER para continuar...";
    std::cin.get();
}

// ========== FUNÇÃO PRINCIPAL (Ponto de entrada) ==========

int main() {
    try {
        // Inicializa o módulo principal (backend) do sistema.
        ModuloCompras modulo;

        limparTela();
        exibirCabecalho();

        // Tenta carregar os dados automaticamente ao iniciar o programa.
        std::cout << "Carregando dados iniciais...\n";
        modulo.carregarTodosDados();

        // Limpa a tela para mostrar o menu limpo.
        limparTela();
        exibirCabecalho();

        int opcao = 0;
        bool rodando = true; // Flag de controle do loop principal.

        // Loop principal do menu: mantém o programa rodando até o usuário escolher Sair.
        while (rodando) {
            exibirCabecalho();
            exibirMenu();

            std::cout << "Escolha uma opcao: ";
            opcao = obterInteiro(); // Lê a opção de forma segura.

            std::cout << "\n";

            // Switch para direcionar para a função correta baseada na escolha.
            switch (opcao) {
                case 1:
                    menuCadastrarFornecedor(modulo);
                    limparTela(); // Limpa a tela após cada operação para manter a interface organizada.
                    break;

                case 2:
                    menuListarFornecedores(modulo);
                    limparTela();
                    break;

                case 3:
                    menuCriarOrdenCompra(modulo);
                    limparTela();
                    break;

                case 4:
                    menuListarOrdens(modulo);
                    limparTela();
                    break;

                case 5:
                    menuExibirEstatisticas(modulo);
                    limparTela();
                    break;

                case 6:
                    menuInvestigarFornecedor(modulo);
                    limparTela();
                    break;

                case 7:
                    menuSalvarDados(modulo);
                    limparTela();
                    break;

                case 8:
                    menuCarregarDados(modulo);
                    limparTela();
                    break;

                case 9:
                    // Opção de sair. Salva os dados automaticamente antes de fechar.
                    std::cout << "Salvando dados antes de encerrar...\n";
                    modulo.salvarTodosDados();
                    std::cout << "Ate logo!\n";
                    rodando = false; // Quebra o loop while.
                    break;

                default:
                    // Tratamento para números fora do intervalo 1-9.
                    std::cout << "Opcao invalida! Digite um numero entre 1 e 9.\n";
                    std::cout << "Pressione ENTER para continuar...";
                    std::cin.get();
                    limparTela();
                    break;
            }
        }

        return 0; // Retorna 0 indicando sucesso.

    } catch (const std::exception& e) {
        // Captura erros críticos não tratados (ex: falha de memória) e exibe antes de fechar.
        std::cout << "\nERRO CRITICO: " << e.what() << "\n";
        return 1; // Retorna 1 indicando erro.
    }
}