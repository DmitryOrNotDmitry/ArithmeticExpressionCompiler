#pragma once

#include <string>
#include <memory>

#include "Node.h"
#include "Token.h"
#include "ThreeAddressInstruction.h"
#include "SymbolTable.h"

using namespace std;

class ThreeAddressCodeGenerator
{
protected:

  const string TMP_VAR_NAME_PREFIX = "@tmp";

  int countTmpVars;

  vector<ThreeAddressInstruction> instructions;

  SymbolTable& symbolTable;

  virtual shared_ptr<Token> createTmpIdentifier(IdentifierType type);

  virtual void operandPostProcess(shared_ptr<Token>& operand);

  bool isFloatOperand(shared_ptr<Token>& token);

  shared_ptr<Token> generateInstruction(shared_ptr<Node>& node);

public:

  ThreeAddressCodeGenerator(SymbolTable& symbolTable);

  const vector<ThreeAddressInstruction>& generateCode(shared_ptr<Node>& syntaxTree);

};

