#pragma once
#include "ThreeAddressCodeGenerator.h"

#include <queue>
#include <vector>

using namespace std;

class OptimizeThreeAddressGenerator : public ThreeAddressCodeGenerator
{

  vector<shared_ptr<Token>> freeTmpIdentifier;
  
  bool isTmpIdentifier(shared_ptr<Token>& token);

  pair<bool, shared_ptr<Token>> findFreeTmpId(IdentifierType tmpIdType);

protected:

  shared_ptr<Token> createTmpIdentifier(IdentifierType type) override;

  void operandPostProcess(shared_ptr<Token>& operand) override;

public:

  OptimizeThreeAddressGenerator(SymbolTable& symbolTable);
};

