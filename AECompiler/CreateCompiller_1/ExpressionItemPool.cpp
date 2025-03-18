#include "ExpressionItemPool.h"


const ExpressionItem* ExpressionItemPool::randomItem(const list<ExpressionItem>& items) const
{
  if (items.size() == 0)
    return nullptr;

  int randIdx = rand() % items.size();

  return &(*next(items.begin(), randIdx));
}


const ExpressionItem* ExpressionItemPool::byView(string view) const
{
  const ExpressionItem* item = nullptr;

  auto itemIter = allItems.find(view);
  if (itemIter != allItems.end())
    item = itemIter->second;

  return item;
}

const ExpressionItem* ExpressionItemPool::getRandomOperator() const
{
  return randomItem(operators);
}

const ExpressionItem* ExpressionItemPool::getRandomOperation() const
{
  return randomItem(operations);
}
