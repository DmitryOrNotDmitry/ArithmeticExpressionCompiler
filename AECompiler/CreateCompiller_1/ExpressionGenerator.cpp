#include "ExpressionGenerator.h"

Expression ExpressionGenerator::generate(int minOperators, int maxOperators)
{
  Expression exp;

  int countOperators = minOperators + rand() % (maxOperators - minOperators + 1);
  
  for (int i = 1; i < countOperators; i++)
  {
    exp.add(pool.getRandomOperator());
    exp.add(pool.getRandomOperation());
  }

  exp.add(pool.getRandomOperator());

  return exp;
}
