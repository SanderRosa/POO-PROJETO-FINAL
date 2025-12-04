/* ============================================================================
   SISTEMA DE GERENCIAMENTO DE COMPRAS - JAVASCRIPT
   ============================================================================ */

// Dados simulados
let fornecedores = [];
let ordens = [];
let proximoIdFornecedor = 1;
let proximoIdOrdem = 1;

// ============================================================================
// NAVEGAÇÃO ENTRE SEÇÕES
// ============================================================================

function showSection(sectionId) {
    // Ocultar todas as seções
    const sections = document.querySelectorAll('.section');
    sections.forEach(section => {
        section.classList.remove('active');
    });

    // Mostrar seção selecionada
    document.getElementById(sectionId).classList.add('active');

    // Atualizar links de navegação
    const navLinks = document.querySelectorAll('.nav-link');
    navLinks.forEach(link => {
        link.classList.remove('active');
        if (link.getAttribute('href') === `#${sectionId}`) {
            link.classList.add('active');
        }
    });
}

// ============================================================================
// GERENCIAMENTO DE FORNECEDORES
// ============================================================================

document.addEventListener('DOMContentLoaded', function() {
    // Form de Fornecedor
    const fornecedorForm = document.getElementById('fornecedorForm');
    if (fornecedorForm) {
        fornecedorForm.addEventListener('submit', function(e) {
            e.preventDefault();
            adicionarFornecedor();
        });
    }

    // Form de Ordem
    const ordemForm = document.getElementById('ordemForm');
    if (ordemForm) {
        ordemForm.addEventListener('submit', function(e) {
            e.preventDefault();
            criarOrdem();
        });
    }

    // Form de Reserva
    const reservaForm = document.getElementById('reservaForm');
    if (reservaForm) {
        reservaForm.addEventListener('submit', function(e) {
            e.preventDefault();
            reservarMaterial();
        });
    }

    // Form de Pedido
    const pedidoForm = document.getElementById('pedidoForm');
    if (pedidoForm) {
        pedidoForm.addEventListener('submit', function(e) {
            e.preventDefault();
            criarPedido();
        });
    }

    // Busca e Filtro de Fornecedores
    const searchFornecedor = document.getElementById('searchFornecedor');
    const filterProduto = document.getElementById('filterProduto');
    if (searchFornecedor) {
        searchFornecedor.addEventListener('input', function() {
            filtrarFornecedores();
        });
    }
    if (filterProduto) {
        filterProduto.addEventListener('change', function() {
            filtrarFornecedores();
        });
    }

    // Busca e Filtro de Ordens
    const searchOrdem = document.getElementById('searchOrdem');
    const filterStatus = document.getElementById('filterStatus');
    if (searchOrdem) {
        searchOrdem.addEventListener('input', function() {
            filtrarOrdens();
        });
    }
    if (filterStatus) {
        filterStatus.addEventListener('change', function() {
            filtrarOrdens();
        });
    }

    // Carregar dados iniciais (simulados)
    carregarDadosIniciais();
    atualizarSelectFornecedores();
});

function adicionarFornecedor() {
    const nome = document.getElementById('nome').value;
    const endereco = document.getElementById('endereco').value;
    const cnpj = document.getElementById('cnpj').value;
    const produto = document.getElementById('produto').value;
    const preco = parseFloat(document.getElementById('preco').value);

    if (!nome || !endereco || !cnpj || !produto || !preco) {
        alert('Por favor, preencha todos os campos');
        return;
    }

    const fornecedor = {
        id: proximoIdFornecedor++,
        nome,
        endereco,
        cnpj,
        produto,
        preco
    };

    fornecedores.push(fornecedor);
    atualizarTabelaFornecedores();
    atualizarSelectFornecedores();

    // Limpar form
    document.getElementById('fornecedorForm').reset();
    mostrarNotificacao(`Fornecedor "${nome}" adicionado com sucesso!`, 'success');
}

function atualizarTabelaFornecedores() {
    const tbody = document.getElementById('fornecedorTable');
    
    if (fornecedores.length === 0) {
        tbody.innerHTML = '<tr class="placeholder"><td colspan="7">Nenhum fornecedor cadastrado</td></tr>';
        return;
    }

    tbody.innerHTML = fornecedores.map(f => `
        <tr>
            <td>#${f.id}</td>
            <td>${f.nome}</td>
            <td>${f.cnpj}</td>
            <td>${f.produto}</td>
            <td>R$ ${f.preco.toFixed(2)}</td>
            <td>${f.endereco}</td>
            <td>
                <button class="btn btn-sm btn-info" onclick="editarFornecedor(${f.id})">Editar</button>
                <button class="btn btn-sm btn-danger" onclick="removerFornecedor(${f.id})">Remover</button>
            </td>
        </tr>
    `).join('');
}

