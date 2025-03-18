#include "File.h"
#include "format.h"


#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <charconv>
#include <SymbolTable.h>
#include <IdToken.h>

#include "BinaryAECodeReader.h"
#include "InstructionCalculator.h"


using namespace std;

int SymbolTable::idPool = 1;
const SymbolTable* IdToken::currentTable = nullptr;


bool isTmpIdentifier(const std::string& idName)
{
  if (idName.size() == 0)
    return false;

  return idName[0] == '@';
}


template<class T>
bool tryParse(const std::string& str, T& value)
{
  auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);

  return ec == std::errc{};
}


template<>
bool tryParse<double>(const std::string& str, double& value)
{
  auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);

  std::regex doublePattren("[0-9]([.][0-9]*)?");

  return ec == std::errc{} && std::regex_match(str, doublePattren);
}


template<>
bool tryParse<int>(const std::string& str, int& value)
{
  auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);

  std::regex intPattern("[0-9]*");

  return ec == std::errc{} && std::regex_match(str, intPattern);
}



void fillSymbolTableByUserInput(SymbolTable& symbolTable)
{
  cout << "Введите значения для переменных:\n";

  for (auto& [id, symbol] : symbolTable.allSymbols())
  {
    if (isTmpIdentifier(symbol.getName()))
      continue;

    cout << '\n';
    if (symbol.getType() == IdentifierType::INTEGER)
    {
      cout << symbol.getName() << " [целое] = ";
      int value = 1;

      std::string strValue;
      std::getline(cin, strValue);

      while (!tryParse(strValue, value))
      {
        cout << "Введите целое число!\n";
        std::getline(cin, strValue);
        strValue.erase(strValue.rbegin().base());
      }

      symbol.setValue(value);
      continue;
    }

    if (symbol.getType() == IdentifierType::FLOAT)
    {
      cout << symbol.getName() << " [вещественное] = ";
      double value = 1.1;
      
      std::string strValue;
      std::getline(cin, strValue);

      while (!tryParse(strValue, value))
      {
        cout << "Введите вещественное число!\n";
        std::getline(cin, strValue);
        strValue.erase(strValue.rbegin().base());
      }
      
      symbol.setValue(value);
      continue;
    }
  }
}


int main(int argc, char* argv[])
{
  //setlocale(LC_ALL, "Russian");
  //argc = 10;
  //vector<string> argv = {
  //  "-",
  //  "port_code.bin",
  //};

  if (argc < 2)
  {
    cout << "Утилита AEInterpretator.exe используется для интерпретации промежуточного кода арифметических выражений\n\n";
    cout << "\t- AEInterpretator.exe <бинарный файл промежуточного кода>";
    return -1;
  }

  string inBinaryCode(argv[1]);
  File inBinaryCodeFile(inBinaryCode, true);


  SymbolTable symbolTable;
  BinaryAECodeReader fileReader(inBinaryCodeFile, symbolTable);

  fileReader.readSymbolTable();

  fillSymbolTableByUserInput(symbolTable);

  InstructionCalculator calculator(symbolTable);
  
  InstructionData instruction;

  while (true)
  {
    if (fileReader.IsDone())
    {
      const Symbol& endResult = symbolTable.getSymbol(instruction.resultId);

      if (endResult.getType() == IdentifierType::INTEGER)
        cout << "Результат выражения = " << endResult.getValue<int>();
      else
        cout << "Результат выражения = " << endResult.getValue<double>();

      break;
    }
    
    instruction = fileReader.readNext();

    calculator.execute(instruction);
  }

  return 0;
}
