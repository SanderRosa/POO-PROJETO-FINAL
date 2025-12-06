#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "ModuloCompras.h"

#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32")
#else
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <cstring>
#endif

#ifdef _WIN32
using socket_t = SOCKET;
inline bool initSockets() { WSADATA wsa; return WSAStartup(MAKEWORD(2, 2), &wsa) == 0; }
inline void closeSocket(socket_t s) { closesocket(s); }
using socklen_arg = int;
#else
using socket_t = int;
inline bool initSockets() { return true; }
inline void closeSocket(socket_t s) { close(s); }
using socklen_arg = socklen_t;
#endif

#ifndef SERVIDOR_STANDALONE
    #ifdef _WIN32
        #define SERVIDOR_STANDALONE 0
    #else
        #define SERVIDOR_STANDALONE 1
    #endif
#endif

namespace {
struct ProducaoRegistro {
    int id{};
    int idMaterial{};
    int quantidade{};
    int prioridade{};
    std::string status;
    int idOrdemCompra{};
    std::string dataCriacao;
    std::string dataPrevistaEntrega;
};

struct EstoquePrevisto {
    int idMaterial{};
    int quantidade{};
    int idOrdemCompra{};
    std::string dataPrevista;
};

const std::string ARQ_FORNECEDORES = "data/fornecedores.txt";
const std::string ARQ_ORDENS = "data/ordens.txt";
const std::string ARQ_PRODUCAO = "data/producao.txt";
const std::string ARQ_ESTOQUE_PREV = "data/estoque_previsto.txt";

ModuloCompras g_modulo;
std::vector<ProducaoRegistro> g_producao;
std::vector<EstoquePrevisto> g_previsto;
int g_producaoNextId = 1;
std::mutex g_mutex;

std::string jsonEscape(const std::string& in) {
    std::string out;
    out.reserve(in.size());
    for (char c : in) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default: out += c; break;
        }
    }
    return out;
}

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> out;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) out.push_back(item);
    return out;
}

std::map<std::string, std::string> parseQuery(const std::string& query) {
    std::map<std::string, std::string> params;
    std::stringstream ss(query);
    std::string kv;
    while (std::getline(ss, kv, '&')) {
        auto pos = kv.find('=');
        if (pos != std::string::npos) {
            std::string k = kv.substr(0, pos);
            std::string v = kv.substr(pos + 1);
            std::replace(v.begin(), v.end(), '+', ' ');
            params[k] = v;
        }
    }
    return params;
}

std::string nowString() {
    auto now = std::chrono::system_clock::now();
    auto t = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&t), "%d/%m/%Y %H:%M:%S");
    return oss.str();
}

void carregarProducao() {
    g_producao.clear();
    g_producaoNextId = 1;
    std::ifstream f(ARQ_PRODUCAO);
    if (!f.is_open()) return;
    std::string line;
    std::getline(f, line);
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        auto p = split(line, '|');
        if (p.size() < 8) continue;
        ProducaoRegistro r;
        r.id = std::stoi(p[0]);
        r.idMaterial = std::stoi(p[1]);
        r.quantidade = std::stoi(p[2]);
        r.prioridade = std::stoi(p[3]);
        r.status = p[4];
        r.idOrdemCompra = std::stoi(p[5]);
        r.dataCriacao = p[6];
        r.dataPrevistaEntrega = p[7];
        g_producao.push_back(r);
        g_producaoNextId = std::max(g_producaoNextId, r.id + 1);
    }
}

void salvarProducao() {
    std::ofstream f(ARQ_PRODUCAO);
    f << "ID|IdMaterial|Quantidade|Prioridade|Status|IdOrdemCompra|DataCriacao|DataPrevistaEntrega\n";
    for (const auto& r : g_producao) {
        f << r.id << "|" << r.idMaterial << "|" << r.quantidade << "|" << r.prioridade << "|"
          << r.status << "|" << r.idOrdemCompra << "|" << r.dataCriacao << "|" << r.dataPrevistaEntrega << "\n";
    }
}

