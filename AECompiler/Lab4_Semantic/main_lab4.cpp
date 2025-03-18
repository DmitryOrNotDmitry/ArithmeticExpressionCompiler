#include "File.h"
#include "IdToken.h"
#include "LexicalAnalyser.h"
#include "SymbolTable.h"
#include "SyntaxAnalyser.h"
#include "format.h"

#include <iostream>
#include <vector>
#include <LexicalException.h>
#include "SyntaxException.h"
#include "SemanticException.h"
#include "SemanticAnalyser.h"


int SymbolTable::idPool = 1;
const SymbolTable* IdToken::currentTable = nullptr;


using namespace std;



void lexMode(const string& inFile, const string& outTokenFile, const string& outSymbolsFile);

void synMode(const string& inFile, const string& outSynTreeFile);

void semMode(const string& inFile, const string& outModifySynTreeFile);


int main(int argc, char* argv[])
{
  //setlocale(LC_ALL, "Russian");
  //argc = 10;
  //vector<string> argv = {
  //  "-",
  //  "sem",
  //  "in.txt",
  //  "tree.txt",
  //  "symbols.txt",
  //};

  const string LEX_MODE = "lex";
  const string SYN_MODE = "syn";
  const string SEM_MODE = "sem";

  const string ERROR_LEX_MODE = "   - " + LEX_MODE + " <файл с входящим выражением> <файл вывода токенов> <файл вывода таблицы символов>\n";
  const string ERROR_SYN_MODE = "   - " + SYN_MODE + " <файл с входящим выражением> <файл вывода синтаксического дерева>\n";
  const string ERROR_SEM_MODE = "   - " + SEM_MODE + " <файл с входящим выражением> <файл вывода модифицированного синтаксического дерева>\n";

  if (argc < 2)
  {
    cout << "Утилита lab4.exe используется для лексического и синтаксического анализа арифметических выражений\n\n";
    cout << ERROR_LEX_MODE << "\n";
    cout << ERROR_SYN_MODE << "\n";
    cout << ERROR_SEM_MODE;
    return -1;
  }

  string workMode(argv[1]);
  workMode = str_tolower(workMode);

  try
  {
    if (workMode == LEX_MODE)
    {
      if (argc < 5)
      {
        cout << ERROR_LEX_MODE;
        return -1;
      }

      string inExpression(argv[2]);
      string outTokens(argv[3]);
      string outSymbolsTable(argv[4]);

      lexMode(inExpression, outTokens, outSymbolsTable);
    }

    else if (workMode == SYN_MODE)
    {
      if (argc < 4)
      {
        cout << ERROR_SYN_MODE;
        return -1;
      }

      string inExpression(argv[2]);
      string outSynTree(argv[3]);

      synMode(inExpression, outSynTree);
    }

    else if (workMode == SEM_MODE)
    {
      if (argc < 4)
      {
        cout << ERROR_SYN_MODE;
        return -1;
      }

      string inExpression(argv[2]);
      string outModyfiedSynTree(argv[3]);

      semMode(inExpression, outModyfiedSynTree);
    }

    else
    {
      cout << format("Режима работы '{}' не существует", { workMode });
    }

  }
  catch (const SemanticException& e)
  {
    cout << "[Семантическая ошибка] - " << e.what();
    return -1;
  }
  catch (const SyntaxException& e)
  {
    cout << "[Синтаксическая ошибка] - " << e.what();
    return -1;
  }
  catch (const LexicalException& e)
  {
    cout << "[Лексическая ошибка] - " << e.what();
    return -1;
  }

  return 0;
}


void lexProcess(const string& inFile, SymbolTable& symbolTable, vector<shared_ptr<Token>>& tokens)
{
  File in(inFile);

  string inputExperssion = in.getLine();
  in.close();

  LexicalAnalyser lex(symbolTable);

  tokens = lex.process(inputExperssion);

  IdToken::currentTable = &symbolTable;
}


void lexMode(const string& inFile, const string& outTokenFile, const string& outSymbolsFile)
{
  SymbolTable symbolTable;
  vector<shared_ptr<Token>> tokens;

  lexProcess(inFile, symbolTable, tokens);

  File tokensFile(outTokenFile);

  for (shared_ptr<Token>& token : tokens)
    tokensFile << token->toStringWithComment() << "\n";

  tokensFile.close();


  File symbolsFile(outSymbolsFile);

  for (const auto& [id, symbol] : symbolTable.allSymbols())
    symbolsFile << std::to_string(id) << " - " << symbol.getName() << TokenComments::getIdentifierCommentToSymbol(symbol.getType()) << "\n";

  symbolsFile.close();
}


void synMode(const string& inFile, const string& outSynTreeFile)
{
  SymbolTable symbolTable;
  vector<shared_ptr<Token>> tokens;

  lexProcess(inFile, symbolTable, tokens);


  SyntaxAnalyser synAnalyser;

  synAnalyser.check(tokens);

  std::shared_ptr<Node> tree = synAnalyser.createSyntaxTree(tokens);

  File syntaxTreeFile(outSynTreeFile);
  tree->print(syntaxTreeFile);
  syntaxTreeFile.close();
}


void semMode(const string& inFile, const string& outModifySynTreeFile)
{
  SymbolTable symbolTable;
  vector<shared_ptr<Token>> tokens;

  lexProcess(inFile, symbolTable, tokens);


  SyntaxAnalyser synAnalyser;

  synAnalyser.check(tokens);

  std::shared_ptr<Node> tree = synAnalyser.createSyntaxTree(tokens);


  SemanticAnalyser semAnalyser(symbolTable);

  std::shared_ptr<Node> modifyTree = semAnalyser.modifySyntaxTree(tree);

  File syntaxTreeFile(outModifySynTreeFile);
  modifyTree->print(syntaxTreeFile);
  syntaxTreeFile.close();
}