function filtrarFornecedores() {
    const searchTerm = document.getElementById('searchFornecedor').value.toLowerCase();
    const filterProduto = document.getElementById('filterProduto').value;

    const tbody = document.getElementById('fornecedorTable');
    const rows = Array.from(tbody.querySelectorAll('tr')).filter(row => {
        if (row.classList.contains('placeholder')) return false;
        
        const texto = row.textContent.toLowerCase();
        const matchSearch = texto.includes(searchTerm);
        const matchProduto = !filterProduto || row.textContent.includes(filterProduto);
        
        return matchSearch && matchProduto;
    });

    if (rows.length === 0) {
        tbody.innerHTML = '<tr class="placeholder"><td colspan="7">Nenhum fornecedor encontrado</td></tr>';
        return;
    }

    tbody.innerHTML = '';
    rows.forEach(row => tbody.appendChild(row.cloneNode(true)));
}

function removerFornecedor(id) {
    if (!confirm('Tem certeza que deseja remover este fornecedor?')) return;
    
    fornecedores = fornecedores.filter(f => f.id !== id);
    atualizarTabelaFornecedores();
    atualizarSelectFornecedores();
    mostrarNotificacao('Fornecedor removido com sucesso!', 'success');
}

function editarFornecedor(id) {
    alert('Funcionalidade de edição em desenvolvimento');
}

function atualizarSelectFornecedores() {
    const select = document.getElementById('idFornecedor');
    if (!select) return;
    
    const html = '<option value="">Selecione um fornecedor...</option>' + 
        fornecedores.map(f => `<option value="${f.id}">${f.nome} - ${f.produto}</option>`).join('');
    select.innerHTML = html;
}

// ============================================================================
// GERENCIAMENTO DE ORDENS
// ============================================================================

function criarOrdem() {
    const idFornecedor = parseInt(document.getElementById('idFornecedor').value);
    const idItem = parseInt(document.getElementById('idItem').value);
    const quantidade = parseInt(document.getElementById('quantidade').value);
    const valorUnitario = parseFloat(document.getElementById('valorUnitario').value);

    if (!idFornecedor || !idItem || !quantidade || !valorUnitario) {
        alert('Por favor, preencha todos os campos');
        return;
    }

    const fornecedor = fornecedores.find(f => f.id === idFornecedor);
    if (!fornecedor) {
        alert('Fornecedor não encontrado');
        return;
    }

    const ordem = {
        id: proximoIdOrdem++,
        idItem,
        quantidade,
        valorUnitario,
        idFornecedor,
        fornecedorNome: fornecedor.nome,
        status: 'PENDENTE',
        data: new Date().toLocaleDateString('pt-BR'),
        valorTotal: quantidade * valorUnitario
    };

    ordens.push(ordem);
    atualizarTabelaOrdens();

    // Limpar form
    document.getElementById('ordemForm').reset();
    mostrarNotificacao('Ordem de compra criada com sucesso!', 'success');
}

function atualizarTabelaOrdens() {
    const tbody = document.getElementById('ordemTable');
    
    if (ordens.length === 0) {
        tbody.innerHTML = '<tr class="placeholder"><td colspan="9">Nenhuma ordem cadastrada</td></tr>';
        return;
    }

    tbody.innerHTML = ordens.map(o => `
        <tr>
            <td>#${o.id}</td>
            <td>${o.idItem}</td>
            <td>${o.quantidade}</td>
            <td>R$ ${o.valorUnitario.toFixed(2)}</td>
            <td>R$ ${o.valorTotal.toFixed(2)}</td>
            <td>${o.fornecedorNome}</td>
            <td>${getStatusBadge(o.status)}</td>
            <td>${o.data}</td>
            <td>
                <button class="btn btn-sm btn-info" onclick="detalheOrdem(${o.id})">Detalhes</button>
            </td>
        </tr>
    `).join('');
}

function filtrarOrdens() {
    const searchTerm = document.getElementById('searchOrdem').value.toLowerCase();
    const filterStatus = document.getElementById('filterStatus').value;

    const tbody = document.getElementById('ordemTable');
    const rows = Array.from(tbody.querySelectorAll('tr')).filter(row => {
        if (row.classList.contains('placeholder')) return false;
        
        const texto = row.textContent.toLowerCase();
        const matchSearch = texto.includes(searchTerm);
        const matchStatus = !filterStatus || row.textContent.includes(filterStatus);
        
        return matchSearch && matchStatus;
    });

    if (rows.length === 0) {
        tbody.innerHTML = '<tr class="placeholder"><td colspan="9">Nenhuma ordem encontrada</td></tr>';
        return;
    }

    tbody.innerHTML = '';
    rows.forEach(row => tbody.appendChild(row.cloneNode(true)));
}

