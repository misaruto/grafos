#ifndef FILEMANIPULATOR_H
#define FILEMANIPULATOR_H

#include <iostream>
#include <fstream>
#include <functional>

using namespace std;

class FileManipulator
{
private:
  int nr_lines;
  int actual_line;
  fstream file;
  bool in_write_mode;
  string file_name;
  string* lines;
  void read_file_lines();
  void inicia_variaveis(string mode,string file_name);
  void ask_file_name();
  
public:
  FileManipulator();
  FileManipulator(string file_name);
  FileManipulator(string file_name,string mode);
  ~FileManipulator();
  void write(string text);
  string* get_lines();
  string get_next_line();
  string get_nm_file();
  void for_each(function<void(string, int, string *)> callback);
};
#endif