#!/bin/bash

echo "=========================================="
echo "TESTE COMPLETO DO MÓDULO DE COMPRAS"
echo "=========================================="
echo ""

# Teste 1: Listar itens do estoque
echo "### TESTE 1: Listar Itens do Estoque ###"
echo -e "8\n16" | timeout 3 ./bin/modulo_compras 2>&1 | grep -A 10 "ITENS DO ESTOQUE"
echo ""

# Teste 2: Consultar item específico do estoque
echo "### TESTE 2: Consultar Item ID 1 do Estoque ###"
echo -e "7\n1\n16" | timeout 3 ./bin/modulo_compras 2>&1 | grep -A 5 "CONSULTAR ITEM"
echo ""

# Teste 3: Listar pedidos de produção pendentes
echo "### TESTE 3: Listar Pedidos de Produção Pendentes ###"
echo -e "11\n16" | timeout 3 ./bin/modulo_compras 2>&1 | grep -A 8 "PEDIDOS DE PRODUCAO"
echo ""

# Teste 4: Consultar saldo financeiro
echo "### TESTE 4: Consultar Saldo Financeiro ###"
echo -e "13\n16" | timeout 3 ./bin/modulo_compras 2>&1 | grep "SALDO DISPONIVEL"
echo ""

# Teste 5: Listar contas a pagar
echo "### TESTE 5: Listar Contas a Pagar ###"
echo -e "12\n16" | timeout 3 ./bin/modulo_compras 2>&1 | grep -A 5 "CONTAS A PAGAR"
echo ""

# Teste 6: Criar pedido de material para produção
echo "### TESTE 6: Criar Pedido de Material (ID 3, Qtd 25, Prioridade Alta) ###"
echo -e "10\n3\n25\n3\n16" | timeout 3 ./bin/modulo_compras 2>&1 | grep -A 8 "CRIAR PEDIDO"
echo ""

# Teste 7: Reservar material do estoque
echo "### TESTE 7: Reservar Material ID 1 (Qtd 10) ###"
echo -e "9\n1\n10\n16" | timeout 3 ./bin/modulo_compras 2>&1 | grep -A 5 "RESERVAR MATERIAL"
echo ""

# Teste 8: Listar fornecedores
echo "### TESTE 8: Listar Fornecedores Cadastrados ###"
echo -e "2\n16" | timeout 3 ./bin/modulo_compras 2>&1 | grep -A 15 "LISTA DE FORNECEDORES"
echo ""

# Teste 9: Exibir estatísticas
echo "### TESTE 9: Exibir Estatísticas do Sistema ###"
echo -e "5\n16" | timeout 3 ./bin/modulo_compras 2>&1 | grep -A 10 "Total de"
echo ""

echo "=========================================="
echo "TESTES CONCLUÍDOS"
echo "=========================================="
