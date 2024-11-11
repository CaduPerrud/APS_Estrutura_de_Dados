#include <algorithm>
#include <chrono> // Biblioteca para medição de tempo com alta precisão
#include <cstdlib>
#include <iostream>
#include <vector>

// Protótipos das funções
void gerarValoresAleatorios(std::vector<int> &dados, int quantidade);
void bubbleSort(std::vector<int> &dados);
void merge(std::vector<int> &dados, int esquerda, int meio, int direita);
void mergeSort(std::vector<int> &dados, int esquerda, int direita);
void selectionSort(std::vector<int> &dados);
void ordenarDados(std::vector<int> &dados, int tipoOrdenacao);
void exibirDados(const std::vector<int> &dados);
void adicionarValoresManualmente(std::vector<int> &dados);

int main() {
  srand(static_cast<unsigned int>(time(0)));
  std::vector<int> dados;
  int opcao;

  std::cout << "Selecione uma opção:\n";
  std::cout << "1. Gerar valores aleatórios\n";
  std::cout << "2. Adicionar valores manualmente\n";
  std::cout << "Opção: ";
  std::cin >> opcao;

  if (opcao == 1) {
    int quantidade;
    std::cout << "Quantos valores aleatórios deseja gerar? ";
    std::cin >> quantidade;
    gerarValoresAleatorios(dados, quantidade);
  } else if (opcao == 2) {
    adicionarValoresManualmente(dados);
  } else {
    std::cout << "Opção inválida!\n";
    return 1;
  }

  std::cout << "\nEscolha o tipo de ordenação:\n";
  std::cout << "1. Bubble Sort\n";
  std::cout << "2. Merge Sort\n";
  std::cout << "3. Selection Sort\n";
  std::cout << "Opção: ";
  std::cin >> opcao;

  ordenarDados(dados, opcao);

  return 0;
}

void gerarValoresAleatorios(std::vector<int> &dados, int quantidade) {
  for (int i = 0; i < quantidade; ++i) {
    dados.push_back(rand() % 100); // Gera valores aleatórios de 0 a 99
  }
}

void bubbleSort(std::vector<int> &dados) {
  auto inicio = std::chrono::steady_clock::now(); // Início da medição de tempo
  for (size_t i = 0; i < dados.size() - 1; ++i) {
    for (size_t j = 0; j < dados.size() - i - 1; ++j) {
      if (dados[j] > dados[j + 1]) {
        std::swap(dados[j], dados[j + 1]);
      }
    }
  }
  auto fim = std::chrono::steady_clock::now(); // Fim da medição de tempo
  std::chrono::duration<double, std::milli> tempoGasto =
      fim - inicio; // Cálculo do tempo

  std::cout << "Dados ordenados com Bubble Sort: ";
  for (const auto &valor : dados) {
    std::cout << valor << " ";
  }
  std::cout << "\nTempo gasto: " << tempoGasto.count() << " ms\n";
}

void merge(std::vector<int> &dados, int esquerda, int meio, int direita) {
  int n1 = meio - esquerda + 1;
  int n2 = direita - meio;

  std::vector<int> esquerdaArr(n1), direitaArr(n2);

  for (int i = 0; i < n1; ++i)
    esquerdaArr[i] = dados[esquerda + i];
  for (int j = 0; j < n2; ++j)
    direitaArr[j] = dados[meio + 1 + j];

  int i = 0, j = 0, k = esquerda;
  while (i < n1 && j < n2) {
    if (esquerdaArr[i] <= direitaArr[j]) {
      dados[k++] = esquerdaArr[i++];
    } else {
      dados[k++] = direitaArr[j++];
    }
  }

  while (i < n1)
    dados[k++] = esquerdaArr[i++];
  while (j < n2)
    dados[k++] = direitaArr[j++];
}

void mergeSort(std::vector<int> &dados, int esquerda, int direita) {
  if (esquerda < direita) {
    int meio = esquerda + (direita - esquerda) / 2;
    mergeSort(dados, esquerda, meio);
    mergeSort(dados, meio + 1, direita);
    merge(dados, esquerda, meio, direita);
  }
}

void selectionSort(std::vector<int> &dados) {
  auto inicio = std::chrono::steady_clock::now(); // Início da medição de tempo
  for (size_t i = 0; i < dados.size() - 1; ++i) {
    size_t minIndex = i;
    for (size_t j = i + 1; j < dados.size(); ++j) {
      if (dados[j] < dados[minIndex]) {
        minIndex = j;
      }
    }
    std::swap(dados[i], dados[minIndex]);
  }
  auto fim = std::chrono::steady_clock::now(); // Fim da medição de tempo
  std::chrono::duration<double, std::milli> tempoGasto =
      fim - inicio; // Cálculo do tempo

  std::cout << "Dados ordenados com Selection Sort: ";
  for (const auto &valor : dados) {
    std::cout << valor << " ";
  }
  std::cout << "\nTempo gasto: " << tempoGasto.count() << " ms\n";
}

void ordenarDados(std::vector<int> &dados, int tipoOrdenacao) {
  if (tipoOrdenacao == 1) {
    bubbleSort(dados);
  } else if (tipoOrdenacao == 2) {
    auto inicio =
        std::chrono::steady_clock::now(); // Início da medição de tempo
    mergeSort(dados, 0, dados.size() - 1);
    auto fim = std::chrono::steady_clock::now(); // Fim da medição de tempo
    std::chrono::duration<double, std::milli> tempoGasto =
        fim - inicio; // Cálculo do tempo

    std::cout << "Dados ordenados com Merge Sort: ";
    for (const auto &valor : dados) {
      std::cout << valor << " ";
    }
    std::cout << "\nTempo gasto: " << tempoGasto.count() << " ms\n";
  } else if (tipoOrdenacao == 3) {
    selectionSort(dados);
  } else {
    std::cout << "Tipo de ordenação inválido!\n";
  }
}

void exibirDados(const std::vector<int> &dados) {
  std::cout << "Dados atuais: ";
  for (const auto &valor : dados) {
    std::cout << valor << " ";
  }
  std::cout << std::endl;
}

void adicionarValoresManualmente(std::vector<int> &dados) {
  int valor;
  std::cout
      << "Digite os valores a serem adicionados (digite -1 para encerrar): ";
  while (std::cin >> valor && valor != -1) {
    dados.push_back(valor);
  }
}