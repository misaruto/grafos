#include <iostream>

#include "./src/Headers/Grafo.h"
using namespace std;

int main() {
  Grafo grafo = Grafo("./src/Entrada/grafo.txt","Grafo teste");
  grafo.salvar("./grafo.txt");
  return 0;
}