void carregarPrevisto() {
    g_previsto.clear();
    std::ifstream f(ARQ_ESTOQUE_PREV);
    if (!f.is_open()) return;
    std::string line;
    std::getline(f, line);
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        auto p = split(line, '|');
        if (p.size() < 4) continue;
        EstoquePrevisto e;
        e.idMaterial = std::stoi(p[0]);
        e.quantidade = std::stoi(p[1]);
        e.idOrdemCompra = std::stoi(p[2]);
        e.dataPrevista = p[3];
        g_previsto.push_back(e);
    }
}

void salvarPrevisto() {
    std::ofstream f(ARQ_ESTOQUE_PREV);
    f << "IdMaterial|Quantidade|IdOrdemCompra|DataPrevista\n";
    for (const auto& e : g_previsto) {
        f << e.idMaterial << "|" << e.quantidade << "|" << e.idOrdemCompra << "|" << e.dataPrevista << "\n";
    }
}

std::string jsonFornecedores(const ListaGenerica<Fornecedor>& lista) {
    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < lista.obterTamanho(); ++i) {
        const auto& f = lista.obter(i);
        os << "{";
        os << "\"id\":" << f.getId() << ",";
        os << "\"nome\":\"" << jsonEscape(f.getNome()) << "\",";
        os << "\"produto\":\"" << jsonEscape(f.getProduto()) << "\",";
        os << "\"preco\":" << f.getPrecoProduto() << ",";
        os << "\"cnpj\":\"" << jsonEscape(f.getCNPJ()) << "\",";
        os << "\"endereco\":\"" << jsonEscape(f.getEndereco()) << "\"";
        os << "}";
        if (i + 1 < lista.obterTamanho()) os << ",";
    }
    os << "]";
    return os.str();
}

std::string jsonOrdens(const ListaGenerica<OrdemCompra>& lista) {
    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < lista.obterTamanho(); ++i) {
        const auto& o = lista.obter(i);
        os << "{";
        os << "\"id\":" << o.getIdTransacao() << ",";
        os << "\"idItem\":" << o.getIdItem() << ",";
        os << "\"quantidade\":" << o.getQuantidade() << ",";
        os << "\"valor\":" << o.getValorUnitario() << ",";
        os << "\"status\":" << static_cast<int>(o.getStatus()) << ",";
        os << "\"descricao\":\"" << jsonEscape(o.getDataSolicitacao()) << "\",";
        os << "\"data_chegada\":\"" << jsonEscape(o.getDataChegadaPrevista()) << "\"";
        os << "}";
        if (i + 1 < lista.obterTamanho()) os << ",";
    }
    os << "]";
    return os.str();
}

std::string jsonEstoqueAtual(const ListaGenerica<OrdemCompra>& lista) {
    std::map<int, int> soma;
    for (size_t i = 0; i < lista.obterTamanho(); ++i) {
        const auto& o = lista.obter(i);
        if (o.getStatus() != StatusOrdem::REJEITADO) {
            soma[o.getIdItem()] += o.getQuantidade();
        }
    }
    std::ostringstream os;
    os << "[";
    size_t idx = 0;
    for (const auto& kv : soma) {
        os << "{";
        os << "\"id\":" << kv.first << ",";
        os << "\"nome\":\"Item " << kv.first << "\",";
        os << "\"quantidade\":" << kv.second << "}";
        if (++idx < soma.size()) os << ",";
    }
    os << "]";
    return os.str();
}

std::string jsonPrevisto() {
    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < g_previsto.size(); ++i) {
        const auto& e = g_previsto[i];
        os << "{";
        os << "\"idMaterial\":" << e.idMaterial << ",";
        os << "\"quantidade\":" << e.quantidade << ",";
        os << "\"idOrdemCompra\":" << e.idOrdemCompra << ",";
        os << "\"dataPrevista\":\"" << jsonEscape(e.dataPrevista) << "\"";
        os << "}";
        if (i + 1 < g_previsto.size()) os << ",";
    }
    os << "]";
    return os.str();
}