function detalheOrdem(id) {
    const ordem = ordens.find(o => o.id === id);
    if (ordem) {
        alert(`
Ordem #${ordem.id}
Item: ${ordem.idItem}
Quantidade: ${ordem.quantidade}
Valor Total: R$ ${ordem.valorTotal.toFixed(2)}
Fornecedor: ${ordem.fornecedorNome}
Status: ${ordem.status}
Data: ${ordem.data}
        `);
    }
}

function getStatusBadge(status) {
    const badgeClass = {
        'PENDENTE': 'badge-warning',
        'APROVADO': 'badge-success',
        'REJEITADO': 'badge-danger',
        'ENVIADO': 'badge-info',
        'ENTREGUE': 'badge-success'
    };
    
    return `<span class="badge ${badgeClass[status] || 'badge-info'}">${status}</span>`;
}

// ============================================================================
// GERENCIAMENTO DE ESTOQUE
// ============================================================================

function reservarMaterial() {
    const idMaterial = document.getElementById('idMaterialReserva').value;
    const quantidade = parseInt(document.getElementById('qtdReserva').value);

    if (!idMaterial || !quantidade) {
        alert('Por favor, preencha todos os campos');
        return;
    }

    mostrarNotificacao(`${quantidade} unidades de material #${idMaterial} reservadas com sucesso!`, 'success');
    document.getElementById('reservaForm').reset();
}

// ============================================================================
// GERENCIAMENTO DE PRODUÇÃO
// ============================================================================

function criarPedido() {
    const idMaterial = document.getElementById('idMaterialPedido').value;
    const quantidade = parseInt(document.getElementById('qtdPedido').value);
    const prioridade = document.getElementById('prioridade').value;

    if (!idMaterial || !quantidade || !prioridade) {
        alert('Por favor, preencha todos os campos');
        return;
    }

    mostrarNotificacao(`Pedido de material criado com sucesso!`, 'success');
    document.getElementById('pedidoForm').reset();
}

// ============================================================================
// DADOS INICIAIS
// ============================================================================

function carregarDadosIniciais() {
    // Fornecedores iniciais
    fornecedores = [
        {
            id: 1,
            nome: 'Aços Brasil Ltda',
            endereco: 'São Paulo - SP',
            cnpj: '01.234.567/0001-89',
            produto: 'Aço Inox',
            preco: 150.00
        },
        {
            id: 2,
            nome: 'Parafusos do Brasil',
            endereco: 'Blumenau - SC',
            cnpj: '02.345.678/0001-90',
            produto: 'Parafusos M10',
            preco: 5.50
        },
        {
            id: 3,
            nome: 'Indústria de Borracha',
            endereco: 'Rio de Janeiro - RJ',
            cnpj: '03.456.789/0001-91',
            produto: 'Borracha Industrial',
            preco: 85.00
        }
    ];
    proximoIdFornecedor = 4;

    atualizarTabelaFornecedores();
    atualizarSelectFornecedores();
}

// ============================================================================
// NOTIFICAÇÕES
// ============================================================================

function mostrarNotificacao(mensagem, tipo = 'info') {
    // Criar elemento de notificação
    const notificacao = document.createElement('div');
    notificacao.className = `notificacao notificacao-${tipo}`;
    notificacao.textContent = mensagem;
    notificacao.style.cssText = `
        position: fixed;
        top: 20px;
        right: 20px;
        padding: 15px 20px;
        border-radius: 4px;
        color: white;
        font-weight: 600;
        z-index: 1000;
        animation: slideIn 0.3s ease;
        max-width: 400px;
    `;

    // Definir cores de fundo
    const cores = {
        success: '#27ae60',
        danger: '#e74c3c',
        warning: '#f39c12',
        info: '#3498db'
    };
    notificacao.style.backgroundColor = cores[tipo] || cores.info;

    document.body.appendChild(notificacao);

    // Remover após 3 segundos
    setTimeout(() => {
        notificacao.style.animation = 'slideOut 0.3s ease';
        setTimeout(() => notificacao.remove(), 300);
    }, 3000);
}

// Adicionar animações CSS dinamicamente
const style = document.createElement('style');
style.textContent = `
    @keyframes slideIn {
        from {
            transform: translateX(400px);
            opacity: 0;
        }
        to {
            transform: translateX(0);
            opacity: 1;
        }
    }

    @keyframes slideOut {
        from {
            transform: translateX(0);
            opacity: 1;
        }
        to {
            transform: translateX(400px);
            opacity: 0;
        }
    }
`;
document.head.appendChild(style);
