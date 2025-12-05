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

A interface tenta conectar a um backend em `http://localhost:8080/api`

### Estados de Conexão:
- 🟢 **Online** - Backend ativo e respondendo
- 🔴 **Offline** - Backend indisponível (modo leitura)
- ⚠️ **Erro** - Problema na conexão

## 📚 Próximos Passos

1. Acessar: https://sanderrosa.github.io/POO-PROJETO-FINAL/
2. Verificar se interface carrega corretamente
3. Compilar e rodar o backend C++ para funcionalidade completa

## ⚠️ Status Atual

**Configuração**: ✅ Completa
**Hospedagem**: ✅ Ativa
**Interface**: ✅ Acessível
**Backend**: ⚠️ Requer execução local
- Para usar completamente, execute o programa C++ localmente
