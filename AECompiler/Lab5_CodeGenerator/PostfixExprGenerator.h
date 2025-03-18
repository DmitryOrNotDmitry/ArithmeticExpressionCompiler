#pragma once
#include <vector>
#include <memory>

#include "Token.h"
#include "Node.h"

using namespace std;

class PostfixExprGenerator
{

  vector<shared_ptr<Token>> expression;

  void extendExpression(shared_ptr<Node> node)
  {
    if (node->left)
      extendExpression(node->left);

    if (node->right)
      extendExpression(node->right);

    expression.push_back(node->token);
  }

public:

  vector<shared_ptr<Token>> generatePostfixExpr(shared_ptr<Node> syntaxTree)
  {
    expression.clear();

    extendExpression(syntaxTree);

    return expression;
  }

};

