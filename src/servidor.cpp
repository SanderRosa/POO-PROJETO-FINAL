#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <chrono>
#include <thread>

// Servidor HTTP mínimo em C++ (Linux/Mac) para expor a API na porta 8080.
// Sem dependências externas.

#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
#include <cstring>
#endif

namespace {
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
            // decode + and %20 minimally
            std::replace(v.begin(), v.end(), '+', ' ');
            params[k] = v;
        }
    }
    return params;
}

struct Fornecedor {
    int id{};
    std::string nome;
    std::string cnpj;
    std::string endereco;
    std::string produto;
    double preco{};
};

struct Ordem {
    int id{};
    int idItem{};
    int quantidade{};
    double valor{};
    int idFornecedor{};
    int status{};
    std::string data;
};

std::vector<Fornecedor> carregarFornecedores(const std::string& path) {
    std::vector<Fornecedor> out;
    std::ifstream f(path);
    if (!f.is_open()) return out;
    std::string line;
    // descarta header
    std::getline(f, line);
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '\r') continue;
        auto parts = split(line, '|');
        if (parts.size() < 6 || parts[0].empty()) continue;
        Fornecedor fnd;
        fnd.id = std::stoi(parts[0]);
        fnd.nome = parts[1];
        fnd.endereco = parts[2];
        fnd.cnpj = parts[3];
        fnd.produto = parts[4];
        fnd.preco = std::stod(parts[5].empty() ? "0" : parts[5]);
        out.push_back(fnd);
    }
    return out;
}

std::vector<Ordem> carregarOrdens(const std::string& path) {
    std::vector<Ordem> out;
    std::ifstream f(path);
    if (!f.is_open()) return out;
    std::string line;
    // header
    std::getline(f, line);
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '\r') continue;
        auto parts = split(line, '|');
        if (parts.size() < 7 || parts[0].empty()) continue;
        Ordem o;
        o.id = std::stoi(parts[0]);
        o.idItem = std::stoi(parts[1]);
        o.quantidade = std::stoi(parts[2]);
        o.valor = std::stod(parts[3]);
        o.idFornecedor = std::stoi(parts[4]);
        o.status = std::stoi(parts[5]);
        o.data = parts[6];
        out.push_back(o);
    }
    return out;
}

std::string jsonFornecedores(const std::vector<Fornecedor>& v) {
    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        const auto& f = v[i];
        os << "{";
        os << "\"id\":" << f.id << ",";
        os << "\"nome\":\"" << jsonEscape(f.nome) << "\",";
        os << "\"produto\":\"" << jsonEscape(f.produto) << "\",";
        os << "\"preco\":" << f.preco << ",";
        os << "\"cnpj\":\"" << jsonEscape(f.cnpj) << "\",";
        os << "\"endereco\":\"" << jsonEscape(f.endereco) << "\"";
        os << "}";
        if (i + 1 < v.size()) os << ",";
    }
    os << "]";
    return os.str();
}

std::string jsonOrdens(const std::vector<Ordem>& v) {
    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        const auto& o = v[i];
        os << "{";
        os << "\"id\":" << o.id << ",";
        os << "\"item\":\"Item " << o.idItem << "\",";
        os << "\"quantidade\":" << o.quantidade << ",";
        os << "\"valor\":" << o.valor << ",";
        os << "\"status\":" << o.status << ",";
        os << "\"descricao\":\"" << jsonEscape(o.data) << "\"";
        os << "}";
        if (i + 1 < v.size()) os << ",";
    }
    os << "]";
    return os.str();
}

std::string jsonEstoque(const std::vector<Ordem>& ordens) {
    std::map<int, int> soma;
    for (const auto& o : ordens) soma[o.idItem] += o.quantidade;
    std::ostringstream os;
    os << "[";
    size_t idx = 0;
    for (const auto& kv : soma) {
        os << "{";
        os << "\"id\":" << kv.first << ",";
        os << "\"nome\":\"Item " << kv.first << "\",";
        os << "\"quantidade\":" << kv.second;
        os << "}";
        if (++idx < soma.size()) os << ",";
    }
    os << "]";
    return os.str();
}

