#pragma once
#include <memory>

#include "Node.h"


using namespace std;

class ExpressionOptimizator
{
  void calcConstOperations(shared_ptr<Node>& node);

  void calcInt2FloatWithConst(shared_ptr<Node>& node);

  void calcEvidentOperations(shared_ptr<Node>& node);

  void recursive_run(shared_ptr<Node>& node);

public:

  void run(shared_ptr<Node>& modifySyntaxTree);

};

