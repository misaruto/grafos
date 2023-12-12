#ifndef GRAFO_H
#define GRAFO_H
#include <iostream>

#include "./Matriz.h"
#include "./FileManipulator.h"
using namespace std;

class Grafo
{
private:
  string nm_grafo;
  string nm_path;
  int nr_vertice;
  int nr_aresta;
  bool in_simples;
  bool in_completo;
  bool in_conexo;
  bool in_pseudo_grafo;
  bool in_bipartido;
  bool in_orientado;
  bool in_multi_grafo;
  bool in_possui_ciclo;

  Matriz *arvore_radicada;
  Matriz *arvore_geradora_minima;
  Matriz *arvore_caminhos_minimos;

  Matriz *matriz_entrada_grafo;
  Matriz *matriz_adjacencia;
  Matriz *matriz_incidencia;
  Matriz *matriz_fecho_transitivo;
  Matriz *matriz_distancias;
  Matriz *matriz_pesos;

  void iniciar_variaveis();
  void importa_arquivo(FileManipulator *file);
  void normaliza_entrada();
  void instanciar_matrizes();
  void gerar_matriz_adjacencia(int x, int y, int peso);
  void gerar_matriz_incidencia(int x, int y, int indice);
  void gerar_matriz_distancia();
  void executa_warshall();
  int percorre_distancia(int inicio, int dest, int distancia, int &total_percorrido, int * percorridos);
  void verifica_multigrafo();
  void verifica_conexo();
  void verifica_completo();
  void verifica_pseudo();
  string gerar_relatorio();
  void func(int*line, int indice, int **matriz);
  
  void main(FileManipulator *file,string nm_grafo);

public:
  Grafo();
  Grafo(string fileName);
  Grafo(string fileName,string nm_grafo);
  Grafo(FileManipulator *file);
  void gerar_matrizes();
  void exibe_matrizes();
  void salvar();
  void salvar(string filename);
  ~Grafo();
};

#endif