std::string jsonFinanceiro(const std::vector<Ordem>& ordens) {
    double total = 0.0;
    for (const auto& o : ordens) total += o.valor * o.quantidade;
    std::ostringstream os;
    os << "{\"saldo\":" << total << ",";
    os << "\"saldo_disponivel\":" << total << ",";
    os << "\"contas_pagar\":" << (total * 0.4) << ",";
    os << "\"pendencias\":" << ordens.size() << "}";
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

std::string handleGet(const std::string& path, const std::vector<Fornecedor>& fornec, const std::vector<Ordem>& ordens) {
    if (path == "/api/status") return statusOk();
    if (path == "/api/fornecedores") return httpResponse(jsonFornecedores(fornec));
    if (path == "/api/ordens") return httpResponse(jsonOrdens(ordens));
    if (path == "/api/estoque") return httpResponse(jsonEstoque(ordens));
    if (path == "/api/financeiro") return httpResponse(jsonFinanceiro(ordens));
    return notFound();
}

bool appendFornecedor(const std::string& file, const std::map<std::string, std::string>& p, int nextId) {
    std::ofstream f(file, std::ios::app);
    if (!f.is_open()) return false;
    f << "\n" << nextId << "|" << p.at("nome") << "|" << p.at("endereco") << "|" << p.at("cnpj") << "|" << p.at("produto") << "|" << p.at("preco");
    return true;
}

bool appendOrdem(const std::string& file, const std::map<std::string, std::string>& p, int nextId) {
    std::ofstream f(file, std::ios::app);
    if (!f.is_open()) return false;
    const std::string data = ""; // omit data/hora para simplicidade
    f << "\n" << nextId << "|" << p.at("idItem") << "|" << p.at("quantidade") << "|" << p.at("valor") << "|" << p.at("idFornecedor") << "|0|" << data;
    return true;
}

std::string handlePost(const std::string& pathWithQuery, std::vector<Fornecedor>& fornec, std::vector<Ordem>& ordens, const std::string& fornecedoresFile, const std::string& ordensFile) {
    auto pos = pathWithQuery.find('?');
    const std::string path = (pos == std::string::npos) ? pathWithQuery : pathWithQuery.substr(0, pos);
    const std::string query = (pos == std::string::npos) ? "" : pathWithQuery.substr(pos + 1);
    auto params = parseQuery(query);

    if (path == "/api/fornecedores") {
        if (params.count("nome") == 0 || params.count("cnpj") == 0 || params.count("endereco") == 0 || params.count("produto") == 0 || params.count("preco") == 0)
            return httpResponse("{\"sucesso\":false,\"msg\":\"Parâmetros incompletos\"}");
        int nextId = fornec.empty() ? 1 : (fornec.back().id + 1);
        if (appendFornecedor(fornecedoresFile, params, nextId)) {
            Fornecedor f{nextId, params["nome"], params["cnpj"], params["endereco"], params["produto"], std::stod(params["preco"])};
            fornec.push_back(f);
            return httpResponse("{\"sucesso\":true}\");
        }
        return httpResponse("{\"sucesso\":false}\");
    }

    if (path == "/api/ordens") {
        if (params.count("idFornecedor") == 0 || params.count("idItem") == 0 || params.count("quantidade") == 0 || params.count("valor") == 0)
            return httpResponse("{\"sucesso\":false,\"msg\":\"Parâmetros incompletos\"}");
        int nextId = ordens.empty() ? 1 : (ordens.back().id + 1);
        if (appendOrdem(ordensFile, params, nextId)) {
            Ordem o{nextId, std::stoi(params["idItem"]), std::stoi(params["quantidade"]), std::stod(params["valor"]), std::stoi(params["idFornecedor"]), 0, ""};
            ordens.push_back(o);
            return httpResponse("{\"sucesso\":true}\");
        }
        return httpResponse("{\"sucesso\":false}\");
    }

    return notFound();
}

void serve(int port, const std::string& fornecedoresFile, const std::string& ordensFile) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Erro ao criar socket\n";
        return;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Erro ao fazer bind. Porta em uso?\n";
        close(server_fd);
        return;
    }

    if (listen(server_fd, 10) < 0) {
        std::cerr << "Erro ao escutar\n";
        close(server_fd);
        return;
    }

    std::cout << "Servidor HTTP C++ na porta " << port << "\n";
    std::cout << "Endpoints: /api/status, /api/fornecedores, /api/ordens, /api/estoque, /api/financeiro\n";
    std::cout << "POST para /api/fornecedores e /api/ordens (query params)\n";

    std::vector<Fornecedor> fornecedores = carregarFornecedores(fornecedoresFile);
    std::vector<Ordem> ordens = carregarOrdens(ordensFile);

    while (true) {
        sockaddr_in client{};
        socklen_t len = sizeof(client);
        int client_fd = accept(server_fd, (sockaddr*)&client, &len);
        if (client_fd < 0) continue;

        char buffer[8192];
        int n = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0) { close(client_fd); continue; }
        buffer[n] = '\0';
        std::string req(buffer);

        std::istringstream iss(req);
        std::string method, path, version;
        iss >> method >> path >> version;

        std::string response;
        if (method == "OPTIONS") {
            response = httpResponse("", 204);
        } else if (method == "GET") {
            response = handleGet(path, fornecedores, ordens);
        } else if (method == "POST") {
            response = handlePost(path, fornecedores, ordens, fornecedoresFile, ordensFile);
        } else {
            response = notFound();
        }

        send(client_fd, response.c_str(), response.size(), 0);
        close(client_fd);
    }
}
} // namespace

int main() {
    const std::string fornecedoresFile = "data/fornecedores.txt";
    const std::string ordensFile = "data/ordens.txt";
    serve(8080, fornecedoresFile, ordensFile);
    return 0;
}
