#include <stdio.h>
#include <stdlib.h>
typedef struct {
  int **matrizAdjacencia;
  int **matrizIncidencia;
  int **matrizFechoTransitivo;
  int **matrizDistancias;
  int **matrizPesos;
  int direcionado;
  int nrVertices;
  int nrArestas;
  int possuiCiclo;
} Grafo;

Grafo *criaMatriz(int nrVertices, int nrArestas,int direcionado) {
  Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));
  grafo->possuiCiclo = 0;
  grafo->nrVertices = nrVertices;
  grafo->nrArestas = nrArestas;
  grafo->direcionado = direcionado;
  // Aloca memória para a matriz de adjacências
  grafo->matrizAdjacencia = (int **)malloc(nrVertices * sizeof(int *));
  grafo->matrizDistancias = (int **)malloc(nrVertices * sizeof(int *));
  grafo->matrizFechoTransitivo = (int **)malloc(nrVertices * sizeof(int *));
  grafo->matrizIncidencia = (int **)malloc(nrArestas * sizeof(int *));
  grafo->matrizPesos = (int **)malloc(nrVertices * sizeof(int *));
  for (int i = 0; i < nrVertices; i++) {
    grafo->matrizAdjacencia[i] = (int *)malloc(nrVertices * sizeof(int));
    grafo->matrizDistancias[i] = (int *)malloc(nrVertices * sizeof(int));
    grafo->matrizPesos[i] = (int *)malloc(nrVertices * sizeof(int));
    grafo->matrizFechoTransitivo[i] = (int *)malloc(nrVertices * sizeof(int));
  }
  for (int i = 0; i < nrArestas; i++) {
    grafo->matrizIncidencia[i] = (int *)malloc(nrVertices * sizeof(int));
  }
  // Inicializa a matriz de adjacências
  for (int i = 0; i < nrVertices; i++) {
    for (int j = 0; j < nrVertices; j++) {
      grafo->matrizAdjacencia[i][j] = 0;
      grafo->matrizDistancias[i][j] = 0;
      grafo->matrizPesos[i][j] = 0;
      grafo->matrizFechoTransitivo[i][j] = 0;
    }
  }
  for (int i = 0; i < nrArestas; i++) {
    for (int j = 0; j < nrVertices; j++) {
      grafo->matrizIncidencia[i][j] = 0;
    }
  }

  return grafo;
}

void liberaMatriz(Grafo *grafo) {
  // Libera a memória alocada para a matriz de adjacências
  for (int i = 0; i < grafo->nrVertices; i++) {
    free(grafo->matrizAdjacencia[i]);
    free(grafo->matrizDistancias[i]);
    free(grafo->matrizFechoTransitivo[i]);
    free(grafo->matrizPesos[i]);
  }
  for (int i = 0; i < grafo->nrArestas; i++) {
    free(grafo->matrizIncidencia[i]);
  }
  free(grafo->matrizAdjacencia);
  free(grafo->matrizIncidencia);
  free(grafo->matrizDistancias);
  free(grafo->matrizFechoTransitivo);
  free(grafo->matrizPesos);
  free(grafo);
}

void adicionaAresta(Grafo *grafo, int u, int v,int peso, int i) {
  // Adiciona uma aresta na matriz de adjacências
  if(!peso){
    peso = 1;
  }
  grafo->matrizAdjacencia[u][v] = peso;
  if(!grafo->direcionado){
    grafo->matrizAdjacencia[v][u] = peso;  
  }
  grafo->matrizIncidencia[i][u] = 1;
  grafo->matrizIncidencia[i][v] = 1;
}

void executa_warshall(Grafo *grafo) {
  for (int k = 0; k < grafo->nrVertices; k++) {
    for (int i = 0; i < grafo->nrVertices; i++) {
      for (int j = 0; j < grafo->nrVertices; j++) {
        if (grafo->matrizAdjacencia[i][k] && grafo->matrizAdjacencia[k][j]) {
          grafo->matrizFechoTransitivo[i][j] = 1;
          grafo->matrizPesos[i][j] = grafo->matrizAdjacencia[i][k] +grafo->matrizAdjacencia[k][j];
        }
      }
    }
  }
}

int retorna_menor(int size, int * vet){
  int aux =-1;
    for (int i = 0; i<size;i++){
    if (vet[i] > 0){
      if(aux <= 0){
        aux = vet[i];
      }else{
        if(aux > vet[i]){
          aux = vet[i];
        }
      }
    }
  }
  return aux;
}

