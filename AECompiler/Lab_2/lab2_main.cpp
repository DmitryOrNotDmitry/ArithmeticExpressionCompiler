#include "File.h"
#include "IdToken.h"
#include "LexicalAnalyser.h"
#include "SymbolTable.h"

#include <iostream>
#include <vector>
#include "LexicalException.h"


//int SymbolTable::idPool = 1;
//const SymbolTable* IdToken::currentTable = nullptr;


using namespace std;


int main(int argc, char* argv[])
{
  //setlocale(LC_ALL, "Russian");

  if (argc < 4)
  {
    cout << "Утилита lab2.exe используется для лексического анализа арифметических выражений\n\n";
    cout << "   - lab2.exe  <файл с входящим выражением> <файл вывода токенов> <файл вывода таблицы символов>\n";
    return -1;
  }

  try
  {
    string inExpression(argv[1]);
    File in(inExpression);

    string inputExperssion = in.getLine();
    in.close();

    SymbolTable symbolTable;
    LexicalAnalyser lex(symbolTable);
    
    vector<shared_ptr<Token>> tokens = lex.process(inputExperssion);

    IdToken::currentTable = &symbolTable;

    string outTokens(argv[2]);
    File tokensFile(outTokens);

    for (shared_ptr<Token>& token : tokens)
      tokensFile << token->toStringWithComment() << "\n";

    tokensFile.close();


    string outSymbolsTable(argv[3]);
    File symbolsFile(outSymbolsTable);

    for (const auto& [id, symbol] : symbolTable.allSymbols())
      symbolsFile << std::to_string(id) << " - " << symbol.getName() << "\n";

    symbolsFile.close();
  }
  catch (const LexicalException& e)
  {
    cout << "[Лексическая ошибка] - " << e.what();
    return -1;
  }
  
  return 0;
}
