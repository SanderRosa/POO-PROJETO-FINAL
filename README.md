# POO-PROJETO-FINAL — Sistema de Gerenciamento de Compras

Sistema de compras em C++17 com interface web estática e servidor HTTP em C++.

## Estrutura
```
├── src/            # Código-fonte C++
├── include/        # Headers C++
├── data/           # Dados de exemplo (fornecedores, ordens)
├── interface/      # Interface web (index.html)
├── api/            # API estática (JSON) para modo leitura
├── iniciar_servidor.sh   # Compila e inicia o servidor HTTP C++
├── INTERFACE.md    # Detalhes da interface web
└── index.html      # Redireciona para interface/index.html
```

## Requisitos
- C++17 (g++)
- Python 3 (opcional, para API simples)
- Navegador moderno

## Backend C++ (console)
Compile e execute a partir da raiz:
```bash
mkdir -p build
g++ -std=c++17 -Iinclude -o build/modulo_compras src/*.cpp
./build/modulo_compras
```

## Servidor HTTP em C++ (API)
Da raiz do projeto:
```bash
bash iniciar_servidor.sh
```
- Compila `src/servidor.cpp` e inicia na porta `8080`.
- Endpoints: `/api/status`, `/api/fornecedores`, `/api/ordens`, `/api/estoque`, `/api/financeiro`, POST em `/api/fornecedores` e `/api/ordens` (parâmetros na query string).
- Usa os arquivos em `data/` para leitura/escrita simples.

## Interface Web
- Arquivo principal: `interface/index.html`
- Abrir localmente (somente leitura com API estática):
  ```bash
  python3 -m http.server 8080
  # depois abra http://localhost:8080/interface/
  ```
- Online (GitHub Pages): https://sanderrosa.github.io/POO-PROJETO-FINAL/interface/

### Conectando a um backend dinâmico
A interface detecta automaticamente a API estática (`/api`). Para usar seu backend:
```
https://sanderrosa.github.io/POO-PROJETO-FINAL/interface/?api=http://SEU_HOST:8080/api
```
- `SEU_HOST` pode ser o IP da máquina ou uma URL de túnel (ngrok, etc.).
- O valor fica salvo no navegador (localStorage).

## Dados
- `data/fornecedores.txt` e `data/ordens.txt` são lidos/escritos pela API Python e pelo módulo C++.
- JSONs em `api/` suportam o modo somente leitura (GitHub Pages/offline).

## Documentação da Interface
Consulte `INTERFACE.md` para detalhes de telas, fluxos e status.

## Licença
MIT (veja `LICENSE`).

