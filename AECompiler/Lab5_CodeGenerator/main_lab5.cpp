#include "File.h"
#include "IdToken.h"
#include "LexicalAnalyser.h"
#include "SymbolTable.h"
#include "SyntaxAnalyser.h"
#include "format.h"

#include "LexicalException.h"
#include "SyntaxException.h"
#include "SemanticException.h"
#include "SemanticAnalyser.h"
#include "ThreeAddressCodeGenerator.h"
#include "PostfixExprGenerator.h"

#include <iostream>
#include <vector>


int SymbolTable::idPool = 1;
const SymbolTable* IdToken::currentTable = nullptr;


using namespace std;



void lexMode(const string& inFile, const string& outTokenFile, const string& outSymbolsFile);

void synMode(const string& inFile, const string& outSynTreeFile);

void semMode(const string& inFile, const string& outModifySynTreeFile);

void gen1Mode(const string& inFile, const string& outPortableCode, const string& outSymbolTableFile);

void gen2Mode(const string& inFile, const string& outPostfixExpression, const string& outSymbolTableFile);


int main(int argc, char* argv[])
{
  //setlocale(LC_ALL, "Russian");
  //argc = 10;
  //vector<string> argv = {
  //  "-",
  //  "gen1",
  //  "in.txt",
  //  "tree.txt",
  //  "symbols.txt",
  //};

  const string LEX_MODE = "lex";
  const string SYN_MODE = "syn";
  const string SEM_MODE = "sem";
  const string GEN1_MODE = "gen1";
  const string GEN2_MODE = "gen2";

  const string ERROR_LEX_MODE = "   - " + LEX_MODE + " <файл с входящим выражением> <файл вывода токенов> <файл вывода таблицы символов>\n";
  const string ERROR_SYN_MODE = "   - " + SYN_MODE + " <файл с входящим выражением> <файл вывода синтаксического дерева>\n";
  const string ERROR_SEM_MODE = "   - " + SEM_MODE + " <файл с входящим выражением> <файл вывода модифицированного синтаксического дерева>\n";
  const string ERROR_GEN1_MODE = "   - " + GEN1_MODE + " <файл с входящим выражением> <файл с трехадресным кодом> <файл таблицы символов>\n";
  const string ERROR_GEN2_MODE = "   - " + GEN2_MODE + " <файл с входящим выражением> <файл с постфиксным выражением> <файл таблицы символов>\n";

  if (argc < 2)
  {
    cout << "Утилита lab5.exe используется для лексического и синтаксического анализа арифметических выражений\n\n";
    cout << ERROR_LEX_MODE << "\n";
    cout << ERROR_SYN_MODE << "\n";
    cout << ERROR_SEM_MODE << "\n";
    cout << ERROR_GEN1_MODE << "\n";
    cout << ERROR_GEN2_MODE;
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

    else if (workMode == GEN1_MODE)
    {
      if (argc < 5)
      {
        cout << ERROR_GEN1_MODE;
        return -1;
      }

      string inExpression(argv[2]);
      string outPortableCode(argv[3]);
      string outSymbolTable(argv[4]);

      gen1Mode(inExpression, outPortableCode, outSymbolTable);
    }

    else if (workMode == GEN2_MODE)
    {
      if (argc < 5)
      {
        cout << ERROR_GEN2_MODE;
        return -1;
      }

      string inExpression(argv[2]);
      string outPostfixExpr(argv[3]);
      string outSymbolTable(argv[4]);

      gen2Mode(inExpression, outPostfixExpr, outSymbolTable);
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


void outSymbolTable(SymbolTable& table, const string& outFileName)
{
  File symbolsFile(outFileName);

  for (const auto& [id, symbol] : table.allSymbols())
    symbolsFile << std::to_string(id) << " - " << symbol.getName() << TokenComments::getIdentifierCommentToSymbol(symbol.getType()) << "\n";

  symbolsFile.close();
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

  outSymbolTable(symbolTable, outSymbolsFile);
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


void gen1Mode(const string& inFile, const string& outPortableCode, const string& outSymbolTableFile)
{
  SymbolTable symbolTable;
  vector<shared_ptr<Token>> tokens;

  lexProcess(inFile, symbolTable, tokens);


  SyntaxAnalyser synAnalyser;

  synAnalyser.check(tokens);

  std::shared_ptr<Node> tree = synAnalyser.createSyntaxTree(tokens);


  SemanticAnalyser semAnalyser(symbolTable);

  std::shared_ptr<Node> modifyTree = semAnalyser.modifySyntaxTree(tree);


  ThreeAddressCodeGenerator codeGenerator(symbolTable);

  const vector<ThreeAddressInstruction> instructions = codeGenerator.generateCode(modifyTree);

  File outIntructions(outPortableCode);
  for (const auto& instruc : instructions)
  {
    outIntructions << instruc.toString() + "\n";
  }

  outSymbolTable(symbolTable, outSymbolTableFile);
}

void gen2Mode(const string& inFile, const string& outPostfixExpression, const string& outSymbolTableFile)
{
  SymbolTable symbolTable;
  vector<shared_ptr<Token>> tokens;

  lexProcess(inFile, symbolTable, tokens);


  SyntaxAnalyser synAnalyser;

  synAnalyser.check(tokens);

  std::shared_ptr<Node> tree = synAnalyser.createSyntaxTree(tokens);


  SemanticAnalyser semAnalyser(symbolTable);

  std::shared_ptr<Node> modifyTree = semAnalyser.modifySyntaxTree(tree);


  PostfixExprGenerator postfixGen;

  vector<shared_ptr<Token>> postfixExpr = postfixGen.generatePostfixExpr(modifyTree);

  File outPostfixExprFile(outPostfixExpression);
  for (const auto& token : postfixExpr)
  {
    outPostfixExprFile << token->toString() + " ";
  }

  outSymbolTable(symbolTable, outSymbolTableFile);
}
