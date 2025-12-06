# POO-PROJETO-FINAL — Sistema de Gerenciamento de Compras

Aplicação em C++17 com API HTTP simples e interface web. Tudo preparado para rodar em localhost.

## Estrutura
```
├── src/            # Código-fonte C++
├── include/        # Headers C++
├── data/           # Dados de exemplo (fornecedores, ordens)
├── interface/      # Interface web (index.html)
├── api/            # JSON estáticos (modo leitura)
├── iniciar_servidor.sh   # Compila e inicia o servidor HTTP C++
├── INTERFACE.md    # Detalhes da interface web
└── index.html      # Redireciona para interface/index.html
```

## Requisitos
- g++ com suporte a C++17
- Navegador moderno

## Como rodar em localhost (Windows, PowerShell)
1) Inicie o backend C++ na porta 8080 (a partir da raiz):
```powershell
./iniciar_servidor.ps1
```

2) Abra a interface diretamente no navegador em `interface/index.html` ou sirva os arquivos estáticos com qualquer servidor HTTP simples em uma porta diferente de 8080. Exemplo usando PowerShell:
```powershell
cd interface
python -m http.server 3000
```
Depois acesse `http://localhost:3000/interface/`.

3) A interface, por padrão, aponta a API para `http://localhost:8080/api`. Para trocar o backend, adicione `?api=http://HOST:PORTA/api` na URL.

## API HTTP (C++)
- Endpoints: `/api/status`, `/api/fornecedores`, `/api/ordens`, `/api/estoque`, `/api/financeiro`
- POST (query string): `/api/fornecedores` (nome, cnpj, endereco, produto, preco) e `/api/ordens` (idFornecedor, idItem, quantidade, valor)
- Arquivos usados: `data/fornecedores.txt` e `data/ordens.txt`

## Build manual (linha de comando, sem servidor HTTP)
```powershell
New-Item -ItemType Directory -Force build | Out-Null
g++ -std=c++17 -Iinclude `
  src/main.cpp src/ModuloCompras.cpp src/GerenciadorFornecedores.cpp `
  src/GerenciadorOrdens.cpp src/PersistenciaCompras.cpp `
  -o build/modulo_compras.exe
./build/modulo_compras.exe
```
> Observação: `src/servidor.cpp` tem `main` próprio; não inclua no binário acima. Use `./iniciar_servidor.ps1` para subir o servidor HTTP.

### Windows (MinGW/CLion)
- Certifique-se de usar C++17 ou superior.
- Console (sem servidor HTTP): compile `src/main.cpp`, `ModuloCompras.cpp`, `GerenciadorFornecedores.cpp`, `GerenciadorOrdens.cpp`, `PersistenciaCompras.cpp` (saída `.exe`).
- Servidor HTTP: `g++ -std=c++17 -DSERVIDOR_STANDALONE=1 -Iinclude src/servidor.cpp -lws2_32 -o http_server.exe` e execute `./http_server.exe`.
- Se `src/servidor.cpp` for incluído em um alvo que já tem `main.cpp`, defina `-DSERVIDOR_STANDALONE=0` para evitar `main` duplicado.

## Licença
MIT (veja `LICENSE`).

