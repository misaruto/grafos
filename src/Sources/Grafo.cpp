#include "../Headers/Grafo.h"
#include <fstream>

void Grafo::iniciar_variaveis(){
  this->nr_vertice = 0;
  this->nr_aresta = 0;
  this->in_orientado = false;
  this->in_possui_ciclo = false;
  this->in_multi_grafo = false;
  this->in_bipartido = false;
  this->in_pseudo_grafo = false;
  this->in_conexo = true;
  this->in_completo = true;
  this->in_simples = true;
}



Grafo::Grafo()
{
  FileManipulator *file = new FileManipulator();
  this->main(file,file->get_nm_file());
  delete file;
}

Grafo::Grafo(string fileName){
  FileManipulator *file = new FileManipulator(fileName);
  this->main(file,file->get_nm_file());
  delete file;
}

Grafo::Grafo(string fileName,string nm_grafo){
  FileManipulator *file = new FileManipulator(fileName);
  this->main(file,nm_grafo);
  delete file;
}

Grafo::Grafo(FileManipulator *file){
  this->main(file,file->get_nm_file());
}

void Grafo::main(FileManipulator *file,string nm_grafo){
  this->nm_grafo = nm_grafo;
  this->nm_path = file->get_nm_file();
  this->iniciar_variaveis();
  this->importa_arquivo(file);
  this->normaliza_entrada();
  this->gerar_matrizes();
  this->verifica_multigrafo();
  this->verifica_conexo();
  this->verifica_completo();
  this->verifica_pseudo();
  cout << this->gerar_relatorio();
}


Grafo::~Grafo(){
  delete this->matriz_entrada_grafo;
  delete this->matriz_adjacencia;
  delete this->matriz_incidencia;
  delete this->matriz_fecho_transitivo;
}

void Grafo::importa_arquivo(FileManipulator *FILE){
  string line = "";
  line = FILE->get_next_line();
  if (line == "direcionado"){
    this->in_orientado = true;
    this->in_simples = false;
    line = FILE->get_next_line();
  }
  this->nr_vertice = stoi(line);
  line = FILE->get_next_line();
  this->nr_aresta = stoi(line);

  this->instanciar_matrizes();
  int indice = 0;
  FILE->for_each(
    [&](string line, int i, string *lines) {
      string aux[3] = {"","",""};
      int len = 0;
      for (char &caracter : line){
        if(caracter == ' '){
          len++;
        }
        else{
          aux[len] += caracter;
        }
      }
      if(aux[0] != ""){
        this->matriz_entrada_grafo->insert(indice, 0, stoi(aux[0]));
      }
      if(aux[1] != ""){
        this->matriz_entrada_grafo->insert(indice, 1, stoi(aux[1]));
      }else{
        this->matriz_entrada_grafo->insert(indice, 0, 0);
      }
      if (aux[2] != ""){
        this->matriz_entrada_grafo->insert(indice, 2, stoi(aux[2]));
      }else if (aux[1] != "" and aux[0] != ""){
        this->matriz_entrada_grafo->insert(indice, 2, 1);
      }
      indice++;
    }
  );
  this->matriz_entrada_grafo->exibir_matriz();
}

void Grafo::instanciar_matrizes()
{
  this->matriz_entrada_grafo = new Matriz(this->nr_aresta, 3, "Matriz de Entrada");
  this->matriz_adjacencia = new Matriz(this->nr_vertice, this->nr_vertice, "Matriz de Adjacencia");
  this->matriz_incidencia = new Matriz(this->nr_aresta, this->nr_vertice, "Matriz de Incidencia");
  this->matriz_fecho_transitivo = new Matriz(
    this->nr_vertice,
    this->nr_vertice,
    "Matriz de Fecho Transitivo"
  );
  this->matriz_distancias = new Matriz(this->nr_vertice, this->nr_vertice, "Matriz de Distancias");
  this->matriz_pesos = new Matriz(this->nr_vertice, this->nr_vertice, "Matriz de Pesos");
}

void Grafo::normaliza_entrada(){
    int sub = 1;
    this->matriz_entrada_grafo->for_each(
      [&](int valor, int x,int y, int ** matriz){
        if(valor == 0){
          sub = 0;
          return;
        }
      }
    );
    this->matriz_entrada_grafo->for_each_row(
        [&](int *valor, int indice, int **matriz){
        valor[0] = valor[0] - sub;
        valor[1] = valor[1] - sub;
      }
    );
}

