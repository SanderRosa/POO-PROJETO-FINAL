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

## Como rodar em localhost
1) Inicie o backend C++ na porta 8080 (a partir da raiz):
```bash
bash iniciar_servidor.sh
```

2) Abra a interface diretamente no navegador em `interface/index.html` ou sirva os arquivos estáticos com qualquer servidor HTTP simples (ex.: `busybox httpd -f -p 3000`) em uma porta diferente de 8080.

3) A interface, por padrão, aponta a API para `http://localhost:8080/api`. Para trocar o backend, adicione `?api=http://HOST:PORTA/api` na URL.

## API HTTP (C++)
- Endpoints: `/api/status`, `/api/fornecedores`, `/api/ordens`, `/api/estoque`, `/api/financeiro`
- POST (query string): `/api/fornecedores` (nome, cnpj, endereco, produto, preco) e `/api/ordens` (idFornecedor, idItem, quantidade, valor)
- Arquivos usados: `data/fornecedores.txt` e `data/ordens.txt`

## Build manual (linha de comando, sem servidor HTTP)
```bash
mkdir -p build
g++ -std=c++17 -Iinclude \
  src/main.cpp src/ModuloCompras.cpp src/GerenciadorFornecedores.cpp \
  src/GerenciadorOrdens.cpp src/PersistenciaCompras.cpp \
  -o build/modulo_compras
./build/modulo_compras
```
> Observação: `src/servidor.cpp` tem `main` próprio; não inclua no binário acima. Use `bash iniciar_servidor.sh` para subir o servidor HTTP.

### Windows (MinGW/CLion)
- Certifique-se de usar C++17 ou superior.
- Para compilar apenas o servidor HTTP, inclua somente `src/servidor.cpp` e os headers em `include/` (não misture com `src/main.cpp`).

## Licença
MIT (veja `LICENSE`).

