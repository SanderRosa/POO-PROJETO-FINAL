#!/bin/bash
# Script para iniciar o servidor da API do Sistema de Gerenciamento de Compras

cd "$(dirname "$0")"

echo "🚀 Iniciando servidor HTTP..."
python3 servidor.py