void Grafo::gerar_matrizes()
{
  this->matriz_entrada_grafo->for_each_row(
      [&](int *valor, int indice, int **matriz)
      {
        this->gerar_matriz_adjacencia(matriz[indice][0] , matriz[indice][1], matriz[indice][2]);
        this->gerar_matriz_incidencia(indice, matriz[indice][0], matriz[indice][1]);
      });
  this->executa_warshall();
  this->gerar_matriz_distancia();
};

void Grafo::gerar_matriz_adjacencia(int i, int j, int peso)
{
  if(peso == 0){
    peso = 1;
  }
  this->matriz_adjacencia->insert(i, j, peso);
  if(!this->in_orientado){
    this->matriz_adjacencia->insert(j, i, peso);
  }
};

void Grafo::gerar_matriz_incidencia(int indice, int i, int j)
{
  this->matriz_incidencia->insert(indice, j, 1);
  this->matriz_incidencia->insert(indice, i, 1);
}

int Grafo::percorre_distancia(int inicio, int dest, int distancia, int &total_percorrido,int *percorridos){
  for(int i = 0; i < total_percorrido;i++){
    if (inicio == percorridos[i] || dest == percorridos[i]){
      this->in_possui_ciclo = true;
      return -1;
    }
  }
  percorridos[total_percorrido] = inicio;
  total_percorrido= total_percorrido+1;

  Matriz * percorrer = new Matriz(this->nr_vertice,3);
  
  int size=0;
  this->matriz_entrada_grafo->for_each_row(
    [&](int * linha, int indice, int ** matriz){
      if((linha[0] == inicio && linha [1] != inicio) || ((linha[1] == inicio && linha[0] != inicio) && !this->in_orientado)){
        percorrer->insert(linha);
        size ++;
      }
    }
  );

  int * distancias = new int[size];
  for (int indice =0; indice < size; indice++){
    distancias[indice] = -1;
    int * linha = percorrer->get_row(indice);
    
    if (linha[0] == dest || linha[1] == dest){
      distancias[indice] = linha[2];
    }
    else{
      if(linha[0] != inicio){
        distancias[indice] = this->percorre_distancia(linha[0], dest, linha[2], total_percorrido, percorridos);
      }else{
        distancias[indice] = this->percorre_distancia(linha[1], dest, linha[2], total_percorrido, percorridos);
      }
    }
  }
  int aux =-1;
  for (int i = 0; i<size;i++){
    if (distancias[i] > 0){
      if(aux <= 0){
        aux = distancias[i];
      }else{
        if(aux > distancias[i]){
          aux = distancias[i];
        }
      }
    }
  }
  for(int i = 0; i < total_percorrido;i++){
    if (inicio == percorridos[i] || dest == percorridos[i]){
      percorridos[i] = 0;
      total_percorrido= total_percorrido-1;
    }
  }

  if(aux <= 0){
    return 0;
  }
  return aux + distancia;
}

/// @brief RODA RÁPIDO NO MEU Ryzen 7 7700x
void Grafo::gerar_matriz_distancia(){
  for (int inicio = 0; inicio < this->nr_vertice; inicio++){
    for (int dest = 0; dest < this->nr_vertice; dest++){
      if(inicio != dest){
        int * percorridos = new int[this->nr_vertice];
        int total_percorrido = 0;
        int distancia = this->percorre_distancia(inicio,dest,0,total_percorrido,percorridos);
        this->matriz_distancias->insert(inicio,dest,distancia);
      }
    }
  }
}

void Grafo::executa_warshall(){
  for(int k =0; k<this->nr_vertice; k++){
    for(int i =0; i<this->nr_vertice; i++){
      for(int j =0; j<this->nr_vertice; j++){
        if(this->matriz_adjacencia->get_item(i,k) && this->matriz_adjacencia->get_item(k,j)){
          this->matriz_fecho_transitivo->insert(i,j,1);
          this->matriz_pesos->insert(i,j,this->matriz_adjacencia->get_item(i,k) +this->matriz_adjacencia->get_item(k,j));
        }
      }
    }
  }
};

