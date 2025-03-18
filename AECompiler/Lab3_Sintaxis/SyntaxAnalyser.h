#pragma once
#include "SymbolTable.h"
#include "Node.h"

#include <vector>
#include <memory>
#include <Token.h>

class SyntaxAnalyser
{

  void checkOperator(Token* pred, Token* token, Token* next);
  void checkOperand(Token* token, Token* next);



  std::shared_ptr<Token> currentToken;
  std::vector<std::shared_ptr<Token>>::const_iterator tokenIter;
  std::vector<std::shared_ptr<Token>>::const_iterator tokenEnd;

  void nextToken();

  std::shared_ptr<Node> operatorsLevel1();

  std::shared_ptr<Node> operatorsLevel2();

  std::shared_ptr<Node> operands();


public:


  void check(const std::vector<std::shared_ptr<Token>>& tokenStream);

  std::shared_ptr<Node> createSyntaxTree(const std::vector<std::shared_ptr<Token>>& tokenStream);

};

