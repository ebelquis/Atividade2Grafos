#include <iostream>
#include <fstream>
#include "Grafo.h"
#include "functions.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Erro: Faltou passar o nome do arquivo no terminal!" << std::endl;
        return 1;
    }
    structures::Grafo<int> meuGrafo;

    
    std::ifstream arquivo(argv[1]);
    if (arquivo.is_open()) {
        meuGrafo.ler(arquivo);
        arquivo.close();
        
        auto ordem_topologica = OrdenacaoTopologica(meuGrafo);
        
        if (ordem_topologica.empty()) {
            std::cout << "Grafo com ciclo :(";
            return 0;
        }

        bool primeiro = true;    
        for (const auto& vertice : ordem_topologica) {
            if (!primeiro) {
                std::cout << ", "; // parece que tem espaco antes e depois
            }
            std::cout << meuGrafo.rotulo(vertice);
            primeiro = false;
        }

    } else {
        std::cerr << "Erro ao abrir o arquivo de teste!" << std::endl;
    }

    return 0;
}