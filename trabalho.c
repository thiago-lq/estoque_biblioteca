#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <stdbool.h>

#define SIZE 10
#define MAX_STR 100
#define LOAD_FACTOR_THRESHOLD 0.7

typedef struct Livro {
    int id;
    char titulo[MAX_STR];
    char autor[MAX_STR];
    int ano;
    char genero[MAX_STR];
    int disponivel;
    struct Livro* next;
} Livro;

Livro* hashTable[SIZE] = {NULL};
int itemCount = 0;
int* idsLivres = NULL;
int qtdIdsLivres = 0;
int proximoId = 1;

bool validarNumero(const char* str);
bool validarTexto(const char* str);
int lerInteiro(const char* prompt);
void lerString(const char* prompt, char* buffer, int tamanho);

unsigned long hash(int id) {
    return id % SIZE;
}

int needsResizing() {
    return ((double)itemCount / SIZE) >= LOAD_FACTOR_THRESHOLD;
}

int obterIdDisponivel() {
    if (qtdIdsLivres > 0) {
        int id = idsLivres[qtdIdsLivres - 1];
        qtdIdsLivres--;
        idsLivres = realloc(idsLivres, qtdIdsLivres * sizeof(int));
        return id;
    }
    return proximoId++;
}

void liberarId(int id) {
    qtdIdsLivres++;
    idsLivres = realloc(idsLivres, qtdIdsLivres * sizeof(int));
    idsLivres[qtdIdsLivres - 1] = id;
}

void inserirLivro() {
    Livro* novo = (Livro*)malloc(sizeof(Livro));
    if (novo == NULL) {
        printf("Erro ao alocar memória.\n");
        return;
    }

    novo->id = obterIdDisponivel();

    printf("\n--- Inserir Novo Livro (ID: %d) ---\n", novo->id);

    lerString("Título: ", novo->titulo, MAX_STR);
    lerString("Autor: ", novo->autor, MAX_STR);

    novo->ano = lerInteiro("Ano: ");
    while (novo->ano <= 0 || novo->ano > 2023) {
        printf("Ano inválido! Digite um ano entre 1 e 2023: ");
        novo->ano = lerInteiro("");
    }

    lerString("Gênero: ", novo->genero, MAX_STR);

    novo->disponivel = 1;

    int index = hash(novo->id);
    novo->next = hashTable[index];
    hashTable[index] = novo;
    itemCount++;

    printf("Livro cadastrado com sucesso! ID: %d\n", novo->id);
}

void removerLivro() {
    int id = lerInteiro("\n--- Remover Livro ---\nID do livro a ser removido: ");

    int index = hash(id);
    Livro* atual = hashTable[index];
    Livro* anterior = NULL;
    int encontrado = 0;

    while (atual != NULL) {
        if (atual->id == id) {
            if (anterior == NULL) {
                hashTable[index] = atual->next;
            } else {
                anterior->next = atual->next;
            }

            liberarId(atual->id);

            free(atual);
            itemCount--;
            printf("Livro removido com sucesso.\n");
            encontrado = 1;
            break;
        }
        anterior = atual;
        atual = atual->next;
    }

    if (!encontrado) {
        printf("Livro com ID %d não encontrado.\n", id);
    }
}

void emprestarLivro() {
    int id = lerInteiro("\n--- Emprestar Livro ---\nID do livro a ser emprestado: ");

    int index = hash(id);
    Livro* atual = hashTable[index];
    int encontrado = 0;

    while (atual != NULL) {
        if (atual->id == id) {
            if (atual->disponivel) {
                atual->disponivel = 0;
                printf("Livro '%s' emprestado com sucesso.\n", atual->titulo);
            } else {
                printf("Livro '%s' já está emprestado.\n", atual->titulo);
            }
            encontrado = 1;
            break;
        }
        atual = atual->next;
    }

    if (!encontrado) {
        printf("Livro com ID %d não encontrado.\n", id);
    }
}

void devolverLivro() {
    int id = lerInteiro("\n--- Devolver Livro ---\nID do livro a ser devolvido: ");

    int index = hash(id);
    Livro* atual = hashTable[index];
    int encontrado = 0;

    while (atual != NULL) {
        if (atual->id == id) {
            if (!atual->disponivel) {
                atual->disponivel = 1;
                printf("Livro '%s' devolvido com sucesso.\n", atual->titulo);
            } else {
                printf("Livro '%s' já está disponível.\n", atual->titulo);
            }
            encontrado = 1;
            break;
        }
        atual = atual->next;
    }

    if (!encontrado) {
        printf("Livro com ID %d não encontrado.\n", id);
    }
}

