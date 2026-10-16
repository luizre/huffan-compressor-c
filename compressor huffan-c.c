#include <stdio.h>
#include <stdlib.h>

#define MAX_TREE_HT 256 

typedef struct No No;
struct No {
    unsigned char item;
    unsigned freq;
    No* esquerda;
    No* direita;
};

typedef struct Fila {
    unsigned tamanho;
    No** vetor;
} Fila;

typedef struct {
    unsigned char caractere;
    char codigo[MAX_TREE_HT];
    int tamanho_bits;
} Tabela;

No* criarno(unsigned char item, unsigned freq) {
    No* temp = (No*)malloc(sizeof(No));
    temp->esquerda = temp->direita = NULL;
    temp->item = item;
    temp->freq = freq;
    return temp;
}

Fila* criarfila(unsigned capacidade) {
    Fila* fila = (Fila*)malloc(sizeof(Fila));
    fila->tamanho = 0;
    fila->vetor = (No**)malloc(capacidade * sizeof(No*));
    return fila;
}

void trocarno(No** a, No** b) {
    No* t = *a;
    *a = *b;
    *b = t;
}

void organizarfila(Fila* fila, int indice) {
    int menor = indice;
    unsigned esquerda = 2 * indice + 1;
    unsigned direita = 2 * indice + 2;

    if (esquerda < fila->tamanho && fila->vetor[esquerda]->freq < fila->vetor[menor]->freq)
        menor = esquerda;

    if (direita < fila->tamanho && fila->vetor[direita]->freq < fila->vetor[menor]->freq)
        menor = direita;

    if (menor != indice) {
        trocarno(&fila->vetor[menor], &fila->vetor[indice]);
        organizarfila(fila, menor);
    }
}

int tamanhoehum(Fila* fila) {
    return (fila->tamanho == 1);
}

No* extrairminimo(Fila* fila) {
    No* temp = fila->vetor[0];
    fila->vetor[0] = fila->vetor[fila->tamanho - 1];
    --fila->tamanho;
    organizarfila(fila, 0);
    return temp;
}

