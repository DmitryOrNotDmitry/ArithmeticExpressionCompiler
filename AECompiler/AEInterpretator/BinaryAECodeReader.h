#pragma once
#include <File.h>
#include <SymbolTable.h>
#include <variant>
#include <TokenType.h>
#include <BinaryCodes.h>


struct InstructionData
{
  TokenType operation;
  int resultId;

  TokenType operand1Type;
  std::variant<int, double> operand1Value;

  TokenType operand2Type;
  std::variant<int, double> operand2Value;
};


class BinaryAECodeReader
{

  File in;
  SymbolTable& symbolTable;

  uint32_t countInstruction;
  uint32_t currentInstruction;

  TokenType readOperand(uint8_t operandTypeCode, std::variant<int, double>& operandValue);
  
public:

  BinaryAECodeReader(File& in, SymbolTable& symbolTable)
    : in(in)
    , symbolTable(symbolTable)
  {
    countInstruction = 0;
    currentInstruction = 0;
  }

  void readSymbolTable();

  bool IsDone();

  InstructionData readNext();

};

