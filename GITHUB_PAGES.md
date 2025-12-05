# GitHub Pages Configuration

✅ O projeto está **TOTALMENTE CONFIGURADO** para GitHub Pages!

## 🌐 Acesso Imediato

### URLs Ativas:
- **Interface Principal**: https://sanderrosa.github.io/POO-PROJETO-FINAL/
- **URL Alternativa**: https://sanderrosa.github.io/POO-PROJETO-FINAL/MODULO%20DE%20COMPRAS/index.html

## Como Funciona

O repositório está configurado com:

### Arquivos Principais
- `index.html` (raiz) - Redirecionador inteligente para a interface
- `MODULO DE COMPRAS/index.html` - Interface web principal
- `_config.yml` - Configuração do GitHub Pages
- `.nojekyll` - Desativa processamento Jekyll

### Fluxo de Funcionamento
1. Usuário acessa `https://sanderrosa.github.io/POO-PROJETO-FINAL/`
2. O `index.html` raiz detecta o ambiente (GitHub Pages ou local)
3. Redireciona automaticamente para `MODULO DE COMPRAS/index.html`
4. Interface carrega e funciona normalmente

## 📋 Estrutura de Hospedagem

```
GitHub Pages (Branch: main)
├── index.html ..................... Página de redirecionamento
├── .nojekyll ...................... Desativa Jekyll
├── _config.yml .................... Configuração do Pages
└── MODULO DE COMPRAS/
    ├── index.html ................. Interface web completa
    ├── INTERFACE.md ............... Documentação
    ├── bin/ ....................... Executáveis C++
    ├── data/ ....................... Dados persistidos
    └── [outros arquivos]
```

## ✨ Funcionalidades

A interface web permite:
- ✅ Visualizar dados de fornecedores
- ✅ Gerenciar ordens de compra
- ✅ Visualizar histórico
- ✅ Preencher formulários
- ✅ Integração com backend (se estiver rodando)

## 🔧 Configuração (Já Feita!)

O repositório já possui:
- ✅ Branch `main` como source
- ✅ Arquivo `_config.yml` configurado
- ✅ Arquivo `.nojekyll` para site dinâmico
- ✅ Redirecionamento inteligente ativado
- ✅ Permissões públicas

**Nenhuma configuração manual adicional é necessária!**

## 📱 Acesso em Qualquer Dispositivo

Qualquer pessoa pode acessar via:
- ✅ Navegador de Desktop
- ✅ Navegador Mobile
- ✅ Tablet
- ✅ Qualquer dispositivo com internet

Sem precisar de:
- ❌ Compilar código C++
- ❌ Instalar dependências
- ❌ Usar terminal
- ❌ Executar localmente

## 🔌 Integração Backend

A interface web conecta a um servidor HTTP REST que fornece dados dinâmicos.

### Servidor em Execução

Para ativar o backend e deixar a interface completamente funcional:

```bash
cd "MODULO DE COMPRAS"
python3 servidor.py
```

Ou use o script:

```bash
cd "MODULO DE COMPRAS"
bash iniciar_servidor.sh
```

#### Usar um host diferente (LAN ou túnel)
Ao abrir a interface no GitHub Pages, a página não consegue acessar `localhost` da sua máquina. Passe a URL da API na query string:

```
https://sanderrosa.github.io/POO-PROJETO-FINAL/?api=http://SEU_HOST:8080/api
```

Exemplos de `SEU_HOST`:
- IP da sua máquina na rede local (ex.: `http://192.168.0.12:8080/api`)
- URL de túnel (ngrok, localhost.run, etc.)

Depois de abrir uma vez com `?api=...`, a URL fica salva no navegador (localStorage).

### Estados de Conexão:
- 🟢 **Online** - Backend ativo em `http://localhost:8080/api`
- 🔴 **Offline** - Backend indisponível (modo leitura de dados)

### Endpoints Disponíveis
- `GET  /api/fornecedores` - Lista fornecedores cadastrados
- `GET  /api/ordens` - Lista ordens de compra
- `GET  /api/estoque` - Informações de estoque
- `GET  /api/financeiro` - Dados financeiros
- `POST /api/fornecedores` - Criar novo fornecedor
- `POST /api/ordens` - Criar nova ordem de compra
- `GET  /api/status` - Verificar status do servidor

## 📚 Próximos Passos

1. Acessar: https://sanderrosa.github.io/POO-PROJETO-FINAL/
2. Verificar se interface carrega corretamente
3. Compilar e rodar o backend C++ para funcionalidade completa

## 📦 Download rápido do backend

- Página com download: https://sanderrosa.github.io/POO-PROJETO-FINAL/downloads/
- Link direto: https://sanderrosa.github.io/POO-PROJETO-FINAL/downloads/backend.zip

Como usar após baixar:
```
python3 servidor.py
```
Ou
```
bash iniciar_servidor.sh
```
Depois abra a interface passando a URL da API (ajuste SEU_HOST/IP):
```
https://sanderrosa.github.io/POO-PROJETO-FINAL/?api=http://SEU_HOST:8080/api
```

## 🔄 Backend estático (somente leitura)

Para que a interface mostre “Online” mesmo sem rodar nada local, há um backend estático hospedado no próprio GitHub Pages:

- API: `https://sanderrosa.github.io/POO-PROJETO-FINAL/api`
- Endpoints em `.json` (ex.: `/fornecedores.json`, `/ordens.json`, `/estoque.json`, `/financeiro.json`, `/status.json`)

A interface já detecta automaticamente quando está no GitHub Pages e usa esses arquivos. Para operações de escrita (POST), ainda é necessário rodar o backend local ou expô-lo via túnel.

## ⚠️ Status Atual

**Configuração**: ✅ Completa
**Hospedagem**: ✅ Ativa
**Interface**: ✅ Acessível
**Backend**: ⚠️ Requer execução local
- Para usar completamente, execute o programa C++ localmente