void inserirnafila(Fila* fila, No* novono) {
    ++fila->tamanho;
    int i = fila->tamanho - 1;

    while (i && novono->freq < fila->vetor[(i - 1) / 2]->freq) {
        fila->vetor[i] = fila->vetor[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    fila->vetor[i] = novono;
}

void construirfila(Fila* fila) {
    int n = fila->tamanho - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        organizarfila(fila, i);
}

int ehfolha(No* raiz) {
    return !(raiz->esquerda) && !(raiz->direita);
}

Fila* criareconstruirfila(unsigned char item[], int freq[], int tamanho) {
    Fila* fila = criarfila(tamanho);
    for (int i = 0; i < tamanho; ++i)
        fila->vetor[i] = criarno(item[i], freq[i]);
    fila->tamanho = tamanho;
    construirfila(fila);
    return fila;
}

No* construirarvorehuffman(unsigned char item[], int freq[], int tamanho) {
    No* esquerda, * direita, * topo;
    Fila* fila = criareconstruirfila(item, freq, tamanho);

    while (!tamanhoehum(fila)) {
        esquerda = extrairminimo(fila);
        direita = extrairminimo(fila);
        topo = criarno('$', esquerda->freq + direita->freq);
        topo->esquerda = esquerda;
        topo->direita = direita;
        inserirnafila(fila, topo);
    }
    return extrairminimo(fila);
}

void gerartabela(No* raiz, int arr[], int topo_arr, Tabela* tab, int* index) {
    if (raiz->esquerda) {
        arr[topo_arr] = 0;
        gerartabela(raiz->esquerda, arr, topo_arr + 1, tab, index);
    }
    if (raiz->direita) {
        arr[topo_arr] = 1;
        gerartabela(raiz->direita, arr, topo_arr + 1, tab, index);
    }
    if (ehfolha(raiz)) {
        tab[*index].caractere = raiz->item;
        tab[*index].tamanho_bits = topo_arr;

        for (int i = 0; i < topo_arr; i++) {
            tab[*index].codigo[i] = arr[i] ? '1' : '0';
        }
        tab[*index].codigo[topo_arr] = '\0';

        if (tab[*index].caractere == '\n') {
            printf("Caractere: '\\n' | Codigo Binario: %-15s | Tamanho: %d bits\n", tab[*index].codigo, tab[*index].tamanho_bits);
        }
        else if (tab[*index].caractere == '\r') {
            printf("Caractere: '\\r' | Codigo Binario: %-15s | Tamanho: %d bits\n", tab[*index].codigo, tab[*index].tamanho_bits);
        }
        else if (tab[*index].caractere == '\t') {
            printf("Caractere: '\\t' | Codigo Binario: %-15s | Tamanho: %d bits\n", tab[*index].codigo, tab[*index].tamanho_bits);
        }
        else if (tab[*index].caractere < 32 || tab[*index].caractere == 127) {
            printf("Caractere: CTRL(%d) | Codigo Binario: %-15s | Tamanho: %d bits\n", tab[*index].caractere, tab[*index].codigo, tab[*index].tamanho_bits);
        }
        else {
            printf("Caractere: '%c'  | Codigo Binario: %-15s | Tamanho: %d bits\n", tab[*index].caractere, tab[*index].codigo, tab[*index].tamanho_bits);
        }

        (*index)++;
    }
}

void compactararquivo(const char* arquivoEntrada, const char* arquivoSaida, Tabela* tab, int tamTab) {
    FILE* in = fopen(arquivoEntrada, "r");
    FILE* out = fopen(arquivoSaida, "wb");

    if (!in || !out) {
        printf("Erro ao abrir arquivos para compactacao\n");
        return;
    }

    unsigned char buffer = 0;
    int bit_count = 0;
    int ch;

    while ((ch = fgetc(in)) != EOF) {
        for (int i = 0; i < tamTab; i++) {
            if (tab[i].caractere == (unsigned char)ch) {
                for (int j = 0; tab[i].codigo[j] != '\0'; j++) {
                    buffer = buffer << 1;
                    if (tab[i].codigo[j] == '1') {
                        buffer = buffer | 1;
                    }
                    bit_count++;

                    if (bit_count == 8) {
                        fputc(buffer, out);
                        buffer = 0;
                        bit_count = 0;
                    }
                }
                break;
            }
        }
    }

    if (bit_count > 0) {
        buffer = buffer << (8 - bit_count);
        fputc(buffer, out);
    }

    fclose(in);
    fclose(out);
    printf("Arquivo compactado com sucesso!\n");
}

void descompactararquivo(const char* arquivoEntrada, const char* arquivoSaida, No* raiz, int totalLetras) {
    FILE* in = fopen(arquivoEntrada, "rb");
    FILE* out = fopen(arquivoSaida, "w");

    if (!in || !out) {
        printf("Erro ao abrir arquivos para descompactacao\n");
        return;
    }

    No* atual = raiz;
    int byte_lido;
    int letras_escritas = 0;

    while ((byte_lido = fgetc(in)) != EOF && letras_escritas < totalLetras) {
        for (int i = 7; i >= 0; i--) {
            int bit = (byte_lido >> i) & 1;

            if (bit == 0) {
                atual = atual->esquerda;
            }
            else {
                atual = atual->direita;
            }

            if (ehfolha(atual)) {
                fputc(atual->item, out);
                atual = raiz;
                letras_escritas++;

                if (letras_escritas == totalLetras) break;
            }
        }
    }

    fclose(in);
    fclose(out);
    printf("Arquivo descompactado com sucesso!\n");
}

int main() {
    FILE* arquivo = fopen("textoCompactadorDescompactador.txt", "r");
    if (arquivo == NULL) {
        printf("Arquivo de entrada nao encontrado.\n");
        return 1;
    }

    long contagem[256] = { 0 };
    int totalLetras = 0;
    int ch;

    while ((ch = fgetc(arquivo)) != EOF) {
        contagem[(unsigned char)ch]++;
        totalLetras++;
    }
    fclose(arquivo);

    unsigned char items[256];
    int freqs[256];
    int size = 0;

    for (int i = 0; i < 256; i++) {
        if (contagem[i] > 0) {
            items[size] = (unsigned char)i;
            freqs[size] = (int)contagem[i];

            if (i == '\n') {
                printf("Letra: '\\n' | Ocorrencias: %d\n", freqs[size]);
            }
            else if (i == '\r') {
                printf("Letra: '\\r' | Ocorrencias: %d\n", freqs[size]);
            }
            else if (i == '\t') {
                printf("Letra: '\\t' | Ocorrencias: %d\n", freqs[size]);
            }
            else if (i < 32 || i == 127) {
                printf("Letra: CTRL(%d) | Ocorrencias: %d\n", i, freqs[size]);
            }
            else {
                printf("Letra: '%c'  | Ocorrencias: %d\n", items[size], freqs[size]);
            }
            size++;
        }
    }

    No* raiz = construirarvorehuffman(items, freqs, size);
    printf("\n--- Versao Huffman ---\n");

    Tabela dicionario[256];
    int totalDicionario = 0;
    int arr[MAX_TREE_HT], top = 0;

    gerartabela(raiz, arr, top, dicionario, &totalDicionario);

    compactararquivo("textoCompactadorDescompactador.txt", "saida_compactada.bin", dicionario, totalDicionario);
    descompactararquivo("saida_compactada.bin", "saida_descompactada.txt", raiz, totalLetras);

    return 0;
}