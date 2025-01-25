#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <iostream>
#include <stdexcept>
#include <string>

// Estrutura que define o nó usado nas listas de linhas e colunas
struct Node {
    Node* DIREITA; // Ponteiro para o próximo nó na mesma linha
    Node* ABAIXO;  // Ponteiro para o próximo nó na mesma coluna
    int LINHA;     // Índice da linha onde o valor está localizado
    int COLUNA;    // Índice da coluna onde o valor está localizado
    double VALOR;  // Valor armazenado na célula da matriz
};

// Classe que representa uma matriz esparsa usando listas circulares encadeadas
class SparseMatrix {
private:
    int LINHA_MATRIZ;  // Número total de linhas da matriz
    int COLUNA_MATRIZ; // Número total de colunas da matriz
    Node** LINHAS_LISTA; // Vetor de ponteiros para listas que representam as linhas
    Node** COLUNAS_LISTA; // Vetor de ponteiros para listas que representam as colunas

    //verifica se os índices são válidos
    void verificarIndices(int i, int j) {
        if (i < 1 || i > LINHA_MATRIZ || j < 1 || j > COLUNA_MATRIZ) {
            throw std::out_of_range("Índices inválidos"); // Lança exceção se índices forem inválidos
        }
    }

public:

    // Construtor da classe SparseMatrix
    // Inicializa uma matriz esparsa vazia com m linhas e n colunas
    // Lança exceção se m ou n forem menores ou iguais a 0
    // q1
    SparseMatrix(int m, int n) {
        if (m <= 0 || n <= 0) {
            throw std::invalid_argument("Numero de linhas e colunas deve ser maior que zero");
        }

        // Inicializa o número de linhas e colunas da matriz
        LINHA_MATRIZ = m;
        COLUNA_MATRIZ = n;

        // Aloca memória para as listas de linhas e colunas, mas não para arrays
        LINHAS_LISTA = new Node*[LINHA_MATRIZ + 1];  // Um índice extra para simplificar a indexação
        COLUNAS_LISTA = new Node*[COLUNA_MATRIZ + 1]; // Um índice extra para simplificar a indexação

        // Inicializa as listas de linhas e colunas com nós sentinelas
        for (int i = 1; i <= LINHA_MATRIZ; ++i) {
            LINHAS_LISTA[i] = new Node{nullptr, nullptr, 0, 0, 0}; // Cria um nó sentinela para cada linha
            LINHAS_LISTA[i]->DIREITA = LINHAS_LISTA[i]; // A lista da linha aponta para si mesma (circular)
        }

        for (int j = 1; j <= COLUNA_MATRIZ; ++j) {
            COLUNAS_LISTA[j] = new Node{nullptr, nullptr, 0, 0, 0}; // Cria um nó sentinela para cada coluna
            COLUNAS_LISTA[j]->ABAIXO = COLUNAS_LISTA[j]; // A lista da coluna aponta para si mesma (circular)
        }
}

    // Destrutor da classe SparseMatrix
    // O destrutor libera toda a memória que foi alocada dinamicamente para as linhas, colunas e os nós da matriz.
    // q2

    ~SparseMatrix() {
        // Libera memória das listas de linhas
        for (int i = 1; i <= LINHA_MATRIZ; ++i) {
            Node* atual = LINHAS_LISTA[i]->DIREITA; // Começa no primeiro nó da linha
            while (atual != LINHAS_LISTA[i]) { // Percorre a lista até o nó sentinela
                Node* temp = atual;
                atual = atual->DIREITA; // Avança para o próximo nó
                delete temp; // Libera a memória do nó atual
            }
            delete LINHAS_LISTA[i]; // Libera o nó sentinela da linha
        }

        // Libera memória das listas de colunas
        for (int j = 1; j <= COLUNA_MATRIZ; ++j) {
            delete COLUNAS_LISTA[j]; // Libera o nó sentinela da coluna
        }

        // Libera os vetores de ponteiros para as listas de linhas e colunas
        delete[] LINHAS_LISTA;
        delete[] COLUNAS_LISTA;
    }