void Grafo::verifica_multigrafo() {
  this->matriz_incidencia->for_each_row(
    [&](int*line, int indice, int **matriz){
      int coluna1 =-1;
      int coluna2 =-1;
      this->matriz_incidencia->for_each_column(indice,
        [&](int valor, int y){
          if(valor){
            if(coluna1 == -1){
              coluna1 = y;
            }else{
              coluna2 = y;
            }
          }
        }
      );
      for(int i=0; i<this->nr_aresta;i++){
        if(i!=indice && matriz[i][coluna1] && matriz[i][coluna2]){
          this->in_multi_grafo = true;
          return;
        }
      }
    }
  );
}

void Grafo::verifica_conexo(){
  for(int j=0;j<this->nr_vertice;j++ ){
    int soma = 0;
    for(int i=0;i<this->nr_aresta;i++){
      soma += this->matriz_incidencia->get_item(i,j);
    }
    if(soma < 2){
      this->in_conexo = false;
      return;
    }
  }
};

void Grafo::verifica_completo(){
  for (int i=0;i<this->nr_vertice;i++){
    for (int j=0;j<this->nr_vertice;j++){
      if(i!=j && !this->matriz_adjacencia->get_item(i,j)){
        this->in_completo = false;
        return;
      }
    }
  }
};

void Grafo::verifica_pseudo(){
  for (int i = 0; i < this->nr_vertice; i++){
    if(this->matriz_adjacencia->get_item(i,i)){
      this->in_pseudo_grafo = true;
      return;
    }
  }
}

string Grafo::gerar_relatorio(){
  string aux = "";
  aux += "Relatorio do Grafo " + this->nm_grafo + "\n";
  aux += "    Arquivo: "+this->nm_path+"\n";
  aux += "    Vertices: "+to_string(this->nr_vertice)+"\n";
  aux += "    Arestas: "+to_string(this->nr_aresta)+"\n";
  aux += "    ";
  if(this->in_orientado){
    aux += "Grafo Orientado ";
  }else{
    aux += "Grafo Não Orientado ";
  }
  aux += "\n    ";
  if(this->in_simples){
    aux += "Grafo Simples ";
  }else{
    aux += "Grafo Complexo ";
  }
  aux += "\n    ";
  if(this->in_completo){
    aux += "Grafo Completo ";
  }else{
    aux += "Grafo Incompleto ";
  }
  aux += "\n    ";
  if(this->in_conexo){
    aux += "Grafo Conexo ";
  }else{
    aux += "Grafo Desconexo ";
  }
  aux += "\n    ";
  if(this->in_pseudo_grafo){
    aux += "É pseudo grafo ";
  }else{
    aux += "Não é pseudo grafo ";
  }
  aux += "\n    ";
  if(this->in_bipartido){
    aux += "Grafo bipartido ";
  }else{
    aux += "Grafo não bipartido ";
  }
  aux += "\n    ";
  if(this->in_multi_grafo){
    aux += "Grafo multigrafo ";
  }else{
    aux += "Grafo não multigrafo ";
  }
  aux += "\n    ";
  if(this->in_possui_ciclo){
    aux += "Grafo possui ciclo ";
  }else{
    aux += "Grafo não possui ciclo ";
  }
  aux += "\n\n\n";
  aux += "Matrizes do grafo: \n";
  aux += this->matriz_entrada_grafo->to_string();
  aux += this->matriz_adjacencia->to_string();
  aux += this->matriz_incidencia->to_string();
  aux += this->matriz_fecho_transitivo->to_string();
  aux += this->matriz_distancias->to_string();
  aux += this->matriz_pesos->to_string();
  aux += "\n\n\n";
  return aux;
}

void Grafo::exibe_matrizes(){
  this->matriz_adjacencia->exibir_matriz();
  cout << endl;
  this->matriz_incidencia->exibir_matriz();
  cout << endl;
  this->matriz_fecho_transitivo->exibir_matriz();
  cout << endl;
  this->matriz_distancias->exibir_matriz();
  cout << endl;
  this->matriz_pesos->exibir_matriz();
  cout << this->in_multi_grafo<<endl;
  cout << this->in_conexo<<endl;
}
void Grafo::salvar(string filename){
  FileManipulator *file = new FileManipulator(filename,"w");
  file->write(this->gerar_relatorio());
}
void Grafo::salvar(){
  FileManipulator *file = new FileManipulator("","w");
  file->write(this->gerar_relatorio());
}

