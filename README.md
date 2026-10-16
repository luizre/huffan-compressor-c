# Compactador e Descompactador de Arquivos (Algoritmo de Huffman)

Implementação em C de uma ferramenta de compressão e descompressão de arquivos de texto sem perdas (lossless), baseada na codificação de Huffman. O programa opera diretamente no nível dos bits para reduzir o tamanho físico dos arquivos em disco em cerca de 50% em amostras de texto.

## Arquitetura e Funcionamento

O processo de execução é dividido em quatro etapas principais:

1. **Leitura e Histograma de Frequências:** O programa faz a leitura sequencial do arquivo de entrada (`fgetc`) e contabiliza a ocorrência de cada byte em um vetor de frequências de 256 posições, registrando também o total de caracteres válidos (`totalLetras`) para controle de integridade na reconstrução.
2. **Fila de Prioridades (Min-Heap) e Árvore de Huffman:** Os caracteres ativos são alocados dinamicamente e inseridos em um Min-Heap. A árvore de prefixos é construída extraindo iterativamente os dois nós de menor frequência, unindo-os sob um nó pai com a soma dos pesos e reinserindo o resultado na fila até restar apenas o nó raiz.
3. **Dicionário de Dados (DFS):** Uma busca em profundidade (Depth-First Search) percorre a árvore gerada a partir da raiz (atribuindo `0` para a esquerda e `1` para a direita). Ao atingir um nó folha, a sequência binária é fechada com o terminador `\0` e registrada na tabela de codificação.
4. **Compressão e Descompressão Bit a Bit:**
   - **Compactação:** O arquivo original é relido e convertido usando a tabela gerada. Os bits são agrupados em um buffer do tipo `unsigned char` por meio de deslocamento à esquerda (`<<`) e disjunção lógica (`|`). A cada 8 bits preenchidos, o byte completo é gravado em fluxo binário (`wb`). Bits residuais no fim do arquivo recebem alinhamento à esquerda (padding) antes da gravação final.
   - **Descompactação:** O arquivo binário é lido byte a byte (`rb`). Cada byte é decomposto por deslocamento à direita (`>>`) combinado com a máscara lógica (`& 1`) em um laço decrescente. O valor resultante guia o ponteiro pela árvore (esquerda para `0`, direita para `1`) até encontrar uma folha e gravar o caractere correspondente no arquivo final, encerrando o processo ao atingir a contagem exata de `totalLetras` para ignorar os bits de padding.

## Estrutura do Código

- `struct No`: Unidade básica da árvore de Huffman. Armazena o byte (`unsigned char item`), a frequência de ocorrência (`freq`) e os ponteiros para os filhos à esquerda e à direita.
- `struct Fila`: Estrutura da Fila de Prioridades (Min-Heap), contendo o tamanho atual e o vetor dinâmico de ponteiros para os nós.
- `struct Tabela`: Dicionário estático que associa cada caractere ao seu código binário em formato de string e ao seu comprimento em bits.
- `organizarfila` e `construirfila`: Mantêm a propriedade matemática do Min-Heap (Heapify), garantindo que os nós de menor frequência permaneçam no topo.
- `inserirnafila` e `extrairminimo`: Controlam a inserção de nós combinados e a remoção dos elementos de menor peso da fila.
- `construirarvorehuffman`: Orquestra a fusão dos nós do Min-Heap até consolidar a raiz da árvore.
- `gerartabela`: Realiza a travessia recursiva na árvore para preencher o dicionário de códigos binários.
- `compactararquivo` e `descompactararquivo`: Executam as operações de entrada/saída em disco e a manipulação de bits para comprimir e restaurar os dados.

## Como Compilar e Executar

### Pré-requisitos
Compilador C padrão (GCC, Clang ou MSVC).

### Compilação via terminal (GCC)

gcc -Wall -O2 huffman.c -o huffman

### Execução
Certifique-se de que o arquivo de entrada `textoCompactadorDescompactador.txt` esteja no mesmo diretório do executável e rode:

./huffman

O programa exibirá no terminal a contagem de ocorrências e a tabela de codificação de Huffman gerada, criando em seguida dois arquivos no diretório:
- `saida_compactada.bin`: Arquivo binário comprimido.
- `saida_descompactada.txt`: Arquivo de texto reconstruído a partir do binário.

## Referências

- A estrutura base da Fila de Prioridades (Min-Heap) e a lógica inicial de ordenação dos nós da árvore tiveram como referência a implementação documentada pelo portal Programiz: [Huffman Coding Algorithm](https://www.programiz.com/dsa/huffman-coding).
- A partir dessa base teórica, o código foi expandido e adaptado para operar com leitura e escrita dinâmica de arquivos em disco, tratamento de caracteres invisíveis (como quebras de linha e espaços), suporte completo à tabela de 256 bytes e gravação física em nível de bits (bitwise operations).