int calcula_distancia(int inicio, int dest, int distancia, int *qt_perc,int *percorridos, Grafo * grafo){
  int total_percorrido = *qt_perc;
  for(int i = 0; i < total_percorrido; i++){
    if (inicio == percorridos[i] || dest == percorridos[i]){
      grafo->possuiCiclo = 1;
      return -1;
    }
  }

  percorridos[total_percorrido] = inicio;
  total_percorrido= total_percorrido+1;
  *qt_perc = total_percorrido;

  int * percorrer = (int *)malloc(grafo->nrVertices * sizeof(int));
  int * peso = (int *)malloc(grafo->nrVertices * sizeof(int));
  int size=0;
  for (int i = 0; i < grafo->nrVertices; i++) {
    if(grafo->matrizAdjacencia[inicio][i]){
      percorrer[size] = i;
      peso[size] = grafo->matrizAdjacencia[inicio][i];
      size++;
    }
  }

  int * distancias = (int *)malloc(size * sizeof(int));
  for (int indice =0; indice < size; indice++){
    distancias[indice] = -1;
    int vertice = percorrer[indice];
    if (vertice == dest){
      distancias[indice] = peso[indice];
    }else{
      distancias[indice] = calcula_distancia(vertice, dest, peso[indice], qt_perc, percorridos,grafo);
    }
  }
  
  int menor_distancia = retorna_menor(size,distancias);

  for(int i = 0; i < total_percorrido;i++){
    if (inicio == percorridos[i] || dest == percorridos[i]){
      percorridos[i] = 0;
      total_percorrido= total_percorrido-1;
    }
  }
  *qt_perc = total_percorrido;
  free(percorrer);
  free(peso);
  free(distancias);
  if(menor_distancia <= 0){
    return 0;
  }
  return menor_distancia + distancia;
}


void gerar_matriz_distancia(Grafo * grafo){
  printf("Gerando matriz de distancias\n");
  for (int inicio = 0; inicio < grafo->nrVertices; inicio++){
    for (int dest = 0; dest < grafo->nrVertices; dest++){
      if(inicio != dest){
        int * percorridos = (int *)malloc(grafo->nrVertices * sizeof(int));
        int total_percorrido = 0;
        int distancia = calcula_distancia(inicio,dest,0,&total_percorrido,percorridos,grafo);
        grafo->matrizDistancias[inicio][dest] = distancia;
        free(percorridos);
      }
    }
  }
}


void imprimeMatrizAdjacencias(Grafo *grafo) {
  printf("Matriz de adjacencias:\n");
  for (int i = 0; i < grafo->nrVertices; i++) {
    for (int j = 0; j < grafo->nrVertices; j++) {
      printf("%d ", grafo->matrizAdjacencia[i][j]);
    }
    printf("\n");
  }
}

void imprimeMatrizPesos(Grafo *grafo) {
  printf("Matriz de Pesos:\n");
  for (int i = 0; i < grafo->nrVertices; i++) {
    for (int j = 0; j < grafo->nrVertices; j++) {
      printf("%d ", grafo->matrizPesos[i][j]);
    }
    printf("\n");
  }
}

void imprimeMatrizmatrizDistancias(Grafo *grafo) {
  printf("Matriz de distancias:\n");
  for (int i = 0; i < grafo->nrVertices; i++) {
    for (int j = 0; j < grafo->nrVertices; j++) {
      printf("%d ", grafo->matrizDistancias[i][j]);
    }
    printf("\n");
  }
}

void imprimeMatrizIncidencias(Grafo *grafo) {
  printf("Matriz de incidencias:\n");
  for (int i = 0; i < grafo->nrArestas; i++) {
    for (int j = 0; j < grafo->nrVertices; j++) {
      printf("%d ", grafo->matrizIncidencia[i][j]);
    }
    printf("\n");
  }
}

void imprimeMatrizFechoTransitivo(Grafo *grafo) {
  printf("Matriz de Fecho Transistivo:\n");
  for (int i = 0; i < grafo->nrVertices; i++) {
    for (int j = 0; j < grafo->nrVertices; j++) {
      printf("%d ", grafo->matrizFechoTransitivo[i][j]);
    }
    printf("\n");
  }
}

