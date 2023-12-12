#include "../Headers/Matriz.h"

#include <functional>
#include <iostream>
#include <iomanip>

using namespace std;

#define ANSI_RESET "\033[0m"
#define ANSI_BOLD "\033[1m"
#define ANSI_COLOR_RED "\033[31m"
#define ANSI_COLOR_GREEN "\033[32m"
#define ANSI_COLOR_YELLOW "\033[33m"
#define ANSI_COLOR_BLUE "\033[34m"
#define ANSI_COLOR_MAGENTA "\033[35m"
#define ANSI_COLOR_CYAN "\033[36m"

Matriz::Matriz()
{
  this->x = 0;
  this->y = 0;
  this->actualX = 0;
  this->actualY = 0;
  this->matriz = NULL;
  this->nm_matriz = "";
  this->in_instanciada = false;
}
Matriz::Matriz(string nm_matriz)
{
  this->x = 0;
  this->y = 0;
  this->actualX = 0;
  this->actualY = 0;
  this->matriz = NULL;
  this->in_instanciada = false;
  this->nm_matriz = nm_matriz;
}
Matriz::Matriz(int x, int y, string nm_matriz)
{
  this->x = x;
  this->y = y;
  this->actualX = 0;
  this->actualY = 0;
  this->nm_matriz = nm_matriz;
  this->inicia_matriz();
}
Matriz::Matriz(int x, int y)
{
  this->x = x;
  this->y = y;
  this->actualX = 0;
  this->actualY = 0;
  this->nm_matriz = "";
  this->inicia_matriz();
}
Matriz::~Matriz()
{
  for (int i = 0; i < this->x; i++)
  {
    delete[] this->matriz[i];
  }
  delete[] this->matriz;
}

void Matriz::insert(int * list){
  if(this->actualX > this->x){
    cout << "tentando inserir acima do limite";
    return;
  }
  this->matriz[actualX] = list;
  actualX ++;
};
void Matriz::insert(int valor){
  this->matriz[actualX][actualY] = valor;
  this->actualX ++;
  this->actualY ++;
};


void Matriz::inicia_matriz(){
  this->matriz = new int *[this->x];
  for (int i = 0; i < this->x; i++){
    matriz[i] = new int[this->y];
  }

  for (int i = 0; i < this->x; i++){
    for (int j = 0; j < this->y; j++){
      matriz[i][j] = 0;
    }
  }
  this->in_instanciada = true;
}

int **Matriz::get_matriz() { return this->matriz; }
void Matriz::insert(int x, int y, int valor) { matriz[x][y] = valor; }
int Matriz::get_item(int x, int y) { return matriz[x][y];  }
int* Matriz::get_row(int x) { return matriz[x]; }

void Matriz::exibir_matriz(){
  cout << "Exibindo " << this->nm_matriz << endl;
  cout << "    ";
  for (int j = 0; j < this->y; ++j) {
    cout << ANSI_BOLD << ANSI_COLOR_GREEN << setw(4) << j << ANSI_RESET;
  }
  cout << endl;

  for (int i = 0; i < this->x; ++i) {
    cout << ANSI_BOLD << ANSI_COLOR_BLUE << setw(4) << i << ANSI_RESET;
    for (int j = 0; j < this->y; ++j) {
      cout << setw(4) << matriz[i][j];
    }
    cout << endl;
  }
}

string Matriz::to_string(){
  string aux = "";
  aux += "Dados da " + this->nm_matriz + "\n";

  for (int i = 0; i < this->x; ++i) {
    for (int j = 0; j < this->y; ++j) {
      aux += "    " + std::to_string(this-> matriz[i][j]);
    }
    aux += "\n";
  }
  aux += "\n";
  return aux;
}

void Matriz::for_each(function<void(int, int, int, int **)> callback){
  for (int i = 0; i < this->x; i++){
    for (int j = 0; j < this->y; j++){
      callback(this->matriz[i][j], i, j, this->matriz);
    }
  }
}

void Matriz::for_each_row(function<void(int *, int, int **)> callback){
  for (int i = 0; i < this->x; i++){
    callback(this->matriz[i], i, this->matriz);
  }
}
void Matriz::for_each_column(int indice,function<void(int, int)> callback){
  for(int j = 0; j < this->y; j++){
    callback(this->matriz[indice][j], j);
  }
};
// int Matriz::find(function<bool(int, int, int **)> callback){
//   for (int i = 0; i < this->x; i++)
//   {
//     for (int j = 0; j < this->y; j++)
//     {
//       if(callback(i, j, matriz)){
//         return matriz[i][j];
//       }
//     }
//   }
// };

// int* Matriz::find_row(function<bool(int *, int, int **)> callback){
//   for (int i = 0; i < this->x; i++)
//   {
//     if(callback(matriz[i], i, matriz)){
//       return matriz[i];
//     }
//   }
// };