#include <algorithm>
#include <chrono> // Para medir o tempo
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Estrutura para armazenar cada linha do CSV
struct csvRow {
  vector < string > columns;
};

// Protótipos das funções
vector < csvRow > lerCSV(const string & inputFile, vector < string > & header);
void ordenarBubbleSort(vector < csvRow > & dados, int indiceColuna);
void ordenarSelectionSort(vector < csvRow > & dados, int indiceColuna);
void ordenarMergeSort(vector < csvRow > & dados, int inicio, int fim, int indiceColuna);
void salvarCSV(const vector < csvRow > & dados,
  const vector < string > & header,
    const string & nomeArquivoSaida);

int main() {
  string inputFile = "focos_diario_br_20241007.csv";
  vector < string > header;
  vector < csvRow > dados = lerCSV(inputFile, header);

  if (dados.empty()) {
    cerr << "Nenhum dado foi lido." << endl;
    return 1;
  }

  int colunaParaOrdenar;
  cout << "Digite o índice da coluna (0 a " << header.size() - 1 <<
    ") pela qual deseja ordenar: ";
  cin >> colunaParaOrdenar;

  if (colunaParaOrdenar < 0 || colunaParaOrdenar >= header.size()) {
    cerr << "Índice de coluna inválido!" << endl;
    return 1;
  }

  int algoritmo;
  cout << "Escolha o algoritmo de ordenação (1 - Bubble Sort, 2 - "
  "Selection Sort, 3 - Merge Sort): ";
  cin >> algoritmo;

  // Medir o tempo de execução da ordenação
  auto inicio = chrono::steady_clock::now();

  switch (algoritmo) {
  case 1:
    ordenarBubbleSort(dados, colunaParaOrdenar);
    break;
  case 2:
    ordenarSelectionSort(dados, colunaParaOrdenar);
    break;
  case 3:
    ordenarMergeSort(dados, colunaParaOrdenar);
    break;
  default:
    cerr << "Algoritmo inválido!" << endl;
    return 1;
  }

  // Medir o tempo de execução após a ordenação
  auto fim = chrono::steady_clock::now();
  chrono::duration < double > duracao = fim - inicio;

  cout << "Tempo de execução da ordenação: " << duracao.count() <<
    " segundos" << endl;

  // Salva os dados ordenados em um novo file CSV
  string nomeArquivoSaida = "sorted_" + inputFile;
  salvarCSV(dados, header, nomeArquivoSaida);

  cout << "Dados ordenados salvos em " << nomeArquivoSaida << endl;

  return 0;
}

// Função para ler o file CSV
vector < csvRow > lerCSV(const string & inputFile, vector < string > & header) {
  ifstream file(inputFile);
  vector < csvRow > dados;
  string linha;

  if (!file.is_open()) {
    cerr << "Erro ao abrir o file!" << endl;
    return dados;
  }

  // Lê a primeira linha como cabeçalho
  if (getline(file, linha)) {
    stringstream ss(linha);
    string coluna;
    while (getline(ss, coluna, ',')) {
      header.push_back(coluna);
    }
  }

  // Lê cada linha do file
  while (getline(file, linha)) {
    csvRow linhaDados;
    stringstream ss(linha);
    string coluna;

    // Divide a linha em columns
    while (getline(ss, coluna, ',')) {
      linhaDados.columns.push_back(coluna);
    }

    dados.push_back(linhaDados);
  }

  file.close();
  return dados;
}

// Função Bubble Sort
void ordenarBubbleSort(vector < csvRow > & dados, int indiceColuna) {
  int n = dados.size();
  for (int i = 0; i < n - 1; ++i) {
    for (int j = 0; j < n - i - 1; ++j) {
      if (dados[j].columns[indiceColuna] > dados[j + 1].columns[indiceColuna]) {
        swap(dados[j], dados[j + 1]);
      }
    }
  }
}

// Função Selection Sort
void ordenarSelectionSort(vector < csvRow > & dados, int indiceColuna) {
  int n = dados.size();
  for (int i = 0; i < n - 1; ++i) {
    int minIndex = i;
    for (int j = i + 1; j < n; ++j) {
      if (dados[j].columns[indiceColuna] <
        dados[minIndex].columns[indiceColuna]) {
        minIndex = j;
      }
    }
    swap(dados[i], dados[minIndex]);
  }
}

// Função Merge Sort (recursiva)
void merge(vector < csvRow > & dados, int inicio, int meio, int fim,
  int indiceColuna) {
  int n1 = meio - inicio + 1;
  int n2 = fim - meio;

  vector < csvRow > esquerda(n1), direita(n2);

  for (int i = 0; i < n1; ++i)
    esquerda[i] = dados[inicio + i];
  for (int j = 0; j < n2; ++j)
    direita[j] = dados[meio + 1 + j];

  int i = 0, j = 0, k = inicio;
  while (i < n1 && j < n2) {
    if (esquerda[i].columns[indiceColuna] <= direita[j].columns[indiceColuna]) {
      dados[k] = esquerda[i];
      i++;
    } else {
      dados[k] = direita[j];
      j++;
    }
    k++;
  }

  while (i < n1) {
    dados[k] = esquerda[i];
    i++;
    k++;
  }

  while (j < n2) {
    dados[k] = direita[j];
    j++;
    k++;
  }
}

void ordenarMergeSort(vector<csvRow> &dados, int indiceColuna) {
  int n = dados.size();

  // Tamanho atual da sub-lista a ser mesclada
  for (int tamanhoAtual = 1; tamanhoAtual <= n - 1; tamanhoAtual *= 2) {
    // Escolhe o ponto inicial para a sub-lista atual
    for (int inicioEsquerda = 0; inicioEsquerda < n - 1; inicioEsquerda += 2 * tamanhoAtual) {
      int meio = min(inicioEsquerda + tamanhoAtual - 1, n - 1);
      int fimDireita = min(inicioEsquerda + 2 * tamanhoAtual - 1, n - 1);

      // Mescla as duas metades
      merge(dados, inicioEsquerda, meio, fimDireita, indiceColuna);
    }
  }
}

// Função para salvar dados ordenados em um novo file CSV
void salvarCSV(const vector < csvRow > & dados,
  const vector < string > & header,
    const string & nomeArquivoSaida) {
  ofstream arquivoSaida(nomeArquivoSaida);
  if (!arquivoSaida.is_open()) {
    cerr << "Erro ao abrir o file de saída!" << endl;
    return;
  }

  // Escreve o cabeçalho
  for (size_t i = 0; i < header.size(); ++i) {
    arquivoSaida << header[i];
    if (i < header.size() - 1)
      arquivoSaida << ",";
  }
  arquivoSaida << "\n";

  // Escreve os dados
  for (const auto & linha: dados) {
    for (size_t i = 0; i < linha.columns.size(); ++i) {
      arquivoSaida << linha.columns[i];
      if (i < linha.columns.size() - 1)
        arquivoSaida << ",";
    }
    arquivoSaida << "\n";
  }

  arquivoSaida.close();
}
