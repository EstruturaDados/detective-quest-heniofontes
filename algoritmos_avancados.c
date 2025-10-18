#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

// 🌱 Nível Novato: Mapa da Mansão com Árvore Binária

// Struct para representar uma sala na mansão
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
    char pista[100]; // Pista associada a esta sala
    int visitada;    // Controle se a sala já foi visitada
} Sala;

// 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca

// Struct para representar uma pista na árvore BST
typedef struct Pista {
    char texto[100];
    struct Pista *esquerda;
    struct Pista *direita;
} Pista;

// 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash

// Struct para entrada na tabela hash (pista -> suspeito)
typedef struct EntradaHash {
    char pista[100];
    char suspeito[50];
    struct EntradaHash *proximo; // Para tratamento de colisões
} EntradaHash;

// Tabela hash para associações pista-suspeito
#define TAMANHO_HASH 50
EntradaHash* tabelaHash[TAMANHO_HASH];

// Variáveis globais para controle do jogo
Pista* arvorePistas = NULL;
int totalPistasColetadas = 0;

/**
 * Função: criarSala
 * Objetivo: Cria dinamicamente um cômodo da mansão
 * Parâmetros: nome da sala e pista associada
 * Retorno: Ponteiro para a sala criada
 */
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    novaSala->visitada = 0;
    return novaSala;
}

/**
 * Função: conectarSalas
 * Objetivo: Conectar salas para formar a árvore binária
 */
void conectarSalas(Sala* pai, Sala* esq, Sala* dir) {
    if (pai) {
        pai->esquerda = esq;
        pai->direita = dir;
    }
}

/**
 * Função: construirMansao
 * Objetivo: Construir o mapa fixo da mansão
 */
Sala* construirMansao() {
    // Criar todas as salas com suas pistas
    Sala* hall = criarSala("Hall de Entrada", "Porta principal arrombada");
    Sala* biblioteca = criarSala("Biblioteca", "Livro raro desaparecido");
    Sala* cozinha = criarSala("Cozinha", "Faca com manchas suspeitas");
    Sala* salaEstar = criarSala("Sala de Estar", "Cortina rasgada na luta");
    Sala* quarto = criarSala("Quarto Principal", "Joias desaparecidas");
    Sala* sotao = criarSala("Sotao", "Baú antigo violado");
    Sala* jardim = criarSala("Jardim", "Pegadas na terra molhada");
    Sala* escritorio = criarSala("Escritorio", "Documento importante roubado");
    Sala* lavanderia = criarSala("Lavanderia", "Manchas de sangue no uniforme");
    Sala* garage = criarSala("Garagem", "Ferramenta usada no crime");
    
    // Conectar as salas conforme o mapa
    conectarSalas(hall, biblioteca, cozinha);
    conectarSalas(biblioteca, salaEstar, quarto);
    conectarSalas(cozinha, jardim, sotao);
    conectarSalas(salaEstar, escritorio, lavanderia);
    conectarSalas(quarto, garage, NULL);
    
    return hall;
}

/**
 * Função: inserirPista
 * Objetivo: Insere a pista coletada na árvore de pistas (BST)
 * Parâmetros: raiz da árvore e texto da pista
 * Retorno: Nova raiz da árvore
 */
Pista* inserirPista(Pista* raiz, const char* texto) {
    if (raiz == NULL) {
        Pista* novaPista = (Pista*)malloc(sizeof(Pista));
        strcpy(novaPista->texto, texto);
        novaPista->esquerda = NULL;
        novaPista->direita = NULL;
        return novaPista;
    }
    
    int comparacao = strcmp(texto, raiz->texto);
    
    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, texto);
    }
    // Se for igual, não insere duplicata
    
    return raiz;
}

/**
 * Função: inicializarHash
 * Objetivo: Inicializar a tabela hash
 */
void inicializarHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabelaHash[i] = NULL;
    }
}

/**
 * Função: hash
 * Objetivo: Calcular índice hash para uma pista
 */
int hash(const char* pista) {
    int soma = 0;
    for (int i = 0; pista[i] != '\0'; i++) {
        soma += pista[i];
    }
    return soma % TAMANHO_HASH;
}

/**
 * Função: inserirNaHash
 * Objetivo: Insere associação pista/suspeito na tabela hash
 * Parâmetros: pista e suspeito associado
 */
