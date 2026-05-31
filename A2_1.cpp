#include <iostream>
#include <fstream>
#include "Grafo.h"
#include "functions.h"

int main() {
    structures::Grafo<int> meuGrafo;
    
    std::ifstream arquivo("grafo_teste.txt");
    if (arquivo.is_open()) {
        meuGrafo.ler(arquivo);
        arquivo.close();
        
        auto componentes = AcharCFC(meuGrafo);
        
        for (const auto& componente : componentes) {
            bool primeiro = true; // Variável para rastrear o primeiro elemento
            
            for (const auto& vertice : componente) {
                if (!primeiro) {
                    std::cout << ","; // Imprime a vírgula antes do vértice (se não for o 1º)
                }
                std::cout << vertice;
                primeiro = false; // Após o 1º loop, nenhum outro será o primeiro
            }
            std::cout << "\n";
        }
    } else {
        std::cerr << "Erro ao abrir o arquivo de teste!" << std::endl;
    }

    return 0;
}