#include <iostream>
#include <fstream>
#include <algorithm>

#include "ExpressionItemPool.h"
#include "ExpressionItem.h"
#include "ExpressionGenerator.h"
#include "ExpressionTranslator.h"

using namespace std;


string str_tolower(std::string s)
{
  transform(s.begin(), s.end(), s.begin(),
    [](unsigned char c) { return std::tolower(c); }
  );
  return s;
}


int main(int argc, char* argv[])
{

  if (argc < 2)
  {
    cout << "Утилита lab1.exe используется для создания и трансляции арифметических выыражений\n\n";
    cout << "   - lab1.exe G <файл вывода> <кол-во выражений> <минимальное кол-во операторов> <максимальное кол-во операторов>\n";
    cout << "   заполняет файл с помощью случайно сгенерированных арифметических выражений\n\n";
    cout << "   - lab1.exe T <файл источника> <файл вывода>\n";
    cout << "   транслирует формальные выражения из источника в словесное представления и заполняет файл вывода\n\n";
    return -1;
  }

  string programMode(argv[1]);
  
  ExpressionItemPool pool(
    {
      ExpressionItem("1", "один"),
      ExpressionItem("2", "два"),
      ExpressionItem("3", "три"),
      ExpressionItem("4", "четыре"),
      ExpressionItem("5", "пять"),
      ExpressionItem("6", "шесть"),
      ExpressionItem("7", "семь"),
      ExpressionItem("8", "восемь"),
      ExpressionItem("9", "девять"),

    },

    {
      ExpressionItem("+", "плюс"),
      ExpressionItem("-", "минус"),
      ExpressionItem("*", "умножить на"),
      ExpressionItem(":", "делить на"),

    }
  );


  if (str_tolower(programMode) == "g")
  {
    ExpressionGenerator gen(pool);

    if (argc < 6)
    {
      cout << "Недостаточно аргументов, используйте:\n";
      cout << "lab1.exe G <файл вывода> <кол-во выражений> <минимальное кол-во операторов> <максимальное кол-во операторов>\n";
      return -1;
    }

    string outputFileName(argv[2]);
    
    int countExprs = atoi(argv[3]);
    int minOperators = atoi(argv[4]);
    int maxOperators = atoi(argv[5]);

    if (minOperators > maxOperators)
    {
      cout << "Минимальное кол-во операторов должно быть не больше чем максимальное кол-во операторов\n";
      return -1;
    }

    std::ofstream out;
    out.open(outputFileName);
    
    if (out.fail()) {
      cout << "Ошибка в указании имени файла \"" << outputFileName << "\"\n";
      return -1;
    }
    
    if (out.is_open())
    {
      for (int i = 0; i < countExprs; i++)
        out << gen.generate(minOperators, maxOperators).View() << endl;
    }

    out.close();
  }
  else if (str_tolower(programMode) == "t")
  {
    if (argc < 4)
    {
      cout << "Недостаточно аргументов, используйте\n";
      cout << "lab1.exe T <файл источника> <файл вывода>\n";
      return -1;
    }
  
    string sourceFileName(argv[2]);
    string outputFileName(argv[3]);

    ExpressionTranslator tran(pool);

    std::ofstream out;
    out.open(outputFileName);
    if (out.fail()) {
      cout << "Ошибка в указании имени файла \"" << outputFileName << "\"\n";
      return -1;
    }

    if (!out.is_open())
    {
      cout << "Не удалось открыть файл вывода " << outputFileName;
      return -1;
    }

    std::ifstream source;
    source.open(sourceFileName);
    if (out.fail()) {
      cout << "Ошибка в указании имени файла \"" << outputFileName << "\"\n";
      return -1;
    }
    
    if (!source.is_open())
    {
      cout << "Не удалось открыть файл источника " << sourceFileName;
      return -1;
    }

    string line;
    string transLine;

    try
    {
      while (std::getline(source, line)) 
      {
        transLine = tran.translate(line);

        out << transLine << endl;
      }
    }
    catch (runtime_error& e)
    {
      out.close();
      source.close();
      cout << e.what();
      return -1;
    }

    out.close();
    source.close();
  }
  else {
    cout << "Неизветсный режим работы \"" << programMode << "\"\n";
  }
  
}
 