void inserirNaHash(const char* pista, const char* suspeito) {
    int indice = hash(pista);
    EntradaHash* novaEntrada = (EntradaHash*)malloc(sizeof(EntradaHash));
    
    strcpy(novaEntrada->pista, pista);
    strcpy(novaEntrada->suspeito, suspeito);
    novaEntrada->proximo = tabelaHash[indice];
    tabelaHash[indice] = novaEntrada;
}

/**
 * Função: encontrarSuspeito
 * Objetivo: Consulta o suspeito correspondente a uma pista
 * Parâmetros: pista a ser consultada
 * Retorno: Nome do suspeito ou "Desconhecido" se não encontrado
 */
const char* encontrarSuspeito(const char* pista) {
    int indice = hash(pista);
    EntradaHash* atual = tabelaHash[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    
    return "Desconhecido";
}

/**
 * Função: popularTabelaHash
 * Objetivo: Popular a tabela hash com as associações pista-suspeito
 */
void popularTabelaHash() {
    // Associações pré-definidas entre pistas e suspeitos
    inserirNaHash("Porta principal arrombada", "Joao");
    inserirNaHash("Livro raro desaparecido", "Maria");
    inserirNaHash("Faca com manchas suspeitas", "Carlos");
    inserirNaHash("Cortina rasgada na luta", "Ana");
    inserirNaHash("Joias desaparecidas", "Pedro");
    inserirNaHash("Baú antigo violado", "Joao");
    inserirNaHash("Pegadas na terra molhada", "Carlos");
    inserirNaHash("Documento importante roubado", "Maria");
    inserirNaHash("Manchas de sangue no uniforme", "Ana");
    inserirNaHash("Ferramenta usada no crime", "Pedro");
}

/**
 * Função: emOrdem
 * Objetivo: Percorrer a árvore de pistas em ordem (para exibição ordenada)
 */
void emOrdem(Pista* raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda);
        const char* suspeito = encontrarSuspeito(raiz->texto);
        printf("- %s -> Suspeito: %s\n", raiz->texto, suspeito);
        emOrdem(raiz->direita);
    }
}

/**
 * Função: listarPistasColetadas
 * Objetivo: Listar todas as pistas coletadas em ordem alfabética
 */
void listarPistasColetadas() {
    printf("\n=== PISTAS COLETADAS (Ordem Alfabetica) ===\n");
    if (arvorePistas == NULL) {
        printf("Nenhuma pista coletada ainda.\n");
    } else {
        emOrdem(arvorePistas);
    }
    printf("Total de pistas: %d\n", totalPistasColetadas);
    printf("==========================================\n");
}

/**
 * Função: contarPistasPorSuspeito
 * Objetivo: Contar quantas pistas apontam para cada suspeito
 */
int contarPistasPorSuspeito(const char* suspeito) {
    int contador = 0;
    
    for (int i = 0; i < TAMANHO_HASH; i++) {
        EntradaHash* atual = tabelaHash[i];
        while (atual != NULL) {
            // Verificar se esta pista está na árvore de pistas coletadas
            Pista* pistaAtual = arvorePistas;
            while (pistaAtual != NULL) {
                if (strcmp(pistaAtual->texto, atual->pista) == 0 && 
                    strcmp(atual->suspeito, suspeito) == 0) {
                    contador++;
                    break;
                }
                // Buscar na BST
                int comparacao = strcmp(atual->pista, pistaAtual->texto);
                if (comparacao < 0) {
                    pistaAtual = pistaAtual->esquerda;
                } else {
                    pistaAtual = pistaAtual->direita;
                }
            }
            atual = atual->proximo;
        }
    }
    
    return contador;
}

/**
 * Função: explorarSalas
 * Objetivo: Navega pela árvore e ativa o sistema de pistas
 * Parâmetros: Sala inicial para exploração
 */
