#pragma once
#include <vector>
#include "ExpressionItem.h"

class Expression
{

  vector<const ExpressionItem*> items;

public:
  Expression() {}

  void add(const ExpressionItem* item);
  
  string View() const;
  string WordView() const;

};

