#pragma once
#include <string>
#include <fstream>

class File
{
  template<class T>
  friend File& operator<<(File& file, const T& data) {
    if (!file.out.is_open())
      file.openWrite();

    file.out << data;

    return file;
  };

  std::string name;

  std::ofstream out;
  std::ifstream in;

  bool isBinary;

  void openRead()
  {
    if (isBinary)
      in.open(name, std::ios::binary);
    else
      in.open(name);


    if (in.fail()) {
      throw std::runtime_error("Файла \"" + name + "\" не существует\n");
    }
  }

  void openWrite()
  {
    if (isBinary)
      out.open(name, std::ios::binary);
    else
      out.open(name);

    if (out.fail()) {
      throw std::runtime_error("Файла \"" + name + "\" не существует\n");
    }
  } 

public:

  File(const std::string& name, bool isBinary = false)
    : name(name)
    , isBinary(isBinary)
  {
  }

  File(const File& other)
    : name(other.name)
    , isBinary(other.isBinary)
  {}


  ~File()
  {
    close();
  }

  std::string getLine()
  {
    if (!in.is_open())
      openRead();

    std::string input = "";
   
    std::getline(in, input);

    return input;
  }

  void close()
  {
    if (out.is_open())
      out.close();

    if (in.is_open())
      in.close();
  }

  template<class T>
  void writeBinary(const T& data)
  {
    if (!out.is_open())
      openWrite();

    out.write(reinterpret_cast<const char*>(&data), sizeof(data));
  }

  template<class T>
  void readBinary(T& data) {
    if (!in.is_open()) {
      openRead();
    }

    in.read(reinterpret_cast<char*>(&data), sizeof(data));
    if (!in) {
      throw std::ios_base::failure("Ошибка чтения данных из файла.");
    }
  }

  void readBinary(std::string& data) {
    if (!in.is_open()) {
      openRead();
    }

    data.clear();
    char ch;
    while (in.get(ch) && ch != '\0') {
      data += ch;
    }

    if (in.fail() && ch != '\0') {
      throw std::ios_base::failure("Ошибка чтения строки из файла.");
    }
  }

};



