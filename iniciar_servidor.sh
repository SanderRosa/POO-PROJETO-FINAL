#!/bin/bash
# Script para compilar e iniciar o servidor HTTP C++ (porta 8080)
set -e
cd "$(dirname "$0")"

mkdir -p build
echo "🔨 Compilando servidor C++..."
g++ -std=c++17 -O2 -pthread -o build/http_server src/servidor.cpp

echo "🚀 Iniciando servidor C++ na porta 8080..."
./build/http_server
