#!/bin/bash

echo "=========================================="
echo "TESTE DE INTEGRAÇÃO - CRIAR ORDEM DE COMPRA"
echo "=========================================="
echo ""
echo "Este teste irá criar uma ordem de compra e mostrar"
echo "a integração entre Estoque, Produção e Financeiro"
echo ""

# Criar ordem de compra: Fornecedor 1, Material 5, Qtd 10, Valor Unit 50.00
echo "Criando Ordem de Compra:"
echo "  - Fornecedor: #1"
echo "  - Material: #5"
echo "  - Quantidade: 10"
echo "  - Valor Unitário: R$ 50.00"
echo ""

printf "3\n1\n5\n10\n50.00\n16\n" | timeout 20 ./bin/modulo_compras 2>&1 | grep -A 80 "CRIAR NOVA"

echo ""
echo "=========================================="
echo "TESTE CONCLUÍDO"
echo "=========================================="
