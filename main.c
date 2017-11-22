#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define TAMANHO_LISTA 5
#define NAO_ENCONTRADO -1

typedef struct SDivida {
    char nome[20];
    float valor;
} TDivida;

typedef struct SLista {
    int tamanho;
    int indice;
    TDivida ** elementos;
} TLista;

void pseparador() {
    printf("\n----------------------------\n");
}

void ptitulo(char * mensagem) {
    pseparador();
    printf("\n %s\n", mensagem);
    pseparador();
}

// Inicializa um registro de dívida
TDivida * divida_inicializar() {
    TDivida * divida = malloc(sizeof(TDivida));;

    divida->valor = 0;
    strcpy(divida->nome, "");

    return divida;
}

// Cadastra os dados de um registro de dívida
void divida_cadastrar(TDivida * divida) {

    if (strcmp(divida->nome, "") == 0) {
        printf("\nInforme o nome:\n");
        scanf(" %s", divida->nome);
    }

    printf("\nInforme o valor:\n");
    scanf(" %f", &divida->valor);
}

// Exibe os dados de uma dívida
void divida_exibir(TDivida * divida) {
    printf("\n\nNome: %s\n", divida->nome);
    printf("Valor: %.2f\n\n", divida->valor);
}

// Inicializa uma lista
TLista * lista_inicializar() {
    TLista * lista;

    lista = malloc(sizeof(TLista));
    lista->indice = 0;
    lista->tamanho = TAMANHO_LISTA;
    lista->elementos = malloc(sizeof(TDivida *) * lista->tamanho);
    if (lista->elementos == NULL) {
        exit(EXIT_FAILURE);
    }

    // Inicializa as posições do array de ponteiros
    for (int i = 0; i < lista->tamanho; i++) {
        lista->elementos[i] = NULL;
    }

    return lista;
}

// Pesquisa a posição de um registro
int lista_pesquisar_posicao(TLista * lista, char * nome) {
    for (int i = 0; i < lista->indice; i++) {
        if (strcmp(lista->elementos[i]->nome, nome) == 0) {
            return i;
        }
    }

    return NAO_ENCONTRADO;
}

// Pesquisa um registro
TDivida * lista_pesquisar(TLista * lista) {
    char nome[20];

    printf("Informe o nome a pesquisar:\n");
    scanf(" %s", nome);

    int posicao = lista_pesquisar_posicao(lista, nome);
    if (posicao != NAO_ENCONTRADO) {
        return lista->elementos[posicao];
    }

    return NULL;
}

// Adiciona um registro na primeira posição vazia encontrada ou no final da lista
void lista_adicionar(TLista * lista, TDivida * divida) {
    int posicaoVazia = NAO_ENCONTRADO;

    // Percorre a lista procurando uma posição vazia (caso algum elemento tenha sido removido)
    for (int posicaoAtual = 0; posicaoAtual < lista->indice; posicaoAtual++) {
        if (lista->elementos[posicaoAtual] == NULL) {
            posicaoVazia = posicaoAtual;
            break;
        }
    }

    // Caso tenha encontrado adiciona na posição vazia na lista
    if (posicaoVazia != NAO_ENCONTRADO) {
        lista->elementos[posicaoVazia] = divida;
    }
    // Se não encontratou uma posição vazia adiciona no final da lista
    else {
        lista->elementos[lista->indice] = divida;
        lista->indice++;
    }

    // Verifica se após a inserção é necessário alocar mais memória
    if (lista->indice == lista->tamanho) {
        lista->tamanho += TAMANHO_LISTA;
        lista->elementos = realloc(lista->elementos, sizeof(TDivida *) * lista->tamanho);
        if (lista->elementos == NULL) {
            exit(EXIT_FAILURE);
        }

        // Inicializa as novas posições do array
        for (int i = lista->tamanho - TAMANHO_LISTA; i < lista->tamanho; i++) {
            lista->elementos[i] = NULL;
        }
    }
}

// Exibe a lista
void lista_exibir(TLista * lista) {
    for (int i = 0; i < lista->indice; i++) {
        // Desconsidera uma posição vazia onde um elemento foi removido
        if (lista->elementos[i] == NULL) {
            continue;
        }
        divida_exibir(lista->elementos[i]);
    }
}

// Remove um registro da lista
bool lista_remover(TLista * lista) {
    char nome[20];

    printf("Informe o nome a remover:\n");
    scanf(" %s", nome);

    int posicao = lista_pesquisar_posicao(lista, nome);
    if (posicao != NAO_ENCONTRADO) {
        // Remove o elemento da lista, apontando para NULL
        lista->elementos[posicao] = NULL;
        return true;
    }

    return false;
}

// Libera a memória alocada para a lista
void lista_limpar(TLista * lista) {
    free(lista->elementos);
}

// Verifica se a lista está vazia, retornando verdadeiro caso esteja
bool lista_vazia(TLista * lista) {
    for (int i = 0; i < lista->tamanho; i++) {
        if (lista->elementos[i] != NULL) {
            return false;
        }
    }

    return true;
}

int main(void) {
    TLista * lista;
    TDivida * divida;
    int operacao;

    lista = lista_inicializar();

    do {
        operacao = -1;

        system("clear");
        ptitulo("Sistema de Cobrança");

        printf("\nDívidas \n");
        printf("1 - Cadastrar \n");
        printf("2 - Atualizar \n");
        printf("3 - Remover \n");
        printf("4 - Listar \n");
        printf("5 - Pesquisar \n\n");

        printf("0 - Sair\n");
        scanf(" %d", &operacao);

        switch (operacao) {

            case 1:

                ptitulo("Cadastrar Dívida");
                divida = divida_inicializar();
                divida_cadastrar(divida);
                if (lista_pesquisar_posicao(lista, divida->nome) != NAO_ENCONTRADO) {
                    printf("\nJá existe uma dívida cadastrada para essa pessoa. Cadastro cancelado.\n");
                    break;
                }
                lista_adicionar(lista, divida);

                pseparador();
                break;

            case 2:

                ptitulo("Atualizar Dívida");
                if(lista_vazia(lista)) {
                    printf("\nLista vazia.\n");
                    break;
                }

                divida = lista_pesquisar(lista);
                if (divida != NULL) {
                    divida_cadastrar(divida);
                } else {
                    printf("\nDívida não encontrada.\n");
                }

                pseparador();
                break;

            case 3:

                ptitulo("Remover Dívida");
                if(lista_vazia(lista)) {
                    printf("\nLista vazia.\n");
                    break;
                }

                if (lista_remover(lista)) {
                    printf("\nDívida removida.\n");
                } else {
                    printf("\nDívida não encontrada.\n");
                }

                pseparador();
                break;

            case 4:

                ptitulo("Listar Dívidas");
                if(lista_vazia(lista)) {
                    printf("\nLista vazia.\n");
                    break;
                }

                lista_exibir(lista);

                pseparador();
                break;

            case 5:

                ptitulo("Pesquisar Dívida");
                if(lista_vazia(lista)) {
                    printf("\nLista vazia.\n");
                    break;
                }

                divida = lista_pesquisar(lista);
                if (divida != NULL) {
                    divida_exibir(divida);
                } else {
                    printf("\nDívida não encontrada.\n");
                }

                pseparador();
                break;

            case 0:
                break;

            default:
                printf("\nA opção informada é inválida.\n\n");
        }

        if (operacao != 0) {
            printf("\n\n");
            system("echo 'Pressione ENTER para voltar ao menu.'; read x");
        }

    } while (operacao != 0);

    lista_limpar(lista);

    printf("Tchau :D\n");
    return EXIT_SUCCESS;
}
