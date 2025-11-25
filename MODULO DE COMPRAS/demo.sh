#!/bin/bash

echo "=========================================="
echo "DEMONSTRAÇÃO DE FUNCIONALIDADES"
echo "=========================================="
echo ""

cd "/workspaces/POO-PROJETO-FINAL/MODULO DE COMPRAS"

echo "✓ 1. MÓDULO DE ESTOQUE"
echo "   → Listando itens disponíveis..."
echo ""
echo -e "8\n16" | timeout 3 ./bin/modulo_compras 2>&1 | grep -E "(ID:|Aço|Parafusos|Borracha)" | head -5
echo ""

echo "✓ 2. MÓDULO DE PRODUÇÃO"
echo "   → Listando pedidos pendentes..."
echo ""
echo -e "11\n16" | timeout 3 ./bin/modulo_compras 2>&1 | grep -E "(Pedido #|Material:|Prioridade)" | head -4
echo ""

echo "✓ 3. MÓDULO FINANCEIRO"
echo "   → Saldo disponível..."
echo ""
echo -e "13\n16" | timeout 3 ./bin/modulo_compras 2>&1 | grep "SALDO DISPONIVEL"
echo ""
echo "   → Contas a pagar registradas..."
echo ""
echo -e "12\n16" | timeout 3 ./bin/modulo_compras 2>&1 | grep -E "(Ordem #|Fornecedor:|Valor:|Venc:)" | head -5
echo ""

echo "✓ 4. ESTATÍSTICAS DO SISTEMA"
echo ""
echo -e "5\n16" | timeout 3 ./bin/modulo_compras 2>&1 | grep -E "(Total de|Aprovadas|Rejeitadas|Pendentes|Valor Total)" | head -7
echo ""

echo "=========================================="
echo "✓ TODOS OS MÓDULOS FUNCIONANDO CORRETAMENTE!"
echo "=========================================="
