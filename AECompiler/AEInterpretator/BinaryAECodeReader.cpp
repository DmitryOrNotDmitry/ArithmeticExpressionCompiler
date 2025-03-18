#include "BinaryAECodeReader.h"


TokenType BinaryAECodeReader::readOperand(uint8_t operandTypeCode, std::variant<int, double>& operandValue)
{
  TokenType operand1Type = BinaryCodes::operandByCode(operandTypeCode);
  if (operand1Type == TokenType::DOUBLE_CONST)
  {
    double value;
    in.readBinary(value);
    operandValue = value;
  }
  else
  {
    int value;
    in.readBinary(value);
    operandValue = value;
  }

  return operand1Type;
}


void BinaryAECodeReader::readSymbolTable()
{
  char acronym[4];

  in.readBinary(acronym[0]);
  in.readBinary(acronym[1]);
  in.readBinary(acronym[2]);
  in.readBinary(acronym[3]);
  std::string actual(acronym, 4);

  if (actual != "ABPC")
    throw std::exception("Некорректный формат файла: ожидался акроним 'ABPC'");

  uint32_t countSymbols = 0;
  in.readBinary(countSymbols);

  for (uint32_t i = 0; i < countSymbols; i++)
  {
    std::string varName;
    uint8_t varTypeCode;

    in.readBinary(varName);
    in.readBinary(varTypeCode);

    IdentifierType idType = BinaryCodes::idTypeByCode(varTypeCode);
    symbolTable.getIdOrCreate(varName, idType);
  }

  uint32_t tmp;
  in.readBinary(tmp);
  countInstruction = tmp;
}

bool BinaryAECodeReader::IsDone()
{
  return currentInstruction == countInstruction;
}

InstructionData BinaryAECodeReader::readNext()
{
  if (IsDone())
    throw std::exception("Нет больше инструкций для чтения из файла");

  currentInstruction++;

  uint8_t operationCode;
  uint8_t operand1TypeCode;
  uint8_t operand2TypeCode;


  InstructionData newInstr;

  in.readBinary(operationCode);
  TokenType operationType = BinaryCodes::operationByCode(operationCode);
  newInstr.operation = operationType;

  in.readBinary(newInstr.resultId);

  in.readBinary(operand1TypeCode);
  newInstr.operand1Type = readOperand(operand1TypeCode, newInstr.operand1Value);

  if (operationType != TokenType::INT_TO_FLOAT)
  {
    in.readBinary(operand2TypeCode);
    newInstr.operand2Type = readOperand(operand2TypeCode, newInstr.operand2Value);
  }

  return newInstr;
}