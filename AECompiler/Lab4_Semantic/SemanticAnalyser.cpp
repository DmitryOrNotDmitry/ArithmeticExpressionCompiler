#include "SemanticAnalyser.h"
#include "SemanticException.h"
#include "format.h"
#include "Int2FloatToken.h"

void SemanticAnalyser::zeroCheck(std::shared_ptr<Node> node)
{
  if (node->token->getType() == TokenType::OPERATOR_DIV)
  {
    const Token& token = *node->right->token;
    TokenType rightTokenType = token.getType();

    if (rightTokenType == TokenType::INT_CONST)
    {
      if (token.getValue<int>() == 0)
        throw SemanticException(format("Деление на ноль на позиции {}", { std::to_string(token.getPosition()) }));
    }
      
    if (rightTokenType == TokenType::DOUBLE_CONST)
    {
      if ((token.getValue<double>() - 0) < 1e-10)
        throw SemanticException(format("Деление на ноль на позиции {}", { std::to_string(token.getPosition()) }));
    }
  }

  if (node->left)
    zeroCheck(node->left);

  if (node->right)
    zeroCheck(node->right);
}


bool SemanticAnalyser::containsFloatNode(std::shared_ptr<Node> node)
{
  if (!node) return false;

  if (node->token->getType() == TokenType::DOUBLE_CONST)
    return true;

  if (node->token->getType() == TokenType::IDENTIFIER)
  {
    IdentifierType idType = symbolTable.getSymbol(node->token->getValue<int>()).getType();
    if (idType == IdentifierType::FLOAT)
      return true;
  }

  return containsFloatNode(node->left) || containsFloatNode(node->right);
}


IdentifierType SemanticAnalyser::getNodeType(std::shared_ptr<Node> node)
{
  if (containsFloatNode(node))
    return IdentifierType::FLOAT;

  return IdentifierType::INTEGER;
}


void SemanticAnalyser::addNodeConvertInt2Float(std::shared_ptr<Node> node, IdentifierType rightNodeType)
{
  if (!node) return;

  std::shared_ptr<Token> int2Float = std::make_shared<Int2FloatToken>();
  auto newNode = std::make_shared<Node>(int2Float);
  
  if (rightNodeType == IdentifierType::INTEGER)
  {
    newNode->left = node->right;
    node->right = newNode;
  }
  else
  {
    newNode->left = node->left;
    node->left = newNode;
  }
}


void SemanticAnalyser::checkFloats(std::shared_ptr<Node> node)
{
  if (!node) return;

  IdentifierType rightType = getNodeType(node->right);
  IdentifierType leftType = getNodeType(node->left);
  
  checkFloats(node->left);
  checkFloats(node->right);

  if (rightType != leftType)
  {
    addNodeConvertInt2Float(node, rightType);
  }
}


std::shared_ptr<Node> SemanticAnalyser::modifySyntaxTree(std::shared_ptr<Node> syntaxTree)
{
  zeroCheck(syntaxTree);

  checkFloats(syntaxTree);

  return syntaxTree;
}
