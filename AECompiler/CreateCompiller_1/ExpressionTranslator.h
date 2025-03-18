#pragma once
#include "ExpressionItemPool.h"

class ExpressionTranslator
{

  const ExpressionItemPool& pool;

public:

  ExpressionTranslator(const ExpressionItemPool& pool) : pool(pool) { }

  string translate(string line);

};

