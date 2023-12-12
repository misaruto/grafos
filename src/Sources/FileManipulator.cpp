#include "../Headers/FileManipulator.h"


void FileManipulator::inicia_variaveis(string mode,string file_name){
  this->nr_lines = 0;
  this->file_name = file_name;
  this->lines = NULL;
  this->actual_line = 0;
  if (this->file_name == ""){
    this->ask_file_name();
  }

  if(mode == "w"){
    this->file = fstream(file_name.c_str(),ios::out);
    this->in_write_mode = true;
  }else{
    this->file = fstream(file_name.c_str(),ios::in);
    if(!this->file.is_open()){
      this->file.open(this->file_name);
    }
    this->read_file_lines();
  }
}

FileManipulator::FileManipulator(){
  this->inicia_variaveis("","");

}

FileManipulator::FileManipulator(string file_name){
  this->inicia_variaveis("r",file_name);
}

FileManipulator::FileManipulator(string file_name, string mode){
  this->inicia_variaveis(mode,file_name);
}

void FileManipulator::write(string text){
  if (!this->in_write_mode){
    throw invalid_argument("Arquivo não aberto em modo de escrita");
    return;
  }
  if(this->file_name == ""){
    throw invalid_argument("Nome do arquivo não informado");
    return;
  }else if(this->file_name != "" and !this->file.is_open()){
    this->file = fstream(file_name.c_str(),ios::out);
    this->file.open(this->file_name);
  }
  this->file << text;
  this->file.close();
}

FileManipulator::~FileManipulator(){
  delete [] this->lines;
}

void FileManipulator::ask_file_name(){
  cout << "Digite o nome do arquivo: ";
  cin >> this->file_name;
  cout << endl;
  cout << "Nome do arquivo: " << this->file_name << endl;
}

void FileManipulator::read_file_lines(){
  cout << "Lendo arquivo: " << this->file_name << endl;
  if (!this->file.is_open()){
    throw invalid_argument("Arquivo não aberto");
    return;
  }
  cout << "Arquivo aberto" << endl;
  string input[999];
  while(!this->file.eof()){
    getline(this->file, input[this->nr_lines]);
    this->nr_lines++;
    cout << "Linha " << this->nr_lines << ": " << input[this->nr_lines-1] << endl;
  }
  this->lines = new string[this->nr_lines];
  for (int i = 0; i < this->nr_lines; i++){
    this->lines[i] = input[i];

  }
  this->file.close();
}

string* FileManipulator::get_lines(){
  if (this->nr_lines == 0){
    this->read_file_lines();
  }
  return this->lines;
}
string FileManipulator::get_nm_file(){
  return this->file_name;
}

string FileManipulator::get_next_line(){
  if (this->actual_line >= this->nr_lines){
    return "";
  }
  return this->lines[this->actual_line++];
}

void FileManipulator::for_each(function<void(string, int, string *)> callback){
  if(this->nr_lines == 0){
    throw invalid_argument("Arquivo não carregado");
    return;
  }
  for(int i = this->actual_line; i < this->nr_lines; i++){
    callback(this->lines[i], i, this->lines);
  }
};