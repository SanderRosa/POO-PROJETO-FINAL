# GitHub Pages Configuration

O projeto está configurado para ser hospedado no GitHub Pages.

## Como Funciona

A pasta `/MODULO DE COMPRAS/` contém:
- `index.html` - Interface web principal
- `INTERFACE.md` - Documentação

O GitHub Pages automaticamente hospeda esses arquivos em:
**https://sanderrosa.github.io/POO-PROJETO-FINAL/**

## Estrutura de URLs

- Interface: `https://sanderrosa.github.io/POO-PROJETO-FINAL/MODULO%20DE%20COMPRAS/index.html`
- Documentação: `https://sanderrosa.github.io/POO-PROJETO-FINAL/MODULO%20DE%20COMPRAS/INTERFACE.md`

## Configuração no GitHub

Já está automático porque:
1. O repositório é público
2. Os arquivos `.html` estão no repositório
3. GitHub detecta e hospeda automaticamente

## Para Ativar Manualmente (se necessário)

1. Vá em **Settings** do repositório
2. Procure **Pages** na barra lateral
3. Em "Source", selecione `main` branch
4. Salve

Pronto! A interface estará disponível publicamente.

## Acesso

Qualquer pessoa pode acessar a interface web em qualquer navegador sem precisar:
- Compilar o código C++
- Instalar nada localmente
- Usar terminal/linha de comando

A interface funciona como uma Progressive Web App e permite:
- Visualizar dados
- Preencher formulários
- Interagir com o sistema (se backend estiver rodando)

## Observações

- A interface tenta conectar a um backend em `http://localhost:8080/api`
- Se não houver backend, o status mostra "Offline"
- Para usar completamente, execute o programa C++ localmente
