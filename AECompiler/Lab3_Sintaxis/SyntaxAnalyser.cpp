#include "SyntaxAnalyser.h"
#include "format.h"
#include "SyntaxException.h"

#include <stack>
#include <iostream>
#include <queue>

using namespace std;

void SyntaxAnalyser::checkOperator(Token* pred, Token* token, Token* next)
{
  bool isValid = next && pred
    && (next->getType() == TokenType::OPEN_BRACKET || next->isOperand())
    && (pred->getType() == TokenType::CLOSE_BRACKET || pred->isOperand());

  if (!isValid)
    throw SyntaxException(format("У оператора {} на позиции {} отсутствует операнд", { token->toString(), to_string(token->getPosition()) }));
}

void SyntaxAnalyser::checkOperand(Token* token, Token* next)
{
  bool isValid = (!next) ||
    ( next && (next->getType() == TokenType::CLOSE_BRACKET || next->isOperator()) );

  if (!isValid)
  {
    string tokenName;

    if (token->getType() == TokenType::IDENTIFIER)
      tokenName = "идентификатора";
    else
      tokenName = "константы";

    throw SyntaxException(format("После {} {} на позиции {} отсутствует оператор", { tokenName, token->toString(), to_string(token->getPosition()) }));
  }
}

void SyntaxAnalyser::check(const std::vector<std::shared_ptr<Token>>& tokenStream)
{
  if (tokenStream.size() == 0)
    return;

  stack<Token*> bracketStack;
  for (size_t i = 0; i < tokenStream.size(); i++)
  {
    Token* predToken = nullptr;
    if (i > 0)
      predToken = tokenStream[i - 1].get();

    Token* token = tokenStream[i].get();
    Token* nextToken = nullptr;
    if (i < tokenStream.size() - 1)
      nextToken = tokenStream[i + 1].get();


    TokenType tokenType = token->getType();


    if (tokenType == TokenType::OPEN_BRACKET)
      bracketStack.push(token);

    if (tokenType == TokenType::CLOSE_BRACKET)
    {
      if (bracketStack.empty())
        throw SyntaxException(format( "У закрывающей скобки {} на позиции {} отсутствует открывающая скобка", { token->toString(), to_string(token->getPosition()) }));

      bracketStack.pop();
    }


    if (token->isOperator())
      checkOperator(predToken, token, nextToken);

    if (token->isOperand())
      checkOperand(token, nextToken);

  }

  if (!bracketStack.empty())
  {
    Token* topToken = bracketStack.top();
    throw SyntaxException(format("У открывающей скобки {} на позиции {} отсутствует закрывающая скобка", { topToken->toString(), to_string(topToken->getPosition()) }));
  }
}




std::shared_ptr<Node> SyntaxAnalyser::operatorsLevel1()
{
  auto node = operatorsLevel2();
  while (currentToken->getType() == TokenType::OPERATOR_PLUS || currentToken->getType() == TokenType::OPERATOR_MINUS) {
    auto newNode = std::make_shared<Node>(currentToken);

    nextToken();

    auto right = operatorsLevel2();  
    
    newNode->setLeft(node);  
    newNode->setRight(right);  
    node = newNode;  
  }

  return node;
}

std::shared_ptr<Node> SyntaxAnalyser::operatorsLevel2()
{
  auto node = operands();  

  while (currentToken->getType() == TokenType::OPERATOR_MUL || currentToken->getType() == TokenType::OPERATOR_DIV) {
    auto newNode = std::make_shared<Node>(currentToken);
    
    nextToken();  
    
    auto right = operands();  
    
    newNode->setLeft(node);
    newNode->setRight(right);
    node = newNode;  
  }

  return node;
}

std::shared_ptr<Node> SyntaxAnalyser::operands()
{
  if (currentToken->getType() == TokenType::INT_CONST || currentToken->getType() == TokenType::DOUBLE_CONST) {
    auto node = std::make_shared<Node>(currentToken);
    nextToken();
    return node;
  }
  else if (currentToken->getType() == TokenType::IDENTIFIER) {
    auto node = std::make_shared<Node>(currentToken);
    nextToken();
    return node;
  }
  else if (currentToken->getType() == TokenType::OPEN_BRACKET) {
    nextToken();
    auto node = operatorsLevel1();
    nextToken();
    return node;
  }
}


void SyntaxAnalyser::nextToken()
{
  ++tokenIter;
  if (tokenIter != tokenEnd)
    currentToken = *tokenIter;
}


std::shared_ptr<Node> SyntaxAnalyser::createSyntaxTree(const std::vector<std::shared_ptr<Token>>& tokenStream)
{
  tokenIter = tokenStream.begin();
  tokenEnd = tokenStream.end();
  currentToken = *tokenIter;

  auto tree = operatorsLevel1();

  return tree;
}
