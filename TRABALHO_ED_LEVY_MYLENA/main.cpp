#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "SparseMatrix.h"

// Função para exibir os comandos disponíveis
void exibirAjuda() {
    std::cout << "\nComandos disponíveis:" << std::endl;
    std::cout << "1. 'inserir <linha> <coluna> <valor>' - Insere um valor na matriz na posicao (linha, coluna)" << std::endl;
    std::cout << "2. 'obter <linha> <coluna>' - Obtem o valor da matriz na posicao (linha, coluna)" << std::endl;
    std::cout << "3. 'imprimir' - Exibe a matriz esparsa" << std::endl;
    std::cout << "4. 'ajuda' - Exibe este menu de ajuda" << std::endl;
    std::cout << "5. 'sair' - Encerra o programa" << std::endl;
    std::cout << "6. 'ler <nome_arquivo>' - Lê uma matriz esparsa de um arquivo" << std::endl;
    std::cout << "7. 'somar' - Soma duas matrizes esparsas" << std::endl;
    std::cout << "8. 'multiplicar' - Multiplica duas matrizes esparsas" << std::endl;
}

// Função para ler uma matriz esparsa a partir de um arquivo
void readSparseMatrix(SparseMatrix& m, std::string nome_do_arquivo) {
    std::ifstream arquivo(nome_do_arquivo);
    if (!arquivo) {
        std::cerr << "Erro ao abrir o arquivo!" << std::endl;
        return;
    }

    int linhas, colunas;
    arquivo >> linhas >> colunas; // Lê o número de linhas e colunas

    SparseMatrix matriz(linhas, colunas);
    int i, j;
    double valor;

    // Lê as triplas (i, j, valor)
    while (arquivo >> i >> j >> valor) {
        matriz.insert(i, j, valor);
    }

    m = matriz; // Atribui a matriz lida à matriz fornecida
    std::cout << "Matriz carregada com sucesso do arquivo: " << nome_do_arquivo << std::endl;
}

// Função para somar duas matrizes esparsas
SparseMatrix sum(SparseMatrix& A, SparseMatrix& B) {
    if (A.getLinhasMatriz() != B.getLinhasMatriz() || A.getColunasMatriz() != B.getColunasMatriz()) {
        throw std::invalid_argument("As matrizes devem ter as mesmas dimensões para a soma.");
    }

    SparseMatrix C(A.getLinhasMatriz(), A.getColunasMatriz());

    for (int i = 1; i <= A.getLinhasMatriz(); ++i) {
        for (int j = 1; j <= A.getColunasMatriz(); ++j) {
            double valorA = A.get(i, j);
            double valorB = B.get(i, j);
            if (valorA != 0) C.insert(i, j, valorA);
            if (valorB != 0) C.insert(i, j, valorB);
        }
    }

    return C;
}

// Função para multiplicar duas matrizes esparsas
SparseMatrix multiply(SparseMatrix& A, SparseMatrix& B) {
    if (A.getColunasMatriz() != B.getLinhasMatriz()) {
        throw std::invalid_argument("O número de colunas de A deve ser igual ao número de linhas de B.");
    }

    SparseMatrix C(A.getLinhasMatriz(), B.getColunasMatriz());

    for (int i = 1; i <= A.getLinhasMatriz(); ++i) {
        for (int j = 1; j <= B.getColunasMatriz(); ++j) {
            double valorC = 0;
            for (int k = 1; k <= A.getColunasMatriz(); ++k) {
                valorC += A.get(i, k) * B.get(k, j);
            }
            if (valorC != 0) {
                C.insert(i, j, valorC);
            }
        }
    }

    return C;
}

int main() {
    SparseMatrix mat(3, 3);
    SparseMatrix matB(3, 3); // Segunda matriz esparsa para operações de soma e multiplicação
    std::string comando;

    std::cout << "Bem-vindo ao programa de Matriz Esparsa!" << std::endl;
    exibirAjuda();

    while (true) {
        std::cout << "\nDigite um comando: ";
        std::getline(std::cin, comando);

        if (comando == "sair") {
            std::cout << "Saindo do programa..." << std::endl;
            break;
        } else if (comando == "ajuda") {
            exibirAjuda();
        } else if (comando.rfind("inserir", 0) == 0) {
            // Comando para inserir valor
            int linha, coluna;
            double valor;
            if (sscanf(comando.c_str(), "inserir %d %d %lf", &linha, &coluna, &valor) == 3) {
                try {
                    mat.insert(linha, coluna, valor);
                    std::cout << "Valor " << valor << " inserido na posição (" << linha << ", " << coluna << ")." << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Erro: " << e.what() << std::endl;
                }
            } else {
                std::cerr << "Comando inválido. Utilize: inserir <linha> <coluna> <valor>" << std::endl;
            }
        } else if (comando.rfind("obter", 0) == 0) {
            // Comando para obter valor
            int linha, coluna;
            if (sscanf(comando.c_str(), "obter %d %d", &linha, &coluna) == 2) {
                try {
                    double valor = mat.get(linha, coluna);
                    std::cout << "Valor na posição (" << linha << ", " << coluna << "): " << valor << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Erro: " << e.what() << std::endl;
                }
            } else {
                std::cerr << "Comando inválido. Utilize: obter <linha> <coluna>" << std::endl;
            }
        } else if (comando == "imprimir") {
            // Comando para imprimir a matriz
            std::cout << "\nMatriz esparsa: " << std::endl;
            mat.print();
        } else if (comando.rfind("ler", 0) == 0) {
            // Comando para ler matriz de arquivo
            std::string nome_arquivo;
            if (sscanf(comando.c_str(), "ler %s", &nome_arquivo[0]) == 1) {
                readSparseMatrix(mat, nome_arquivo);
            } else {
                std::cerr << "Comando inválido. Utilize: ler <nome_arquivo>" << std::endl;
            }
        } else if (comando == "somar") {
            // Comando para somar matrizes
            try {
                SparseMatrix resultado = sum(mat, matB);
                std::cout << "Resultado da soma:" << std::endl;
                resultado.print();
            } catch (const std::exception& e) {
                std::cerr << "Erro: " << e.what() << std::endl;
            }
        } else if (comando == "multiplicar") {
            // Comando para multiplicar matrizes
            try {
                SparseMatrix resultado = multiply(mat, matB);
                std::cout << "Resultado da multiplicação:" << std::endl;
                resultado.print();
            } catch (const std::exception& e) {
                std::cerr << "Erro: " << e.what() << std::endl;
            }
        } else {
            std::cerr << "Comando não reconhecido. Digite 'ajuda' para listar os comandos disponíveis." << std::endl;
        }
    }

    return 0;
}
