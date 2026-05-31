// Grafo.h
#ifndef GRAFO_H
#define GRAFO_H

// Copyright [2026] Carolina Adilino
#include <list>
#include <algorithm>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

//namespace = uma região do código 
namespace structures {


    //Template = uma função sem tipo. imagina q qnd rodar o código "T" vira int ou float
    template<typename T>
    class Grafo{
        public:
            // Construtor padrão
            Grafo() = default;

            //método retorna quantidade de vértices
            int qtdVertices() const;

            //retorna a quantidade de arestas
            int qtdArestas() const;

            //retorna o grau do vértice v
            int grau(const T& v) const;

            //retorna o rótulo do vértice v
            int rotulo(const T& index) const;

            //retorna os vizinhos do vértice v
            std::list<T> vizinhos(const T& index) const;

            //se {u, v} ∈ E, retorna verdadeiro e se não existir, retorna falso
            bool haAresta(const T& index1, const T& index2) const;

            //se {u, v} ∈ E, retorna o peso da aresta {u, v} se não existir, retorna um valor infinito positivo
            int peso(const T& index1, const T& index2) const;

            // deve carregar um grafo a partir de um arquivo no formato especificado ao final deste documento.
            void ler(std::ifstream &arquivo);
        
        public:
            struct Vertice {
                T rotulo; //rotulo do vertice
                std::list<std::pair<T, int>> vizinhos; // lista com pares <id do vizinho, peso da aresta>
            };
            
            //no map cada elemento apresenta uma key T e um vértice
            std::map<T, Vertice> vertices;

    };


    //Função retorna quantidade de vértices 
    template<typename T>
    int structures::Grafo<T>::qtdVertices() const{
        return vertices.size();
    }

    template<typename T>
    int structures::Grafo<T>::qtdArestas() const{
        int contador = 0;
        for (const auto par : vertices){
            contador += par.second  .vizinhos.size();
        }
        return contador/2; //grafo não direcionado, vai contar tudo 2 vezes
    }
    
    template<typename T>
    int structures::Grafo<T>::grau(const T& index) const{
        return vertices.at(index).vizinhos.size();
    }

    template<typename T>
    int structures::Grafo<T>::rotulo(const T& index) const{
        return vertices.at(index).rotulo;
    }

    template<typename T>
    std::list<T> structures::Grafo<T>::vizinhos(const T& index) const{
        std::list<T> lista_vizinhos;
        for (auto par : vertices.at(index).vizinhos){
            lista_vizinhos.push_back(par.first);
        }
        return lista_vizinhos;
    }

    template<typename T>
    bool structures::Grafo<T>::haAresta(const T& index1, const T& index2) const{
        for (auto par : vertices.at(index1).vizinhos){
            if (par.first == index2){
                return true;
            }
        }
        return false;
    }

    template<typename T>
    int structures::Grafo<T>::peso(const T& index1, const T& index2) const{
        for (auto par : vertices.at(index1).vizinhos){
            if (par.first == index2){
                return par.second;
            }
        }
        return 100000; //MUDAR PARA INFINITO
    }
    
               
        
    template<typename T>
    void structures::Grafo<T>::ler(std::ifstream &arquivo) {
        std::string linha;
        bool dirigido = false; // Flag para controlar se é *edges ou *arcs

        // Limpa o grafo atual caso a função seja chamada mais de uma vez
        vertices.clear();

        while (std::getline(arquivo, linha)) {
            // Ignora linhas vazias ou apenas com espaços
            if (linha.empty() || linha.find_first_not_of(" \r\n\t") == std::string::npos) {
                continue;
            }

            if (linha.find("*vertices") != std::string::npos) {
                std::stringstream ss(linha);
                std::string comando;
                int n;
                ss >> comando >> n; // Extrai a palavra "*vertices" e o número 'n'

                // Lê as próximas 'n' linhas contendo: id rotulo
                for (int i = 0; i < n; i++) {
                    std::getline(arquivo, linha);
                    std::stringstream ss_vertice(linha);
                    T id;
                    
                    // Lê o índice do vértice
                    ss_vertice >> id; 
                    
                    // Inicializa a lista do vértice no map
                    vertices[id] = {};

                    // Se a sua estrutura suportar ler o rótulo, você pode descomentar a linha abaixo:
                    // ss_vertice >> vertices[id].rotulo;
                }
            } 
            else if (linha.find("*edges") != std::string::npos) {
                dirigido = false; // Grafo não direcionado (vai e volta)
            } 
            else if (linha.find("*arcs") != std::string::npos) {
                dirigido = true;  // Grafo direcionado (apenas ida)
            } 
            else {
                // Se não é nenhum comando acima, é uma linha de aresta: u v peso
                std::stringstream ss_aresta(linha);
                T u, v;
                int peso;

                if (ss_aresta >> u >> v >> peso) {
                    // Adiciona a aresta u -> v
                    vertices[u].vizinhos.push_back({v, peso});
                    
                    // Se for *edges (não dirigido), adiciona a volta v -> u
                    if (!dirigido) {
                        vertices[v].vizinhos.push_back({u, peso});
                    }
                }
            }
        }
    }
    //pra debuggar: std::cout << linha << std::endl;

}

#endif