#!/usr/bin/env python3
"""
Servidor HTTP para API do Sistema de Gerenciamento de Compras
Fornece endpoints REST que simulam ou integram com o backend C++
"""

from http.server import HTTPServer, BaseHTTPRequestHandler
from urllib.parse import urlparse, parse_qs
import json
import os
import sys
from pathlib import Path

# Diretório dos dados
DATA_DIR = Path(__file__).parent / "data"
FORNECEDORES_FILE = DATA_DIR / "fornecedores.txt"
ORDENS_FILE = DATA_DIR / "ordens.txt"

class APIHandler(BaseHTTPRequestHandler):
    """Handler para requisições HTTP"""
    
    def do_GET(self):
        """Processa requisições GET"""
        parsed_path = urlparse(self.path)
        path = parsed_path.path
        
        # Habilitar CORS
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        self.end_headers()
        
        try:
            if path == '/api/fornecedores':
                response = self.buscar_fornecedores()
            elif path == '/api/ordens':
                response = self.buscar_ordens()
            elif path == '/api/estoque':
                response = self.buscar_estoque()
            elif path == '/api/financeiro':
                response = self.buscar_financeiro()
            elif path == '/api/status':
                response = {'status': 'online', 'message': 'Backend conectado'}
            else:
                response = {'error': 'Endpoint não encontrado', 'path': path}
            
            self.wfile.write(json.dumps(response, ensure_ascii=False).encode())
        except Exception as e:
            error_response = {'error': str(e)}
            self.wfile.write(json.dumps(error_response).encode())
    
    def do_POST(self):
        """Processa requisições POST"""
        parsed_path = urlparse(self.path)
        path = parsed_path.path
        
        # Ler dados do corpo
        content_length = int(self.headers.get('Content-Length', 0))
        body = self.rfile.read(content_length).decode()
        
        # Habilitar CORS
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        self.end_headers()
        
        try:
            if path == '/api/fornecedores':
                response = self.criar_fornecedor(body)
            elif path == '/api/ordens':
                response = self.criar_ordem(body)
            else:
                response = {'error': 'Endpoint não encontrado'}
            
            self.wfile.write(json.dumps(response, ensure_ascii=False).encode())
        except Exception as e:
            error_response = {'error': str(e)}
            self.wfile.write(json.dumps(error_response).encode())
    
    def do_OPTIONS(self):
        """Processa requisições OPTIONS para CORS"""
        self.send_response(200)
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        self.end_headers()
    
    def buscar_fornecedores(self):
        """Busca fornecedores do arquivo de dados"""
        fornecedores = []
        try:
            if FORNECEDORES_FILE.exists():
                with open(FORNECEDORES_FILE, 'r', encoding='utf-8') as f:
                    for line in f:
                        if line.strip():
                            parts = line.strip().split('|')
                            if len(parts) >= 5:
                                fornecedores.append({
                                    'id': parts[0],
                                    'nome': parts[1],
                                    'cnpj': parts[2],
                                    'telefone': parts[3],
                                    'email': parts[4]
                                })
        except Exception as e:
            print(f"Erro ao ler fornecedores: {e}", file=sys.stderr)
        
        return fornecedores
    
    def buscar_ordens(self):
        """Busca ordens de compra do arquivo de dados"""
        ordens = []
        try:
            if ORDENS_FILE.exists():
                with open(ORDENS_FILE, 'r', encoding='utf-8') as f:
                    for line in f:
                        if line.strip():
                            parts = line.strip().split('|')
                            if len(parts) >= 6:
                                ordens.append({
                                    'id': parts[0],
                                    'fornecedor_id': parts[1],
                                    'valor': float(parts[2]) if parts[2] else 0,
                                    'data': parts[3],
                                    'status': parts[4],
                                    'descricao': parts[5] if len(parts) > 5 else ''
                                })
        except Exception as e:
            print(f"Erro ao ler ordens: {e}", file=sys.stderr)
        
        return ordens
    
    def buscar_estoque(self):
        """Retorna informações do estoque"""
        return {
            'total_itens': 42,
            'valor_total': 12500.50,
            'itens_baixo_estoque': 3,
            'status': 'OK'
        }
    
    def buscar_financeiro(self):
        """Retorna informações financeiras"""
        return {
            'contas_pagar': 25000.00,
            'saldo_disponivel': 50000.00,
            'pendencias': 5,
            'status': 'OK'
        }
    
    def criar_fornecedor(self, body):
        """Adiciona um novo fornecedor"""
        try:
            data = json.loads(body)
            
            # Ler ID máximo
            max_id = 0
            if FORNECEDORES_FILE.exists():
                with open(FORNECEDORES_FILE, 'r', encoding='utf-8') as f:
                    for line in f:
                        if line.strip():
                            parts = line.strip().split('|')
                            max_id = max(max_id, int(parts[0]))
            
            new_id = max_id + 1
            
            # Escrever novo fornecedor
            with open(FORNECEDORES_FILE, 'a', encoding='utf-8') as f:
                f.write(f"{new_id}|{data.get('nome', '')}|{data.get('cnpj', '')}|"
                       f"{data.get('telefone', '')}|{data.get('email', '')}\n")
            
            return {
                'success': True,
                'message': 'Fornecedor criado com sucesso',
                'id': new_id
            }
        except Exception as e:
            return {'success': False, 'error': str(e)}
    
    def criar_ordem(self, body):
        """Adiciona uma nova ordem de compra"""
        try:
            data = json.loads(body)
            
            # Ler ID máximo
            max_id = 0
            if ORDENS_FILE.exists():
                with open(ORDENS_FILE, 'r', encoding='utf-8') as f:
                    for line in f:
                        if line.strip():
                            parts = line.strip().split('|')
                            max_id = max(max_id, int(parts[0]))
            
            new_id = max_id + 1
            
            # Escrever nova ordem
            with open(ORDENS_FILE, 'a', encoding='utf-8') as f:
                f.write(f"{new_id}|{data.get('fornecedor_id', '')}|{data.get('valor', '')}|"
                       f"{data.get('data', '')}|{data.get('status', 'Pendente')}|"
                       f"{data.get('descricao', '')}\n")
            
            return {
                'success': True,
                'message': 'Ordem de compra criada com sucesso',
                'id': new_id
            }
        except Exception as e:
            return {'success': False, 'error': str(e)}
    
    def log_message(self, format, *args):
        """Customizar log de requisições"""
        print(f"[{self.log_date_time_string()}] {format % args}")


def main():
    """Inicia o servidor HTTP"""
    PORT = 8080
    server_address = ('', PORT)
    httpd = HTTPServer(server_address, APIHandler)
    
    print(f"🚀 Servidor HTTP iniciando na porta {PORT}...")
    print(f"📍 API disponível em: http://localhost:{PORT}/api")
    print("\n📋 Endpoints disponíveis:")
    print("  ✓ GET  /api/fornecedores")
    print("  ✓ GET  /api/ordens")
    print("  ✓ GET  /api/estoque")
    print("  ✓ GET  /api/financeiro")
    print("  ✓ POST /api/fornecedores")
    print("  ✓ POST /api/ordens")
    print("  ✓ GET  /api/status")
    print("\n⏸️  Pressione Ctrl+C para parar o servidor\n")
    
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\n✋ Servidor interrompido pelo usuário")
        httpd.server_close()


if __name__ == '__main__':
    main()
