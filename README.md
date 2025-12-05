# POO-PROJETO-FINAL: Sistema de Gerenciamento de Compras

## Visão Geral

Sistema completo de gerenciamento de compras desenvolvido em C++17 com:
- **Backend**: Módulo em C++ com interfaces de integração (Estoque, Produção, Financeiro)
- **Frontend**: Interface web responsiva (HTML/CSS/JavaScript)
- **Persistência**: Arquivos de dados em formato pipe-delimited

## 🚀 Acesso Rápido

### Interface Web Online
📱 **Acesse agora**: [https://sanderrosa.github.io/POO-PROJETO-FINAL/MODULO%20DE%20COMPRAS/](https://sanderrosa.github.io/POO-PROJETO-FINAL/MODULO%20DE%20COMPRAS/)

A interface está hospedada no GitHub Pages e funciona diretamente no navegador!

## Estrutura do Projeto

```
MODULO DE COMPRAS/
├── src/                 # Código-fonte C++
├── include/            # Headers C++
├── bin/                # Executáveis compilados
├── data/               # Arquivos de dados
├── index.html          # Interface web
├── INTERFACE.md        # Documentação da interface
└── [scripts de teste]
```

## Compilação e Execução

### Compilar
```bash
cd "MODULO DE COMPRAS"
g++ -std=c++17 -Iinclude -o bin/modulo_compras src/*.cpp
```

### Executar
```bash
./bin/modulo_compras
```

### Testar
```bash
./teste_completo.sh
./teste_integracao.sh
```

## Tecnologias Utilizadas

- **Linguagem**: C++17
- **Padrões**: Facade, Dependency Injection, Template Method
- **Concorrência**: std::thread, std::mutex
- **Estruturas**: std::vector, std::string, templates
- **Frontend**: HTML5, CSS3, JavaScript (Vanilla)

## Recursos Principais

### Backend (C++)
- Gestão de fornecedores com threads
- Gestão de ordens de compra
- Persistência em arquivos
- Integração com módulos externos (Estoque, Produção, Financeiro)
- Tratamento de exceções personalizado

### Frontend (Web)
- Dashboard com cards de resumo
- Formulários para cadastro de fornecedores e ordens
- Tabelas com listagem de dados
- Status de conexão em tempo real
- Auto-refresh de dados
- Design responsivo

## Padrões de Projeto

1. **Facade Pattern**: `ModuloCompras` como ponto de entrada único
2. **Dependency Injection**: Módulos recebem dependências
3. **Template Method**: `ListaGenerica<T>` genérica
4. **Observer Pattern**: Mock modules simulam comportamento

## Classes Principais

- **Pessoa**: Classe abstrata base
- **Fornecedor**: Herda de Pessoa
- **OrdemCompra**: Representa uma ordem com status enum
- **GerenciadorFornecedores**: Gerencia fornecedores (thread-safe)
- **GerenciadorOrdens**: Gerencia ordens (thread-safe)
- **ListaGenerica<T>**: Template para listas
- **PersistenciaCompras**: Lê/escreve em arquivos
- **ModuloCompras**: Facade coordenador

## Interfaces de Integração

- **IEstoque**: Operações com estoque
- **IProducao**: Operações com produção
- **IFinanceiro**: Operações com financeiro
- **IExibivel**: Interface para exibição de dados

## Formato de Dados

### fornecedores.txt
```
ID|Nome|Endereco|CNPJ|Produto|Preco
1|Aços Brasil|São Paulo|01.234.567/0001-89|Aço Inox|150.00
```

### ordens.txt
```
ID|IdItem|Quantidade|ValorUnitario|IdFornecedor|Status|DataSolicitacao
1|1|50|150.00|1|0|04/12/2025 10:30:00
```

## Como Usar a Interface

1. **Abra**: `index.html` no navegador
2. **Dashboard**: Visualize resumo dos dados
3. **Fornecedores**: Cadastre e liste fornecedores
4. **Ordens**: Crie e acompanhe ordens de compra
5. **Estoque**: Veja o inventário

A interface atualiza automaticamente a cada 5 segundos e exibe status de conexão.

## Documentação Completa

Veja [INTERFACE.md](MODULO%20DE%20COMPRAS/INTERFACE.md) para documentação detalhada da interface web.

## Requisitos

- G++ com suporte C++17
- Navegador moderno (Chrome, Firefox, Safari, Edge)

## Autor

Desenvolvido como Projeto Final de Programação Orientada a Objetos

---

**Última atualização**: 05/12/2025
