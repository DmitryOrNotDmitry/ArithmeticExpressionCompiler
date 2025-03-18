#pragma once
#include <vector>
#include <list>
#include <map>
#include "ExpressionItem.h"

using namespace std;

class ExpressionItemPool
{

  list<ExpressionItem> operators;
  list<ExpressionItem> operations;
  map<string, ExpressionItem*> allItems;

  const ExpressionItem* randomItem(const list<ExpressionItem>& items) const;

public:

  ExpressionItemPool(list<ExpressionItem> operators, list<ExpressionItem> operations)
    : operations(operations), operators(operators)
  {
    for (auto iter = this->operations.begin(); iter != this->operations.end(); ++iter)
      allItems[iter->View()] = &(*iter);

    for (auto iter = this->operators.begin(); iter != this->operators.end(); ++iter)
      allItems[iter->View()] = &(*iter);
  }

  const ExpressionItem* byView(string view) const;

  const ExpressionItem* getRandomOperator() const;
  const ExpressionItem* getRandomOperation() const;

};

