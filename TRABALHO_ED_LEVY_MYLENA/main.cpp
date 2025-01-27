#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "SparseMatrix.h"

using namespace std;

// Função para exibir os comandos disponíveis
void exibirAjuda() {
    cout << "\nComandos disponíveis:" << endl;
    cout << "1. 'inserir <linha> <coluna> <valor>' - Insere um valor na matriz na posicao (linha, coluna)" << endl;
    cout << "2. 'obter <linha> <coluna>' - Obtem o valor da matriz na posicao (linha, coluna)" << endl;
    cout << "3. 'imprimir' - Exibe a matriz esparsa" << endl;
    cout << "4. 'ajuda' - Exibe este menu de ajuda" << endl;
    cout << "5. 'sair' - Encerra o programa" << endl;
    cout << "6. 'ler <nome_arquivo>' - Le uma matriz esparsa de um arquivo" << endl;
    cout << "7. 'somar' - Soma duas matrizes esparsas" << endl;
    cout << "8. 'multiplicar' - Multiplica duas matrizes esparsas" << endl;
    cout << "9. 'remover <linha> <coluna>' - Remove o valor na posicao (linha, coluna)" << endl;
}

// Função para ler uma matriz esparsa a partir de um arquivo
void readSparseMatrix(SparseMatrix& m, string nome_do_arquivo) {
    ifstream arquivo(nome_do_arquivo);
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo!" << endl;
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
    cout << "Matriz carregada com sucesso do arquivo: " << nome_do_arquivo << endl;
}

// Função para somar duas matrizes esparsas
SparseMatrix sum(SparseMatrix& A, SparseMatrix& B) {
    if (A.getLinhasMatriz() != B.getLinhasMatriz() || A.getColunasMatriz() != B.getColunasMatriz()) {
        throw invalid_argument("As matrizes devem ter as mesmas dimensões para a soma.");
    }

    SparseMatrix C(A.getLinhasMatriz(), A.getColunasMatriz()); //já que m x n de A e B são iguais, eu uso o getter para a mesma matriz ou não;

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
        throw invalid_argument("O número de colunas de A deve ser igual ao número de linhas de B.");
    }

    SparseMatrix C(A.getLinhasMatriz(), B.getColunasMatriz()); //na multiplicacao é obrigatoria que as colunas de A sejam iguais as linhas de B

    for (int i = 1; i <= A.getLinhasMatriz(); i++) { // for para linhas de A
        for (int j = 1; j <= B.getColunasMatriz(); j++) { // for para as colunas de B ( para cada linha de A)
        double valorC = 0;
            for (int k = 1; k <= A.getColunasMatriz(); k++) { // for para as colunas de A ( agora percorre tudo )
                double valorA = A.get(i, k); // A recebe o valor (i,k), ou seja, a linha I e coluna K da matriz A
                double valorB = B.get(k, j); // B recebe o valor (k,j), ou seja, a linha K e coluna J da matriz B
                valorC += valorA * valorB;   // acumula o resultado nos valores da matriz C
            }

            if (valorC != 0) {
                C.insert(i, j, valorC); // se o valor de A * B for diferente de 0, insere na matriz C
            }
    }
}


    return C;
}

int main() {
    SparseMatrix mat(3, 3); // Matriz inicial
    SparseMatrix matB(3, 3); // Matriz secundária
    string comando;

    exibirAjuda();

    while (true) {
        cout << "\nDigite um comando: ";
        getline(std::cin, comando);

        if (comando == "sair") {
            cout << "Saindo do programa...";
            break;
        } else if (comando == "AJUDA") {
            exibirAjuda();
        } else if (comando.rfind("inserir", 0) == 0) {
            // Comando para inserir valor
            int linha, coluna;
            double valor;
            if (scanf(comando.c_str(), "inserir %d %d %lf", &linha, &coluna, &valor) == 3) {
                try {
                    mat.insert(linha, coluna, valor);
                    cout << "Valor " << valor << " inserido na posicao (" << linha << ", " << coluna << ")." << endl;
                } catch (const exception& e) {
                    cerr << "Erro: " << e.what() << endl;
                }
            } else {
                cerr << "Comando inválido. Utilize: inserir linha + coluna + valor" << endl;
            }
        } else if (comando.rfind("obter", 0) == 0) {
            // Comando para obter valor
            int linha, coluna;
            if (scanf(comando.c_str(), "obter %d %d", &linha, &coluna) == 2) {
                try {
                    double valor = mat.get(linha, coluna);
                    cout << "Valor na posicao (" << linha << ", " << coluna << "): " << valor << endl;
                } catch (const exception& e) {
                    cerr << "Erro: " << e.what() << endl;
                }
            } else {
                cerr << "Comando invalido. Utilize: obter linha +  coluna" << endl;
            }
        } else if (comando == "imprimir") {
            // Comando para imprimir a matriz
            cout << "\nMatriz esparsa: " << endl;
            mat.printar_matrizEsparsa();
            cout << endl;
            matB.printar_matrizEsparsa();
        } else if (comando.rfind("ler", 0) == 0) {
            // Comando para ler matriz do arquivo
            string nome_arquivo;
            if (scanf(comando.c_str(), "ler %s", &nome_arquivo[0]) == 1) {
                readSparseMatrix(mat, nome_arquivo);
            } else {
                cerr << "Comando invalido. Utilize: ler nome_arquivo" << std::endl;
            }
        } else if (comando == "somar") {
            // Comando para somar matrizes
            try {
                SparseMatrix resultado = sum(mat, matB);
                cout << "Resultado da soma:" << endl;
                resultado.printar_matrizEsparsa();
            } catch (const exception& e) {
                cerr << "Erro: " << e.what() << endl;
            }
        } else if (comando == "multiplicar") {
            // Comando para multiplicar matrizes
            try {
                SparseMatrix resultado = multiply(mat, matB);
                cout << "Resultado da multiplicação:" << endl;
                resultado.printar_matrizEsparsa();
            } catch (exception& e) {
                cerr << "Erro: " << e.what() << endl;
            }
        } else if (comando.rfind("remover", 0) == 0) {
            // Comando para remover valor
            int linha, coluna, value;
            if (sscanf(comando.c_str(), "remover %d %d %lf", &linha, &coluna, &value) == 2) {
                try {
                    mat.remover_valor(linha, coluna, value ); // Chama a função para remover o valor
                    cout << "Valor removido na posição (" << linha << ", " << coluna << ")." << endl;
                } catch (const std::exception& e) {
                    cerr << "Erro: " << e.what() << endl;
                }
            } else {
                cerr << "Comando inválido. Utilize: remover linha + coluna" << endl;
            }
        } else {
            std::cerr << "Comando não reconhecido. Digite AJUDA para listar os comandos disponíveis." << std::endl;
        }
    }

    return 0;
}
