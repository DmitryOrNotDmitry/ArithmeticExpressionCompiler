#include "Expression.h"
#include <iostream>

void Expression::add(const ExpressionItem* item)
{
  items.push_back(item);
}

string Expression::View() const
{
  string expView;

  for (auto item : items)
    expView.append(item->View()).append(" ");

  expView.resize(expView.size() - 1);
  
  return expView;
}

string Expression::WordView() const
{
  string expView;
  
  for (auto item : items)
    expView.append(item->WordView()).append(" ");

  expView.resize(expView.size() - 1);

  return expView;
}