std::string jsonProducao() {
    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < g_producao.size(); ++i) {
        const auto& p = g_producao[i];
        os << "{";
        os << "\"id\":" << p.id << ",";
        os << "\"idMaterial\":" << p.idMaterial << ",";
        os << "\"quantidade\":" << p.quantidade << ",";
        os << "\"prioridade\":" << p.prioridade << ",";
        os << "\"status\":\"" << jsonEscape(p.status) << "\",";
        os << "\"idOrdemCompra\":" << p.idOrdemCompra << ",";
        os << "\"dataCriacao\":\"" << jsonEscape(p.dataCriacao) << "\",";
        os << "\"dataPrevistaEntrega\":\"" << jsonEscape(p.dataPrevistaEntrega) << "\"";
        os << "}";
        if (i + 1 < g_producao.size()) os << ",";
    }
    os << "]";
    return os.str();
}

std::string httpResponse(const std::string& body, int code = 200, const std::string& contentType = "application/json") {
    std::ostringstream os;
    os << "HTTP/1.1 " << code << " OK\r\n";
    os << "Content-Type: " << contentType << "; charset=utf-8\r\n";
    os << "Access-Control-Allow-Origin: *\r\n";
    os << "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n";
    os << "Access-Control-Allow-Headers: Content-Type\r\n";
    os << "Content-Length: " << body.size() << "\r\n\r\n";
    os << body;
    return os.str();
}

std::string notFound() { return httpResponse("{\"error\":\"not found\"}", 404); }

std::string statusOk() { return httpResponse("{\"status\":\"online\",\"message\":\"Backend C++ ativo\"}"); }

