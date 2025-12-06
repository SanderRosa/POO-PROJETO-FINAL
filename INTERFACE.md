# Interface Gráfica - Sistema de Gerenciamento de Compras

## Visão Geral

A interface `index.html` é um frontend web para interagir com o módulo de compras C++ via API REST. Fornece uma experiência visual intuitiva para gerenciar fornecedores, ordens de compra, estoque e financeiro.

## Arquitetura

### Frontend (HTML/CSS/JavaScript)
- **SPA (Single Page Application)**: Carrega tudo uma vez, alterna entre seções sem recarregar
- **Responsivo**: Funciona em desktop, tablet e mobile
- **Design Moderno**: Paleta de cores consistente com componentes limpos

### Backend Esperado (C++17)
- API REST em `http://localhost:8080/api`
- Endpoints: `/estoque`, `/fornecedores`, `/ordens`, `/financeiro`
- Método GET: Retorna dados em JSON
- Método POST: Aceita parâmetros via query string

## Estrutura da Interface

### 1. **Barra Lateral (Sidebar)**
```
┌─────────────────────────┐
│  📦 Modulo de Compras   │
├─────────────────────────┤
│ 📊 Dashboard            │
│ 👥 Fornecedores        │
│ 🛒 Ordens de Compra    │
│ 📦 Estoque             │
├─────────────────────────┤
│ 🟢 Status: Online       │
└─────────────────────────┘
```

**Funcionalidades:**
- Navegação entre 4 seções principais
- Indicador de status do Backend (verde=online, vermelho=offline)
- Design fixo na esquerda com ícones Lucide

### 2. **Dashboard (Visão Geral)**
Exibe 4 cartões resumidos:
- **Saldo Financeiro**: Valor em R$ disponível
- **Fornecedores**: Total de fornecedores cadastrados
- **Itens em Estoque**: Soma de quantidade de materiais
- **Ordens Totais**: Número de ordens de compra

### 3. **Seção de Fornecedores**
**Formulário de Cadastro:**
```
┌─────────────────────────────────┐
│ Cadastrar Fornecedor            │
├─────────────────────────────────┤
│ Nome: [_______________]         │
│ CNPJ: [_______________]         │
│ Endereço: [_______________]     │
│ Produto: [_______________]      │
│ Preço (R$): [_______________]   │
│ [Salvar Fornecedor]             │
└─────────────────────────────────┘
```

**Tabela de Fornecedores:**
```
┌─────┬────────────┬──────────┬─────────┬─────────────┐
│ ID  │ Nome       │ Produto  │ Preço   │ CNPJ        │
├─────┼────────────┼──────────┼─────────┼─────────────┤
│ #1  │ Empresa X  │ Aço Inox │ R$ 150  │ XX.XXX.XXX  │
└─────┴────────────┴──────────┴─────────┴─────────────┘
```

### 4. **Seção de Ordens de Compra**
**Formulário de Nova Ordem:**
```
┌──────────────────────────────────┐
│ Nova Ordem de Compra             │
├──────────────────────────────────┤
│ Fornecedor (ID): [___________]   │
│ Item (ID): [___________]         │
│ Quantidade: [___________]        │
│ Valor Unit. (R$): [___________]  │
│ [Gerar Ordem]                    │
└──────────────────────────────────┘
```

**Tabela de Ordens:**
```
┌────┬────────┬─────┬──────────┬─────────────┐
│ ID │ Item   │ Qtd │ Total    │ Status      │
├────┼────────┼─────┼──────────┼─────────────┤
│ #1 │ Item #1│ 50  │ R$ 1500  │ ✓ Aprovado  │
│ #2 │ Item #2│ 100 │ R$ 2000  │ ⏳ Pendente  │
└────┴────────┴─────┴──────────┴─────────────┘
```

### 5. **Seção de Estoque**
**Tabela de Inventário:**
```
┌────┬──────────────┬─────────┬────────┐
│ ID │ Material     │ Qtd     │ Status │
├────┼──────────────┼─────────┼────────┤
│ #1 │ Aço Inox     │ 100     │ ✓ OK   │
│ #2 │ Parafusos M10│ 500     │ ✓ OK   │
│ #3 │ Borracha     │ 30      │ ⚠ Baixo│
└────┴──────────────┴─────────┴────────┘
```

**Cores de Status:**
- 🟢 **Baixo**: Quantidade < 50 unidades
- 🟢 **OK**: Quantidade >= 50 unidades

## Fluxo de Dados

### Inicialização
```
1. Carrega index.html
2. Inicializa ícones Lucide
3. Chama atualizarDados()
4. Estabelece conexão com Backend
5. Exibe dados ou mensagem de offline
```

### Atualização Automática
```
- A cada 5 segundos: setInterval(atualizarDados, 5000)
- Atualiza todas as tabelas sem recarregar a página
- Mantém a seção atual ativa
```

