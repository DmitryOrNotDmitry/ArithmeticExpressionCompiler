#include "ExpressionOptimizator.h"
#include <functional>

double getTokenConstValue(shared_ptr<Token> token)
{
  if (token->getType() == TokenType::INT_CONST)
    return token->getValue<int>();

  if (token->getType() == TokenType::DOUBLE_CONST)
    return token->getValue<double>();
}

shared_ptr<Token> calcAndCreateNewToken(
  shared_ptr<Token> operation,
  shared_ptr<Token> leftOperand,
  shared_ptr<Token> rightOperand)
{
  double leftValue = getTokenConstValue(leftOperand);
  double rightValue = getTokenConstValue(rightOperand);

  double result = 0;

  if (operation->getType() == TokenType::OPERATOR_PLUS)
    result = leftValue + rightValue;

  if (operation->getType() == TokenType::OPERATOR_MINUS)
    result = leftValue - rightValue;

  if (operation->getType() == TokenType::OPERATOR_MUL)
    result = leftValue * rightValue;

  if (operation->getType() == TokenType::OPERATOR_DIV)
    result = leftValue / rightValue;

  if (leftOperand->getType() == TokenType::INT_CONST && rightOperand->getType() == TokenType::INT_CONST)
  {
    return make_shared<Token>(TokenType::INT_CONST, (int)result, -1);
  }
  else
  {
    return make_shared<Token>(TokenType::DOUBLE_CONST, result, -1);
  }
}

void ExpressionOptimizator::calcConstOperations(shared_ptr<Node>& node)
{
  shared_ptr<Token> nodeToken = node->token;
  if (nodeToken->isBinaryOperator())
  {
    shared_ptr<Token> leftToken = node->left->token;
    shared_ptr<Token> rightToken = node->right->token;

    if (leftToken->isConstOperand() && rightToken->isConstOperand())
    {
      shared_ptr<Token> newToken = calcAndCreateNewToken(nodeToken, leftToken, rightToken);
      
      node->token = newToken;

      node->left = nullptr;
      node->right = nullptr;
    }
  }
}

void ExpressionOptimizator::calcInt2FloatWithConst(shared_ptr<Node>& node)
{
  shared_ptr<Token> nodeToken = node->token;
  if (nodeToken->getType() == TokenType::INT_TO_FLOAT)
  {
    shared_ptr<Token> leftToken = node->left->token;

    if (leftToken->getType() == TokenType::INT_CONST)
    {
      shared_ptr<Token> newToken = make_shared<Token>(TokenType::DOUBLE_CONST, (double) leftToken->getValue<int>(), -1);

      node->token = newToken;

      node->left = nullptr;
    }
  }
}


enum class EvidentType {
  NONE,
  EXPR_TO_EXPR,
  EXPR_TO_ZERO,
};

bool isTokenHasValue(shared_ptr<Token> operand, double value)
{
  if (operand->isConstOperand())
  {
    double leftValue = getTokenConstValue(operand);

    return abs(leftValue - value) < 1e-10;
  }

  return false;
}

bool isZeroConst(shared_ptr<Token> operand)
{
  return isTokenHasValue(operand, 0);
}

bool isOneConst(shared_ptr<Token> operand)
{
  return isTokenHasValue(operand, 1);
}

vector<function<bool(shared_ptr<Node>&, shared_ptr<Node>&, shared_ptr<Node>&)>> evidentChekers = {

  [](shared_ptr<Node>& operation,
    shared_ptr<Node>& leftOperand,
    shared_ptr<Node>& rightOperand)
  {
    bool isReplace = false;
    shared_ptr<Node> newNodeForReplacing;

    if (isZeroConst(leftOperand->token))
    {
      isReplace = true;
      
      TokenType operationType = operation->token->getType();

      if (operationType == TokenType::OPERATOR_MINUS || operationType == TokenType::OPERATOR_PLUS)
        newNodeForReplacing = rightOperand;

      if (operationType == TokenType::OPERATOR_MUL || operationType == TokenType::OPERATOR_DIV)
        newNodeForReplacing = leftOperand;
    }

    if (isZeroConst(rightOperand->token))
    {
      isReplace = true;

      TokenType operationType = operation->token->getType();

      if (operationType == TokenType::OPERATOR_MINUS || operationType == TokenType::OPERATOR_PLUS)
        newNodeForReplacing = leftOperand;

      if (operationType == TokenType::OPERATOR_MUL || operationType == TokenType::OPERATOR_DIV)
        newNodeForReplacing = rightOperand;
    }

    if (isReplace)
    {
      operation->left = nullptr;
      operation->right = nullptr;
      
      operation = newNodeForReplacing;
    }

    return isReplace;
  },

  [](shared_ptr<Node>& operation,
    shared_ptr<Node>& leftOperand,
    shared_ptr<Node>& rightOperand)
  {
    bool isReplace = false;
    shared_ptr<Node> newNodeForReplacing;

    TokenType operationType = operation->token->getType();

    if (operationType == TokenType::OPERATOR_MUL)
    {
      if (isOneConst(leftOperand->token))
      {
        isReplace = true;
        newNodeForReplacing = rightOperand;
      }
    }

    if (operationType == TokenType::OPERATOR_MUL || operationType == TokenType::OPERATOR_DIV)
    {
      if (isOneConst(rightOperand->token))
      {
        isReplace = true;
        newNodeForReplacing = leftOperand;
      }
    }

    if (isReplace)
    {
      operation->left = nullptr;
      operation->right = nullptr;

      operation = newNodeForReplacing;
    }
    
    return isReplace;
  },

};


void ExpressionOptimizator::calcEvidentOperations(shared_ptr<Node>& node)
{
  shared_ptr<Token> nodeToken = node->token;
  if (nodeToken->isBinaryOperator())
  {
    for (auto iterChecker = evidentChekers.begin(); iterChecker != evidentChekers.end(); ++iterChecker)
    {
      auto& evidentChecker = *iterChecker;

      bool isReplaced = evidentChecker(node, node->left, node->right);

      if (isReplaced)
        break;
    }
  }
}

void ExpressionOptimizator::recursive_run(shared_ptr<Node>& node)
{
  if (node->left)
    recursive_run(node->left);

  if (node->right)
    recursive_run(node->right);

  calcInt2FloatWithConst(node);

  calcConstOperations(node);

  calcEvidentOperations(node);
}


void ExpressionOptimizator::run(shared_ptr<Node>& modifySyntaxTree)
{
  recursive_run(modifySyntaxTree);
}