std::string handleGet(const std::string& path, const std::map<std::string, std::string>& params) {
    std::lock_guard<std::mutex> lock(g_mutex);
    if (path == "/api/status") return statusOk();
    if (path == "/api/fornecedores") return httpResponse(jsonFornecedores(g_modulo.obterListaFornecedores()));
    if (path == "/api/fornecedores/produto") {
        const auto& lista = g_modulo.obterListaFornecedores();
        std::ostringstream os; os << "["; bool first = true;
        std::string prod = params.count("produto") ? params.at("produto") : "";
        for (size_t i = 0; i < lista.obterTamanho(); ++i) {
            const auto& f = lista.obter(i);
            if (!prod.empty() && f.getProduto() != prod) continue;
            if (!first) os << ","; else first = false;
            os << "{";
            os << "\"id\":" << f.getId() << ",";
            os << "\"nome\":\"" << jsonEscape(f.getNome()) << "\",";
            os << "\"produto\":\"" << jsonEscape(f.getProduto()) << "\",";
            os << "\"preco\":" << f.getPrecoProduto() << "}";
        }
        os << "]";
        return httpResponse(os.str());
    }
    if (path == "/api/fornecedores/ordenado_preco") {
        const auto& lista = g_modulo.obterListaFornecedores();
        std::vector<Fornecedor> tmp;
        for (size_t i = 0; i < lista.obterTamanho(); ++i) tmp.push_back(lista.obter(i));
        std::sort(tmp.begin(), tmp.end(), [](const Fornecedor& a, const Fornecedor& b){return a.getPrecoProduto() > b.getPrecoProduto();});
        std::ostringstream os; os << "[";
        for (size_t i = 0; i < tmp.size(); ++i) {
            const auto& f = tmp[i];
            os << "{" << "\"id\":" << f.getId() << "," << "\"nome\":\"" << jsonEscape(f.getNome()) << "\"," << "\"preco\":" << f.getPrecoProduto() << "}";
            if (i + 1 < tmp.size()) os << ",";
        }
        os << "]";
        return httpResponse(os.str());
    }
    if (path == "/api/ordens") return httpResponse(jsonOrdens(g_modulo.obterListaOrdens()));
    if (path == "/api/ordens/buscar") {
        int id = params.count("id") ? std::stoi(params.at("id")) : -1;
        OrdemCompra* o = g_modulo.buscarOrdenPorId(id);
        if (!o) return httpResponse("{\"encontrado\":false}");
        std::ostringstream os;
        os << "{\"encontrado\":true,\"id\":" << o->getIdTransacao()
           << ",\"idItem\":" << o->getIdItem() << ",\"quantidade\":" << o->getQuantidade()
           << ",\"valor\":" << o->getValorUnitario() << ",\"status\":" << static_cast<int>(o->getStatus())
           << ",\"data_chegada\":\"" << jsonEscape(o->getDataChegadaPrevista()) << "\"}";
        return httpResponse(os.str());
    }
    if (path == "/api/estatisticas") {
        const auto& lista = g_modulo.obterListaOrdens();
        int aprov = 0, reje = 0, pend = 0; double total = 0;
        for (size_t i = 0; i < lista.obterTamanho(); ++i) {
            const auto& o = lista.obter(i);
            switch (o.getStatus()) {
                case StatusOrdem::APROVADO: aprov++; total += o.getValorTotal(); break;
                case StatusOrdem::REJEITADO: reje++; break;
                default: pend++; break;
            }
        }
        std::ostringstream os;
        os << "{\"aprovadas\":" << aprov << ",\"rejeitadas\":" << reje << ",\"pendentes\":" << pend << ",\"valorTotalAprovado\":" << total << "}";
        return httpResponse(os.str());
    }
    if (path == "/api/investigar") {
        int id = params.count("idFornecedor") ? std::stoi(params.at("idFornecedor")) : -1;
        Fornecedor* f = g_modulo.buscarFornecedorPorId(id);
        if (!f) return httpResponse("{\"sucesso\":false,\"msg\":\"Fornecedor não encontrado\"}");
        std::string url = "https://www.google.com/search?q=" + f->getNome() + "+CNPJ+" + f->getCNPJ();
        return httpResponse("{\"sucesso\":true,\"url\":\"" + jsonEscape(url) + "\"}");
    }
    if (path == "/api/estoque") return httpResponse(jsonEstoqueAtual(g_modulo.obterListaOrdens()));
    if (path == "/api/estoque/consultar") {
        int idMat = params.count("idMaterial") ? std::stoi(params.at("idMaterial")) : -1;
        int qtd = g_modulo.consultarEstoque(idMat);
        std::ostringstream os;
        os << "{\"idMaterial\":" << idMat << ",\"quantidade\":" << qtd << "}";
        return httpResponse(os.str());
    }
    if (path == "/api/estoque/previsto") return httpResponse(jsonPrevisto());
    if (path == "/api/producao") return httpResponse(jsonProducao());
    if (path == "/api/producao/pendentes") {
        std::vector<ProducaoRegistro> pend;
        for (const auto& p : g_producao) if (p.status != "finalizado" && p.status != "concluido") pend.push_back(p);
        std::ostringstream os; os << "[";
        for (size_t i = 0; i < pend.size(); ++i) {
            const auto& p = pend[i];
            os << "{" << "\"id\":" << p.id << ",\"idMaterial\":" << p.idMaterial << ",\"quantidade\":" << p.quantidade << "}";
            if (i + 1 < pend.size()) os << ",";
        }
        os << "]";
        return httpResponse(os.str());
    }
    if (path == "/api/financeiro") {
        const auto& ordens = g_modulo.obterListaOrdens();
        double total = 0;
        for (size_t i = 0; i < ordens.obterTamanho(); ++i) total += ordens.obter(i).getValorTotal();
        double contas = total * 0.4;
        std::ostringstream os;
        os << "{\"saldo\":" << total << ",\"saldo_disponivel\":" << total << ",\"contas_pagar\":" << contas << ",\"pendencias\":" << ordens.obterTamanho() << "}";
        return httpResponse(os.str());
    }
    if (path == "/api/financeiro/contas_pagar") {
        auto lista = g_modulo.getModuloFinanceiro()->listarContasPagar();
        std::ostringstream os; os << "[";
        for (size_t i = 0; i < lista.size(); ++i) {
            os << "{\"descricao\":\"" << jsonEscape(lista[i]) << "\"}";
            if (i + 1 < lista.size()) os << ",";
        }
        os << "]";
        return httpResponse(os.str());
    }
    if (path == "/api/financeiro/saldo") {
        double saldo = g_modulo.consultarSaldoFinanceiro();
        std::ostringstream os; os << "{\"saldo\":" << saldo << "}";
        return httpResponse(os.str());
    }
    if (path == "/api/salvar") {
        g_modulo.salvarTodosDados();
        salvarProducao();
        salvarPrevisto();
        return httpResponse("{\"sucesso\":true}");
    }
    if (path == "/api/carregar") {
        g_modulo.carregarTodosDados();
        carregarProducao();
        carregarPrevisto();
        return httpResponse("{\"sucesso\":true}");
    }
    return notFound();
}