### Fluxo de Cadastro
```
1. Usuário preenche formulário
2. Submit chamada: cadastrarFornecedor(event)
3. Coleta dados do formulário
4. Envia POST para: http://localhost:8080/api/fornecedores?nome=X&...
5. Exibe mensagem de sucesso
6. Limpa o formulário
7. Chama atualizarDados() para refrescar lista
```

## Endpoints Esperados do Backend

### 1. GET `/api/fornecedores`
**Resposta esperada:**
```json
[
  {
    "id": 1,
    "nome": "Empresa X",
    "produto": "Aço Inox",
    "preco": 150.00,
    "cnpj": "01.234.567/0001-89"
  }
]
```

### 2. GET `/api/ordens`
**Resposta esperada:**
```json
[
  {
    "id": 1,
    "idItem": 1,
    "quantidade": 50,
    "valor": 1500.00,
    "status": 1
  }
]
```

### 3. GET `/api/estoque`
**Resposta esperada:**
```json
[
  {
    "id": 1,
    "nome": "Aço Inox",
    "quantidade": 100
  }
]
```

### 4. GET `/api/financeiro`
**Resposta esperada:**
```json
{
  "saldo": 100000.00
}
```

### 5. POST `/api/fornecedores`
**Parâmetros:** `?nome=X&cnpj=Y&endereco=Z&produto=P&preco=V`

### 6. POST `/api/ordens`
**Parâmetros:** `?idFornecedor=X&idItem=Y&quantidade=Z&valor=V`

## Componentes Visuais

### Cores do Sistema
- **Azul Primário** (#2563eb): Botões, links, navegação ativa
- **Cinza Claro** (#f3f4f6): Fundo da página
- **Branco** (#ffffff): Cartões e painéis
- **Verde** (#10b981): Sucesso, status OK
- **Vermelho** (#ef4444): Erro, offline
- **Amarelo** (#f59e0b): Aviso, pendente

### Ícones (Lucide)
- `package`: Logo do módulo
- `layout-dashboard`: Dashboard
- `users`: Fornecedores
- `shopping-cart`: Ordens
- `boxes`: Estoque

## Como Usar

### 1. Abrir a Interface
```bash
# Abrir index.html no navegador
open index.html
# ou
firefox index.html
# ou
chrome index.html
```

### 2. Iniciar o Backend C++
```bash
cd /workspaces/POO-PROJETO-FINAL/MODULO\ DE\ COMPRAS
./bin/modulo_compras
```

### 3. Esperar Conexão
A interface tentará conectar automaticamente:
- Se online: indicador verde + dados carregam
- Se offline: indicador vermelho + dados não carregam

### 4. Navegar entre Seções
Clique nos botões da sidebar para alternar entre:
- Dashboard
- Fornecedores
- Ordens de Compra
- Estoque

### 5. Operações
- **Cadastrar**: Preencha formulário e clique em "Salvar"
- **Atualizar**: Clique no botão "Atualizar Dados" no topo
- **Visualizar**: Veja as tabelas com dados em tempo real

## Resposta de Erro e Tratamento

### Backend Offline
```
Status: 🔴 Desconectado
Ação: Interface exibe "--" nos campos
Retry: Automático a cada 5 segundos
```

### Falha ao Criar Ordem
```
Endpoint: POST /api/ordens
Resposta: { "sucesso": false, "msg": "Motivo do erro" }
Ação: Exibe alert() com mensagem de erro
```

## Personalização

### Alterar URL do Backend
Edite a linha:
```javascript
const API_URL = 'http://localhost:8080/api';
```

### Alterar Intervalo de Atualização
Edite a linha (5000 ms = 5 segundos):
```javascript
setInterval(atualizarDados, 5000);
```

### Alterar Cores
Edite as variáveis CSS:
```css
:root {
    --primary: #2563eb;      /* Altere a cor azul aqui */
    --bg: #f3f4f6;           /* Altere o fundo aqui */
    --success: #10b981;      /* Altere verde aqui */
}
```

## Requisitos

- Navegador moderno (Chrome, Firefox, Safari, Edge)
- JavaScript habilitado
- Conexão com a internet para carregar ícones Lucide (CDN)
- Backend C++ rodando em `http://localhost:8080`

## Notas Técnicas

1. **SPA (Single Page Application)**: Usa `router()` para alternar entre views sem recarregar
2. **Fetch API**: Comunicação HTTP com o backend via `fetch()`
3. **JSON**: Formato de troca de dados
4. **Query Strings**: Parâmetros passados via URL (não usa JSON POST)
5. **Auto-refresh**: Atualização automática a cada 5 segundos
6. **Status Online/Offline**: Detectado automaticamente nas requisições

## Compatibilidade

- ✅ Desktop (Chrome, Firefox, Safari, Edge)
- ✅ Tablet (iPad, Android tablets)
- ❌ Mobile (layout não otimizado para telas < 375px)

## Desenvolvido para

Sistema de Gerenciamento de Compras - POO Projeto Final
Desenvolvido em C++17 com integração de módulos ERP.
