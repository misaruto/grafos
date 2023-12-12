#ifndef MATRIZ_H
#define MATRIZ_H
#include <functional>
#include <iostream>
using namespace std;
class Matriz{
private:
  int x;
  int y;
  int actualX;
  int actualY;
  string nm_matriz;
  int **matriz;
  void inicia_matriz();

public:
  bool in_instanciada;
  void insert(int x, int y, int valor);
  void insert(int * list);
  void insert(int valor);
  int **get_matriz();
  int get_item(int x, int y);
  int *get_row(int x);
  void for_each(function<void(int, int, int, int **)> callback);
  void for_each_row(function<void(int *, int, int **)> callback);
  void for_each_column(int indice, function<void(int, int)> callback);
  void exibir_matriz();
  string to_string();

  Matriz();
  Matriz(int x, int y);
  Matriz(string nmMatriz);
  Matriz(int x, int y, string nmMatriz);
  // int find(function<bool(int, int, int **)> callback);
  // int * find_row(function<bool(int *, int, int **)> callback);
  ~Matriz();
};
#endif
