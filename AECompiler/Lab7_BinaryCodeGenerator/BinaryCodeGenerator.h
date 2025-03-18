#pragma once
#include "SymbolTable.h"
#include "File.h"
#include "ThreeAddressInstruction.h"

class BinaryCodeGenerator
{
  File& out;
  const SymbolTable& symbolTable;
  const std::vector<ThreeAddressInstruction> instructions;
  
  void outOperand(shared_ptr<Token> operand);

public:

  BinaryCodeGenerator(File& out, const SymbolTable& symbolTable, const std::vector<ThreeAddressInstruction> instructions);

  void run();

};