void buscarLivro() {
    int id = lerInteiro("\n--- Buscar Livro ---\nID do livro: ");

    int index = hash(id);
    Livro* atual = hashTable[index];
    int encontrado = 0;

    while (atual != NULL) {
        if (atual->id == id) {
            printf("\nDetalhes do Livro:\n");
            printf("ID: %d\n", atual->id);
            printf("Título: %s\n", atual->titulo);
            printf("Autor: %s\n", atual->autor);
            printf("Ano: %d\n", atual->ano);
            printf("Gênero: %s\n", atual->genero);
            printf("Status: %s\n", atual->disponivel ? "Disponível" : "Emprestado");
            encontrado = 1;
            break;
        }
        atual = atual->next;
    }

    if (!encontrado) {
        printf("Livro com ID %d não encontrado.\n", id);
    }
}

void listarLivros() {
    printf("\n--- Lista de Livros (%d cadastrados) ---\n", itemCount);
    for (int i = 0; i < SIZE; i++) {
        Livro* atual = hashTable[i];
        while (atual != NULL) {
            printf("\nID: %d\n", atual->id);
            printf("Título: %s\n", atual->titulo);
            printf("Autor: %s\n", atual->autor);
            printf("Ano: %d\n", atual->ano);
            printf("Gênero: %s\n", atual->genero);
            printf("Status: %s\n", atual->disponivel ? "Disponível" : "Emprestado");
            printf("-----------------------\n");
            atual = atual->next;
        }
    }
}

void exibirTabelaHash() {
    printf("\n--- Estrutura da Tabela Hash ---\n");
    for (int i = 0; i < SIZE; i++) {
        printf("Índice %d: ", i);
        Livro* atual = hashTable[i];
        if (atual == NULL) {
            printf("vazio\n");
        } else {
            while (atual != NULL) {
                printf("[ID: %d, Título: %s] -> ", atual->id, atual->titulo);
                atual = atual->next;
            }
            printf("NULL\n");
        }
    }
}

void limparBiblioteca() {
    for (int i = 0; i < SIZE; i++) {
        Livro* atual = hashTable[i];
        while (atual != NULL) {
            Livro* temp = atual;
            atual = atual->next;
            free(temp);
        }
        hashTable[i] = NULL;
    }
    itemCount = 0;
    free(idsLivres);
    idsLivres = NULL;
    qtdIdsLivres = 0;
    proximoId = 1;
    printf("Biblioteca limpa com sucesso.\n");
}

bool validarNumero(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

bool validarTexto(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i]) && !isspace(str[i]) && str[i] != '-') {
            return false;
        }
    }
    return true;
}

int lerInteiro(const char* prompt) {
    char buffer[MAX_STR];
    int numero;

    while (1) {
        printf("%s", prompt);
        fgets(buffer, MAX_STR, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (validarNumero(buffer)) {
            numero = atoi(buffer);
            return numero;
        }
        printf("Entrada inválida! Digite apenas números.\n");
    }
}

void lerString(const char* prompt, char* buffer, int tamanho) {
    while (1) {
        printf("%s", prompt);
        fgets(buffer, tamanho, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strlen(buffer) > 0) {
            int j = 0;
            bool espacoAnterior = false;
            for (int i = 0; buffer[i] != '\0'; i++) {
                if (isspace(buffer[i])) {
                    if (!espacoAnterior) {
                        buffer[j++] = buffer[i];
                        espacoAnterior = true;
                    }
                } else {
                    buffer[j++] = buffer[i];
                    espacoAnterior = false;
                }
            }
            buffer[j] = '\0';

            if (j > 0) {
                buffer[0] = toupper(buffer[0]);
                for (int i = 1; buffer[i] != '\0'; i++) {
                    buffer[i] = tolower(buffer[i]);
                }
            }

            return;
        }
        printf("Entrada inválida! Digite um texto válido.\n");
    }
}

void menu() {
    setlocale(LC_ALL, "Portuguese");
    int opcao;

    do {
        printf("\n===== SISTEMA DE BIBLIOTECA =====\n");
        printf("1. Cadastrar novo livro\n");
        printf("2. Remover livro\n");
        printf("3. Emprestar livro\n");
        printf("4. Devolver livro\n");
        printf("5. Buscar livro por ID\n");
        printf("6. Listar todos os livros\n");
        printf("7. Limpar biblioteca\n");
        printf("8. Exibir tabela hash (índices)\n");
        printf("0. Sair\n");
        printf("Escolha: ");

        opcao = lerInteiro("");

        switch (opcao) {
            case 1: inserirLivro(); break;
            case 2: removerLivro(); break;
            case 3: emprestarLivro(); break;
            case 4: devolverLivro(); break;
            case 5: buscarLivro(); break;
            case 6: listarLivros(); break;
            case 7: limparBiblioteca(); break;
            case 8: exibirTabelaHash(); break;
            case 0: printf("Encerrando sistema...\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}

int main() {
    menu();
    limparBiblioteca();
    return 0;
}