int inPseudoGrafo(Grafo *grafo) {
  for (int i = 0; i < grafo->nrVertices; i++) {
    if (grafo->matrizAdjacencia[i][i]) {
      printf("É um pseudo grafo\n");
      return 1;
    }
  }
  printf("Não é um pseudo grafo\n");
  return 0;
}
int inMultiGrafo(Grafo *grafo) {
  int coluna1 = -1, coluna2 = -1;
  for (int i = 0; i < grafo->nrVertices; i++) {
    for (int j = 0; j < grafo->nrVertices; j++) {
      if (grafo->matrizIncidencia[i][j]) {
        if (coluna1 == -1) {
          coluna1 = j;
        } else {
          coluna2 = j;
        }
      }
    }
    if (coluna1 != -1 && coluna2 != -1) {
      for (int k = 0; k < grafo->nrArestas; k++) {
        if (k != i && grafo->matrizIncidencia[k][coluna1] &&
            grafo->matrizIncidencia[k][coluna2]) {
          printf("É multigrafo \n");
          return 1;
        }
      }
      coluna1 = -1;
      coluna2 = -1;
    }
  }
  printf("Não é multigrafo \n");
  return 0;
}

int inSimples(Grafo *grafo, int inMulti, int inPseudo) {
  if (inMulti || inPseudo) {
    printf("Não é um grafo simples \n");
    return 0;
  } else {
    printf("É um grafo simples \n");
    return 0;
  }
  return 0;
}

int inDesconexo(Grafo *grafo) {
  for (int j = 0; j < grafo->nrVertices; j++) {
    int soma = 0;
    for (int i = 0; i < grafo->nrArestas; i++) {
      soma += grafo->matrizIncidencia[i][j];
    }
    if (soma == 0) {
      printf("É desconexo\n");
      return 1;
    }
  }
  printf("Não é desconexo\n");
  return 0;
}

int inCompleto(Grafo *grafo) {
  for (int i = 0; i < grafo->nrVertices; i++) {
    for (int j = 0; j < grafo->nrVertices; j++) {
      if (i != j && grafo->matrizAdjacencia[i][j] == 0) {
        printf("Não é completo\n");
        return 0;
      }
    }
  }
  printf("É completo\n");
  return 1;
}

int exibeGrausDosVertices(Grafo *grafo) {
  for (int j = 0; j < grafo->nrVertices; j++) {
    int soma = 0;
    for (int i = 0; i < grafo->nrArestas; i++) {
      soma += grafo->matrizIncidencia[i][j];
    }
    printf("Grau do vértice %d: %d\n", j + 1, soma);
  }
}

int main() {
  char filename[100]; // nome do arquivo
  int direcionado=0; 
  FILE *file;

  printf("Digite o nome do arquivo: ");
  scanf("%s", filename);
  printf("É direcionado: ");
  scanf("%d", &direcionado);
  // Abre o arquivo para leitura
  file = fopen("./matriz.txt", "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return 1;
  }

  int nrVertices, nrArestas;
  fscanf(file, "%d %d", &nrVertices, &nrArestas);

  Grafo *grafo = criaMatriz(nrVertices, nrArestas,direcionado);

  for (int i = 0; i < nrArestas; i++) {
    int u, v, peso;
    fscanf(file, "%d %d %d", &u, &v, &peso);
    adicionaAresta(grafo, u, v, peso, i);
  }
  fclose(file);
  executa_warshall(grafo);
  gerar_matriz_distancia(grafo);
  printf("\n");
  imprimeMatrizAdjacencias(grafo);
  printf("\n");
  imprimeMatrizIncidencias(grafo);
  printf("\n");
  printf("\n");
  imprimeMatrizPesos(grafo);
  printf("\n");
  imprimeMatrizmatrizDistancias(grafo);
  printf("\n");
  imprimeMatrizFechoTransitivo(grafo);
  printf("\n");
  int inPseudo = inPseudoGrafo(grafo);
  printf("\n");
  int inMulti = inMultiGrafo(grafo);
  printf("\n");
  inSimples(grafo, inMulti, inPseudo);
  printf("\n");
  inDesconexo(grafo);
  printf("\n");
  inCompleto(grafo);
  printf("\n");
  printf("Possui ciclo: %s\n", grafo->possuiCiclo == 1 ? "Sim": "Não");
  printf("\n");
  printf("Ordem do grafo: %d\n", grafo->nrVertices);
  printf("\n");
  printf("Tamanho do grafo: %d\n", grafo->nrArestas);
  printf("\n");
  exibeGrausDosVertices(grafo);
  printf("\n");
  liberaMatriz(grafo);
  printf("\n");
  return 0;
}