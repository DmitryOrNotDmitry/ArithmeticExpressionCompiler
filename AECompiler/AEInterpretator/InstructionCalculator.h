#pragma once
#include <SymbolTable.h>

#include "BinaryAECodeReader.h"
#include <functional>

class InstructionCalculator
{
  SymbolTable& symbolTable;

  InstructionData* curInstruction;

  int getIntOperand(TokenType operandType, Symbol::TValue value);
  double getFloatOperand(TokenType operandType, Symbol::TValue value);

  template<class T>
  T calculate(T left, T right);

  double calculateI2F(int operand);

public:

  InstructionCalculator(SymbolTable& symbolTable)
    : symbolTable(symbolTable)
  {
    curInstruction = nullptr;
  }

  void execute(InstructionData& instruction);

};


