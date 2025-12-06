# PowerShell script to compile and run the C++ HTTP server on Windows (port 8080)
$ErrorActionPreference = 'Stop'

# Go to repo root (script directory)
Set-Location -Path $PSScriptRoot

# Ensure build directory exists
New-Item -ItemType Directory -Force -Path "build" | Out-Null

Write-Host "🔨 Compilando servidor C++ (Windows)..."
# Compile with MinGW g++; add ws2_32 for sockets
# Adjust the path to g++ if it's not on PATH
& g++ -std=c++17 -O2 -Iinclude src/servidor.cpp -lws2_32 -o build/http_server.exe

Write-Host "🚀 Iniciando servidor C++ na porta 8080..."
& ./build/http_server.exe