void explorarSalas(Sala* salaAtual) {
    char movimento;
    
    printf("\n=== DETECTIVE QUEST - EXPLORANDO A MANSAO ===\n");
    printf("Instrucoes: (e) Esquerda, (d) Direita, (s) Sair\n");
    
    while (salaAtual != NULL) {
        printf("\nVoce esta na: %s\n", salaAtual->nome);
        
        // Coletar pista se a sala não foi visitada ainda
        if (!salaAtual->visitada && strlen(salaAtual->pista) > 0) {
            printf("Pista encontrada: %s\n", salaAtual->pista);
            
            // Adicionar à árvore de pistas
            arvorePistas = inserirPista(arvorePistas, salaAtual->pista);
            totalPistasColetadas++;
            
            const char* suspeito = encontrarSuspeito(salaAtual->pista);
            printf("   Suspeito associado: %s\n", suspeito);
            
            salaAtual->visitada = 1;
        } else if (salaAtual->visitada) {
            printf("Esta sala ja foi investigada.\n");
        }
        
        printf("\nOpcoes: ");
        if (salaAtual->esquerda) printf("(e) %s <- ", salaAtual->esquerda->nome);
        if (salaAtual->direita) printf("(d) %s -> ", salaAtual->direita->nome);
        printf("(s) Sair da exploracao\n");
        
        printf("Sua escolha: ");
        scanf(" %c", &movimento);
        
        switch (movimento) {
            case 'e':
            case 'E':
                if (salaAtual->esquerda) {
                    salaAtual = salaAtual->esquerda;
                } else {
                    printf("Nao ha sala a esquerda!\n");
                }
                break;
            case 'd':
            case 'D':
                if (salaAtual->direita) {
                    salaAtual = salaAtual->direita;
                } else {
                    printf("Nao ha sala a direita!\n");
                }
                break;
            case 's':
            case 'S':
                printf("Saindo da exploracao...\n");
                return;
            default:
                printf("Opcao invalida! Use e, d ou s.\n");
        }
    }
}

/**
 * Função: verificarSuspeitoFinal
 * Objetivo: Conduz à fase de julgamento final e verifica a acusação
 */
void verificarSuspeitoFinal() {
    char suspeitoAcusado[50];
    
    printf("\n=== FASE DE JULGAMENTO FINAL ===\n");
    printf("Lista de suspeitos: Joao, Maria, Carlos, Ana, Pedro\n");
    
    listarPistasColetadas();
    
    printf("\nCom base nas pistas coletadas, quem voce acusa?\n");
    printf("Digite o nome do suspeito: ");
    scanf("%s", suspeitoAcusado);
    
    // Contar quantas pistas apontam para o suspeito acusado
    int pistasAssociadas = contarPistasPorSuspeito(suspeitoAcusado);
    
    printf("\n=== VERDICT FINAL ===\n");
    printf("Suspeito acusado: %s\n", suspeitoAcusado);
    printf("Pistas que apontam para %s: %d\n", suspeitoAcusado, pistasAssociadas);
    
    if (pistasAssociadas >= 2) {
        printf("CONCLUSÃO: A acusação é sustentada pelas evidências!\n");
        printf("   %s é considerado CULPADO com base nas pistas coletadas!\n", suspeitoAcusado);
    } else {
        printf("CONCLUSÃO: Evidências insuficientes para sustentar a acusação.\n");
        printf("   %s é considerado INOCENTE por falta de provas concretas.\n", suspeitoAcusado);
    }
    
    printf("\nFim do caso. Detective Quest encerrado!\n");
}

/**
 * Função: menuPrincipal
 * Objetivo: Controlar o fluxo principal do jogo
 */
void menuPrincipal() {
    Sala* mansao = construirMansao();
    inicializarHash();
    popularTabelaHash();
    
    int opcao;
    
    printf("Bem-vindo ao DETECTIVE QUEST!\n");
    printf("Resolva o misterio explorando a mansao e coletando pistas.\n");
    
    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Explorar Mansao\n");
        printf("2. Listar Pistas Coletadas\n");
        printf("3. Fase de Julgamento Final\n");
        printf("4. Sair do Jogo\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                explorarSalas(mansao);
                break;
            case 2:
                listarPistasColetadas();
                break;
            case 3:
                if (totalPistasColetadas > 0) {
                    verificarSuspeitoFinal();
                    opcao = 4; // Encerra o jogo após o julgamento
                } else {
                    printf("Voce precisa coletar pelo menos uma pista antes do julgamento!\n");
                }
                break;
            case 4:
                printf("Obrigado por jogar Detective Quest!\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 4);
}

int main() {
    menuPrincipal();
    return 0;
}
