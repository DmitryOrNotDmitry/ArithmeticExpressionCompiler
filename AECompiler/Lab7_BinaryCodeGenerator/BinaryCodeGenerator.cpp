#include "BinaryCodeGenerator.h"
#include "BinaryCodes.h"

#include <cstdint>


uint8_t getOperationCode(TokenType operationType)
{
  return BinaryCodes::operationCode(operationType);
}

uint8_t getOperandCode(TokenType operandType)
{
  return BinaryCodes::operandCode(operandType);
}

uint8_t getIDTypeCode(IdentifierType idType)
{
  return BinaryCodes::idTypeCode(idType);
}


void BinaryCodeGenerator::outOperand(shared_ptr<Token> operand)
{
  TokenType operand1Type = operand->getType();
  out.writeBinary(getOperandCode(operand1Type));

  if (operand1Type == TokenType::DOUBLE_CONST)
    out.writeBinary(operand->getValue<double>());
  else
    out.writeBinary(operand->getValue<int>());
}

BinaryCodeGenerator::BinaryCodeGenerator(File& out, const SymbolTable& symbolTable, const std::vector<ThreeAddressInstruction> instructions)
  : out(out)
  , symbolTable(symbolTable)
  , instructions(instructions)
{
}


void BinaryCodeGenerator::run()
{
  out << "ABPC";



  uint32_t countSymbols = symbolTable.allSymbols().size();

  out.writeBinary(countSymbols);

  for (const auto& [id, symbol] : symbolTable.allSymbols())
  {
    out << symbol.getName() << '\0';

    out.writeBinary(getIDTypeCode(symbol.getType()));
  }



  uint32_t countInstructions = instructions.size();

  out.writeBinary(countInstructions);

  for (uint32_t i = 0; i < countInstructions; i++)
  {
    out.writeBinary( getOperationCode(instructions[i].getOperation()) );

    out.writeBinary( instructions[i].getResult()->getValue<int>() );

    outOperand(instructions[i].getOperand1());
    
    if (instructions[i].getOperation() == TokenType::INT_TO_FLOAT)
      continue;

    outOperand(instructions[i].getOperand2());
  }
}
