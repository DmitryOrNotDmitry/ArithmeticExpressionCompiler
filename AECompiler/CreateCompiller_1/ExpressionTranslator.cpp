#include "ExpressionTranslator.h"
#include "Expression.h"

#include <stdexcept>

string ExpressionTranslator::translate(string line)
{
  Expression exp;

  for (char c : line)
  {
    if (c == ' ')
      continue;

    const ExpressionItem* item;
    string cStr({ c });

    item = pool.byView(cStr);
    
    if (item == nullptr)
      throw runtime_error("Ошибка трансляции. Нераспознан символ '" + cStr + "'");
  
    exp.add(item);
  }
  
  return exp.WordView();
}