void registrarPrevisto(int idMaterial, int quantidade, int idOrdem, const std::string& dataPrevista) {
    EstoquePrevisto e{ idMaterial, quantidade, idOrdem, dataPrevista };
    g_previsto.push_back(e);
    salvarPrevisto();
}

void registrarProducaoAutomatica(int idMaterial, int quantidade, int idOrdem, const std::string& dataPrevista) {
    ProducaoRegistro r;
    r.id = g_producaoNextId++;
    r.idMaterial = idMaterial;
    r.quantidade = quantidade;
    r.prioridade = 2;
    r.status = "em_preparacao";
    r.idOrdemCompra = idOrdem;
    r.dataCriacao = nowString();
    r.dataPrevistaEntrega = dataPrevista.empty() ? "A definir" : dataPrevista;
    g_producao.push_back(r);
    salvarProducao();
}

std::string handlePost(const std::string& path, const std::map<std::string, std::string>& params) {
    std::lock_guard<std::mutex> lock(g_mutex);

    if (path == "/api/fornecedores") {
        if (params.count("nome") == 0 || params.count("cnpj") == 0 || params.count("endereco") == 0 || params.count("produto") == 0 || params.count("preco") == 0)
            return httpResponse("{\"sucesso\":false,\"msg\":\"Parâmetros incompletos\"}");
        try {
            int id = g_modulo.adicionarFornecedor(params.at("nome"), params.at("endereco"), params.at("cnpj"), params.at("produto"), std::stod(params.at("preco")));
            g_modulo.salvarTodosDados();
            return httpResponse("{\"sucesso\":true,\"id\":" + std::to_string(id) + "}");
        } catch (const std::exception& e) {
            return httpResponse("{\"sucesso\":false,\"msg\":\"" + jsonEscape(e.what()) + "\"}");
        }
    }

    if (path == "/api/ordens") {
        if (params.count("idFornecedor") == 0 || params.count("idItem") == 0 || params.count("quantidade") == 0 || params.count("valor") == 0)
            return httpResponse("{\"sucesso\":false,\"msg\":\"Parâmetros incompletos\"}");
        try {
            int idItem = std::stoi(params.at("idItem"));
            int quantidade = std::stoi(params.at("quantidade"));
            double valor = std::stod(params.at("valor"));
            int idFornecedor = std::stoi(params.at("idFornecedor"));
            std::string dataChegada = params.count("data_chegada") ? params.at("data_chegada") : "";
            int id = g_modulo.criarOrdemCompra(idItem, quantidade, valor, idFornecedor, dataChegada);
            g_modulo.salvarTodosDados();
            registrarPrevisto(idItem, quantidade, id, dataChegada.empty() ? "Nao informada" : dataChegada);
            registrarProducaoAutomatica(idItem, quantidade, id, dataChegada);
            return httpResponse("{\"sucesso\":true,\"id\":" + std::to_string(id) + "}");
        } catch (const std::exception& e) {
            return httpResponse("{\"sucesso\":false,\"msg\":\"" + jsonEscape(e.what()) + "\"}");
        }
    }

    if (path == "/api/estoque/reservar") {
        int idMat = params.count("idMaterial") ? std::stoi(params.at("idMaterial")) : -1;
        int qtd = params.count("quantidade") ? std::stoi(params.at("quantidade")) : 0;
        bool ok = g_modulo.reservarMaterial(idMat, qtd);
        return httpResponse(ok ? "{\"sucesso\":true}" : "{\"sucesso\":false}");
    }

    if (path == "/api/estoque/entrada") {
        int idMat = params.count("idMaterial") ? std::stoi(params.at("idMaterial")) : -1;
        int qtd = params.count("quantidade") ? std::stoi(params.at("quantidade")) : 0;
        int idOrdem = params.count("idOrdemCompra") ? std::stoi(params.at("idOrdemCompra")) : 0;
        std::string dataPrev = params.count("data_prevista") ? params.at("data_prevista") : nowString();
        g_modulo.getModuloEstoque()->registrarEntradaCompra(idMat, qtd, idOrdem);
        registrarPrevisto(idMat, qtd, idOrdem, dataPrev);
        return httpResponse("{\"sucesso\":true}");
    }

    if (path == "/api/producao" || path == "/api/producao/pedido") {
        int idMat = params.count("idMaterial") ? std::stoi(params.at("idMaterial")) : -1;
        int qtd = params.count("quantidade") ? std::stoi(params.at("quantidade")) : 0;
        int prioridade = params.count("prioridade") ? std::stoi(params.at("prioridade")) : 2;
        int idOrdem = params.count("idOrdemCompra") ? std::stoi(params.at("idOrdemCompra")) : 0;
        std::string dataPrev = params.count("data_prevista") ? params.at("data_prevista") : "A definir";
        ProducaoRegistro r;
        r.id = g_producaoNextId++;
        r.idMaterial = idMat;
        r.quantidade = qtd;
        r.prioridade = prioridade;
        r.status = "pendente";
        r.idOrdemCompra = idOrdem;
        r.dataCriacao = nowString();
        r.dataPrevistaEntrega = dataPrev;
        g_producao.push_back(r);
        salvarProducao();
        return httpResponse("{\"sucesso\":true,\"id\":" + std::to_string(r.id) + "}");
    }

    return notFound();
}