    // Função que insere ou atualiza um valor na posição (i, j)
    // Se o valor for 0, não faz nada
    // q3
    void insert(int i, int j, double value) {
        verificarIndices(i, j); // Verifica se os índices são válidos

        if (value == 0) { // Se o valor for 0, não faz nada
            return;
        }

        // Percorre a lista da linha para encontrar a posição correta
        Node* linha = LINHAS_LISTA[i];
        while (linha->DIREITA != LINHAS_LISTA[i] && linha->DIREITA->COLUNA < j) {
            linha = linha->DIREITA; // Avança para o próximo nó
        }

        // Se o valor já existe, atualiza o valor
        if (linha->DIREITA != LINHAS_LISTA[i] && linha->DIREITA->COLUNA == j) {
            linha->DIREITA->VALOR = value;
            return;
        }

        // Caso contrário, cria um novo nó para o valor
        Node* novo = new Node{nullptr, nullptr, i, j, value};
        novo->DIREITA = linha->DIREITA; // Ajusta o ponteiro para a direita na linha
        linha->DIREITA = novo;

        // Percorre a coluna para ajustar os ponteiros verticais
        Node* coluna = COLUNAS_LISTA[j];
        while (coluna->ABAIXO != COLUNAS_LISTA[j] && coluna->ABAIXO->LINHA < i) {
            coluna = coluna->ABAIXO; // Avança para o próximo nó na coluna
        }

        novo->ABAIXO = coluna->ABAIXO; // Ajusta o ponteiro para baixo na coluna
        coluna->ABAIXO = novo;
    }

    // Função que retorna o valor na célula (i, j) da matriz
    // Se a célula não existir, retorna 0
    //q5
    double get(int i, int j) {
        verificarIndices(i, j); // Verifica se os índices são válidos

        // Percorre a lista da linha para encontrar o valor
        Node* linha = LINHAS_LISTA[i]->DIREITA;
        while (linha != LINHAS_LISTA[i] && linha->COLUNA < j) {
            linha = linha->DIREITA; // Avança para o próximo nó na linha
        }

        // Se o valor foi encontrado, retorna o valor
        if (linha != LINHAS_LISTA[i] && linha->COLUNA == j) {
            return linha->VALOR;
        }

        return 0; // Retorna 0 se o valor não for encontrado
    }

    // Função que imprime a matriz esparsa no terminal
    // Imprime todos os valores da matriz, incluindo os zeros
    // q5
    void printar_matrizEsparsa() {
        // Percorre todas as linhas
        for (int i = 1; i <= LINHA_MATRIZ; ++i) {
            Node* linha = LINHAS_LISTA[i]->DIREITA; // Começa no primeiro nó da linha
            for (int j = 1; j <= COLUNA_MATRIZ; ++j) {
                if (linha != LINHAS_LISTA[i] && linha->COLUNA == j) {
                    std::cout << linha->VALOR << " "; // Imprime o valor se a célula não for zero
                    linha = linha->DIREITA; // Avança para o próximo nó na linha
                } else {
                    std::cout << "0 "; // Imprime 0 para células vazias
                }
            }
            std::cout << std::endl; // Nova linha após imprimir uma linha da matriz
        }
    }

 void remover_valor(int m, int n, double value) {//chat ajudou
    verificarIndices(m, n); // Verifica se os índices m, n são válidos

    // Percorre todas as linhas da matriz
    for (int i = 1; i <= LINHA_MATRIZ; ++i) {
        Node* linha = LINHAS_LISTA[i]->DIREITA; // Começa a lista da linha

        // Percorre todas as colunas da linha
        while (linha != LINHAS_LISTA[i]) {
            // Verifica se o valor do nó é igual ao valor que deve ser removido
            if (linha->VALOR == value) {
                // Remover o nó da lista da linha
                Node* temp = linha;
                linha = linha->DIREITA; // Avança para o próximo nó da linha
                // Ajusta o ponteiro para desconectar o nó da lista
                temp->DIREITA = temp->DIREITA->DIREITA;

                // Agora, remova o nó da lista da coluna
                Node* coluna = COLUNAS_LISTA[temp->COLUNA];
                while (coluna->ABAIXO != COLUNAS_LISTA[temp->COLUNA] && coluna->ABAIXO->LINHA != temp->LINHA) {
                    coluna = coluna->ABAIXO; // Avança na lista da coluna
                }

                // Ajusta o ponteiro da coluna para desconectar o nó
                coluna->ABAIXO = coluna->ABAIXO->ABAIXO;

                // Libera a memória do nó removido
                delete temp;
            } else {
                linha = linha->DIREITA; // Avança para o próximo nó na linha
            }
        }
    }
}


    // getters para o número de linhas e colunas
    int getLinhasMatriz()  { 
        return LINHA_MATRIZ; 
        }
    int getColunasMatriz()  { 
        return COLUNA_MATRIZ; 
        }
};

#endif
