#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <iostream>
#include <stdexcept>
#include <string>

//definicão do nó para as listas ( colunas e linhas da matriz)
struct Node {
    Node* DIREITA; //aponta para o próximo elemento da mesma LINHA de uma matriz;
    Node* ABAIXO;  //aponta para o próximo elemento da mesma COLUNA de uma matriz;
    int LINHA;     //linha da matriz onde o valor está guardado;
    int COLUNA;    //coluna da matriz onde o valor está guardado;
    double VALOR;  //valor que está armazenado em posicao(LINHA, COLUNA);
};

// Classe que representa uma matriz esparsa usando listas encadeadas
class SparseMatrix {
private:
    int LINHA_MATRIZ;  // Número de linhas da matriz
    int COLUNA_MATRIZ;  // Número de colunas da matriz
    Node** LINHAS_LISTA; // Vetor de ponteiros para as listas de linhas
    Node** COLUNAS_LISTA; // Vetor de ponteiros para as listas de colunas

    // Função auxiliar para verificar a validade dos índices
    void verificarIndices(int i, int j) {
        if (i < 1 || i > COLUNA_MATRIZ || j < 1 || j > LINHA_MATRIZ) {
            throw std::out_of_range("Índices inválidos");
        }
    }

public:
    //Construtor da classe. Inicializa uma matriz esparsa vazia com capacidade para m
    //linhas e n colunas. Essa funcao deve checar se os valores passados sao validos (se
    //sao inteiros positivos, ou seja m > 0 e m > 0); se nao forem, uma excecao deve ser
    //lancada.
    SparseMatrix(int m, int n) {
        //se o número de linhas ou colunas for menor ou igual a 0
        //gera uma exceção
        if (m <= 0 || n <= 0) {
            throw std::invalid_argument("Número de linhas e colunas deve ser positivo");
        }

        this->LINHA_MATRIZ = m; //linhas da matriz recebe o número de linhas que o usuário colocou
        this->COLUNA_MATRIZ = n; //colunas da matriz recebe o número de linhas que o usuário colocou

        LINHAS_LISTA = new Node*[m + 1]; //cria um novo nó com as linhas da lista que o usuário colocou ( coloquei +1 pois a SparseMatrix começa do ínidice 1)
        COLUNAS_LISTA = new Node*[n + 1]; //cria um novo nó com as colunas da lista que o usuário colocou ( coloquei +1 pois a SparseMatrix começa do ínidice 1)
        
        for (int i = 1; i <= m; ++i) {
            LINHAS_LISTA[i] = new Node{nullptr, nullptr, i, 0, 0}; //inicializa os nós (ou a própria lista), ou seja, inicializa as linhas da nossa matriz esparca
        }                             //nullptr são para os ponteiros que vão servir para a locomoção dentro das listas (não podem apontar pra nenhum valor), o i é incrementado a cada loop, coluna e valor não importam aqui, então inicializei com 0.
        for (int j = 1; j <= n; ++j) {
            COLUNAS_LISTA[j] = new Node{nullptr, nullptr, 0, j, 0}; //inicializa os nós (ou a própria lista), ou seja, inicializa as colunas da nossa matriz esparca
        }                             //nullptr são para os ponteiros que vão servir para a locomoção dentro das listas( não podem apontar pra nenhum valor), o i é incrementado a cada loop, linha e valor não importam aqui, então inicializei com 0.
    }

    //Segundo método: Destrutor da SparseMatrix ( Libera memória que foi alocada
    //dinamicamente para a matriz.)
    ~SparseMatrix() {
        for (int i = 1; i <= LINHA_MATRIZ; ++i) {//itero sobre as linhas da matriz ( não importa o número de linhas)
            Node* atual = LINHAS_LISTA[i]; //crio um ponteiro que aponta para o inicio da lista(na linha i);
            while (atual != nullptr) {   //enquanto o ponteiro não estiver apontando para nullptr (fim das linhas da lista)      
                Node* temp = atual;  //crio um ponteiro temporário para armazenar o nó atual
                atual = atual->DIREITA; // pego o ponteiro que aponta para o início da lista e faço ele apontar para o próximo nó da lista, ou seja, aponta para o próximo elemento
                delete temp; //o elemento anterior é deletado da memória
            }
        }
        delete[] LINHAS_LISTA; //libera espaço da memória para linhas
        delete[] COLUNAS_LISTA; //libera espaço da memória para colunas
    }

    // Insere um valor na posição (i, j) da matriz
    void insert(int i, int j, double value) {
        verificarIndices(i, j);  //verifica se i/j são maiores ou iguais a 0

        if (value == 0) return; //se for 0 não faz nada por que já foi inicializado com 0
        Node* novo = new Node{nullptr, nullptr, i, j, value}; //cria uma nova matriz para atribuir o novo valor

        //inserção na lista da linha i
        Node* linha = LINHAS_LISTA[i]; //cria um ponteiro que aponta para as linhas da lista
        while (linha->DIREITA && linha->DIREITA->COLUNA < j) { //enquanto o nó apontar para o próximo elemento da linha e o próximo elemento na POS linha,coluna for maior que j
            linha = linha->DIREITA; //o nó vai apontar para o próximo elemento da lista.
            
        }
        novo->DIREITA = linha->DIREITA; //a nova matriz recebe os elementos da lista
        linha->DIREITA = novo; // os elementos são atribuidos a nova matriz resultante 

        // Inserção na lista da coluna j
        Node* coluna = COLUNAS_LISTA[j];
        while (coluna->ABAIXO && coluna->ABAIXO->LINHA < i) {
            coluna = coluna->ABAIXO;
        }
        novo->ABAIXO = coluna->ABAIXO;
        coluna->ABAIXO = novo;
    }

    // Retorna o valor na posição (i, j)
    double get(int i, int j) {
        verificarIndices(i, j); // Verifica se os índices são válidos

        Node* linha = LINHAS_LISTA[i];
        while (linha && linha->COLUNA < j) {
            linha = linha->DIREITA;
        }

        if (linha && linha->COLUNA == j) {
            return linha->VALOR;
        }
        return 0; // Retorna 0 se a posição for vazia
    }

    // Imprime a matriz esparsa no terminal
    void print() {
        for (int i = 1; i <= LINHA_MATRIZ; ++i) {
            Node* linha = LINHAS_LISTA[i]->DIREITA;
            for (int j = 1; j <= COLUNA_MATRIZ; ++j) {
                if (linha && linha->COLUNA == j) {
                    std::cout << linha->VALOR << " ";
                    linha = linha->DIREITA;
                } else {
                    std::cout << "0 ";
                }
            }
            std::cout << std::endl;
        }
    }

    //getter para LINHAS da MATRIZ
    int getLinhasMatriz() const { return LINHA_MATRIZ; }

    //getter para COLUNAS da MATRIZ
    int getColunasMatriz() const { return COLUNA_MATRIZ; }
};

#endif // SPARSEMATRIX_H