std::pair<std::string, std::map<std::string, std::string>> parsePathAndParams(const std::string& pathWithQuery, const std::string& body) {
    auto pos = pathWithQuery.find('?');
    std::string path = (pos == std::string::npos) ? pathWithQuery : pathWithQuery.substr(0, pos);
    std::string query = (pos == std::string::npos) ? "" : pathWithQuery.substr(pos + 1);
    auto params = parseQuery(query);
    if (!body.empty()) {
        auto bodyParams = parseQuery(body);
        params.insert(bodyParams.begin(), bodyParams.end());
    }
    return {path, params};
}

void serve(int port) {
    if (!initSockets()) {
        std::cerr << "Erro ao inicializar sockets\n";
        return;
    }

    socket_t server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Erro ao criar socket\n";
        return;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Erro ao fazer bind. Porta em uso?\n";
        closeSocket(server_fd);
        return;
    }

    if (listen(server_fd, 10) < 0) {
        std::cerr << "Erro ao escutar\n";
        closeSocket(server_fd);
        return;
    }

    std::cout << "Servidor HTTP C++ na porta " << port << "\n";
    std::cout << "Endpoints expostos: /api/status, /api/fornecedores, /api/ordens, /api/estoque, /api/estoque/previsto, /api/producao, /api/financeiro" << "\n";

    g_modulo.carregarTodosDados();
    carregarProducao();
    carregarPrevisto();

    while (true) {
        sockaddr_in client{};
        socklen_arg len = static_cast<socklen_arg>(sizeof(client));
        socket_t client_fd = accept(server_fd, (sockaddr*)&client, &len);
        if (client_fd < 0) continue;

        char buffer[16384];
        int n = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0) { closeSocket(client_fd); continue; }
        buffer[n] = '\0';
        std::string req(buffer);

        std::istringstream iss(req);
        std::string method, path, version;
        iss >> method >> path >> version;

        std::string body;
        auto posBody = req.find("\r\n\r\n");
        if (posBody != std::string::npos) body = req.substr(posBody + 4);

        auto parsed = parsePathAndParams(path, body);
        const std::string cleanPath = parsed.first;
        const auto params = parsed.second;

        std::string response;
        if (method == "OPTIONS") {
            response = httpResponse("", 204);
        } else if (method == "GET") {
            response = handleGet(cleanPath, params);
        } else if (method == "POST") {
            response = handlePost(cleanPath, params);
        } else {
            response = notFound();
        }

        send(client_fd, response.c_str(), response.size(), 0);
        closeSocket(client_fd);
    }
}
} // namespace

#if SERVIDOR_STANDALONE
int main() {
    serve(8080);
    return 0;
}
#endif // SERVIDOR_STANDALONE

