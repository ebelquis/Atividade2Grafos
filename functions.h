#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <vector>
#include <map>
#include "Grafo.h"


// Função DFS para o grafo original (gera a ordem de término)
template<typename T>
void BuscaProfundidade_Visita(const structures::Grafo<T>& G, const T& u, std::map<T, bool>& map_conhecido, std::vector<T>& ordem_fim);

// Função DFS adaptada para o grafo transposto (extrai o componente)
template<typename T>
void BuscaVisita_Transposto(const structures::Grafo<T>& G_linha, const T& u, std::map<T, bool>& map_conhecido, std::vector<T>& componente_atual);

// Cria um novo grafo com as arestas invertidas
template<typename T>
structures::Grafo<T> transporGrafo(const structures::Grafo<T>& G);

// Função Principal de Kosaraju
template<typename T>
std::vector<std::vector<T>> AcharCFC(const structures::Grafo<T>& G);

template<typename T>
void VisitaOT(const structures::Grafo<T>& G, const T& u, std::map<T, bool>& conhecido, std::map<T, int>& tempo_conhecido, std::map<T, int>& tempo_finalizado, int& tempo, std::vector<T>& ordem, bool& tem_ciclo);

// Função de Ordenacao topológica
template<typename T>
std::vector<T> OrdenacaoTopologica(const structures::Grafo<T>& G);


template<typename T>
void BuscaProfundidade_Visita(const structures::Grafo<T>& G, const T& u, std::map<T, bool>& map_conhecido, std::vector<T>& ordem_fim) {
    map_conhecido[u] = true;

    // G.vizinhos(u) retorna std::list<T>, facilitando a iteração
    for (const auto& v : G.vizinhos(u)) {
        if (!map_conhecido[v]) {
            BuscaProfundidade_Visita(G, v, map_conhecido, ordem_fim);
        }
    }
    
    // Quando não há mais para onde ir, o tempo de término do vértice chegou
    ordem_fim.push_back(u); 
}

template<typename T>
void BuscaVisita_Transposto(const structures::Grafo<T>& G_linha, const T& u, std::map<T, bool>& map_conhecido, std::vector<T>& componente_atual) {
    map_conhecido[u] = true;
    componente_atual.push_back(u);

    for (const auto& v : G_linha.vizinhos(u)) {
        if (!map_conhecido[v]) {
            BuscaVisita_Transposto(G_linha, v, map_conhecido, componente_atual);
        }
    }
}

template<typename T>
structures::Grafo<T> transporGrafo(const structures::Grafo<T>& G) {
    structures::Grafo<T> G_linha;

    // Inicializa todos os vértices no grafo transposto (garante que vértices isolados existam)
    for (const auto& par : G.vertices) {
        G_linha.vertices[par.first].rotulo = par.second.rotulo;
    }

    // Inverte o direcionamento das arestas
    for (const auto& par : G.vertices) {
        T u = par.first;
        for (const auto& par_vizinho : par.second.vizinhos) {
            T v = par_vizinho.first;
            int peso = par_vizinho.second;
            
            // Grafo original tinha aresta (u -> v). No transposto terá (v -> u)
            G_linha.vertices[v].vizinhos.push_back({u, peso});
        }
    }
    
    return G_linha;
}

template<typename T>
std::vector<std::vector<T>> AcharCFC(const structures::Grafo<T>& G) {
    std::map<T, bool> map_conhecido;
    std::vector<T> ordem_fim; // Funciona como uma pilha de finalização

    // 1º Passo: Inicializar os mapas para a primeira DFS
    for (const auto& par : G.vertices) {
        map_conhecido[par.first] = false;
    }

    // Executa a DFS em todos os vértices não visitados
    for (const auto& par : G.vertices) {
        T v = par.first;
        if (!map_conhecido[v]) {
            BuscaProfundidade_Visita(G, v, map_conhecido, ordem_fim);
        }
    }

    // 2º Passo: Obter o Grafo Transposto
    structures::Grafo<T> G_linha = transporGrafo(G);

    // 3º Passo: Limpar o controle de visitados para a segunda DFS
    for (const auto& par : G.vertices) {
        map_conhecido[par.first] = false;
    }

    std::vector<std::vector<T>> componentes_fortemente_conexos;

    // Executar a DFS no grafo transposto usando a ordem inversa de término
    // (Começamos a ler ordem_fim de trás para a frente com o iterador reverso)
    for (auto it = ordem_fim.rbegin(); it != ordem_fim.rend(); ++it) {
        T u = *it;
        if (!map_conhecido[u]) {
            std::vector<T> componente_atual;
            BuscaVisita_Transposto(G_linha, u, map_conhecido, componente_atual);
            
            // Adiciona o componente encontrado à lista geral
            componentes_fortemente_conexos.push_back(componente_atual);
        }
    }

    return componentes_fortemente_conexos;
}

// Função de Ordenacao topológica
template<typename T>
std::vector<T> OrdenacaoTopologica(const structures::Grafo<T>& G) {
    std::map<T, bool> conhecido;
    std::map<T, int> tempo_conhecido;
    std::map<T, int> tempo_finalizado;
    std::vector<T> ordem;
    bool tem_ciclo = false;

    for (const auto& par : G.vertices){
        conhecido[par.first] = false;
        tempo_conhecido[par.first] = 10000000;
        tempo_finalizado[par.first] = 10000000;
    }

    int tempo = 0;
    for (const auto& par : G.vertices) {
        T u = par.first;
        if (!conhecido.at(u) && !tem_ciclo) {
            VisitaOT(G, u, conhecido, tempo_conhecido, tempo_finalizado, tempo, ordem, tem_ciclo);
        }
    }

    // reverte pq usei push_back em VisitaOT
    if (!tem_ciclo) {
        std::reverse(ordem.begin(), ordem.end());
    }

    return ordem;
}

template<typename T>
void VisitaOT(const structures::Grafo<T>& G, const T& v, std::map<T, bool>& conhecido, std::map<T, int>& tempo_conhecido, std::map<T, int>& tempo_finalizado, int& tempo, std::vector<T>& ordem, bool& tem_ciclo) {
    if (tem_ciclo) return;

    conhecido.at(v) = true;
    tempo += 1;
    tempo_conhecido.at(v) = tempo;

    std::list<T> lista_vizinhos = G.vizinhos(v);

    for (const T& u : lista_vizinhos) {
        // se um vertice for conhecido e dps for encontrado novamente sem ser finalizado, existe ciclo
        if (tempo_conhecido.at(u) != 10000000 && tempo_finalizado.at(u) == 10000000) {
            tem_ciclo = true;
            ordem.clear();
            return;
        }
        
        if (tempo_conhecido.at(u) != 10000000 && tempo_finalizado.at(u) == 10000000) {
            tem_ciclo = true;
            ordem.clear();
            return;
        }
        if (!conhecido.at(u)) {
            VisitaOT(G, u, conhecido, tempo_conhecido, tempo_finalizado, tempo, ordem, tem_ciclo);
        }
    }

    if (tem_ciclo) return;

    tempo += 1;
    tempo_finalizado.at(v) = tempo;

    ordem.push_back(v);
}
#endif