#pragma once
#include "Expression.h"
#include "ExpressionItemPool.h"


class ExpressionGenerator
{

  const ExpressionItemPool& pool;
  
public:

  ExpressionGenerator(const ExpressionItemPool& pool) : pool(pool) { }

  Expression generate(int minOperators, int maxOperators);

};

