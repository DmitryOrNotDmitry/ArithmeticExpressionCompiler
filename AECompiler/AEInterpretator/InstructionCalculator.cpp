#include "InstructionCalculator.h"



int InstructionCalculator::getIntOperand(TokenType operandType, Symbol::TValue value)
{
  int operandValue = std::get<int>(value);
  
  if (operandType == TokenType::IDENTIFIER)
  {
    Symbol operandSymbol = symbolTable.getSymbol(operandValue);
    return operandSymbol.getValue<int>();
  }

  if (operandType == TokenType::INT_CONST)
  {
    return operandValue;
  }
  
  return 0;
}

double InstructionCalculator::getFloatOperand(TokenType operandType, Symbol::TValue value)
{
  if (operandType == TokenType::IDENTIFIER)
  {
    int symbolId = std::get<int>(value);

    Symbol operandSymbol = symbolTable.getSymbol(symbolId);
    return operandSymbol.getValue<double>();
  }

  if (operandType == TokenType::DOUBLE_CONST)
  {
    return std::get<double>(value);
  }

  return 0;
}

double InstructionCalculator::calculateI2F(int operand)
{
  return (double) operand;
}


void InstructionCalculator::execute(InstructionData& instruction)
{
  curInstruction = &instruction;

  Symbol& resultSymbol = symbolTable.getSymbol(curInstruction->resultId);

  if (resultSymbol.getType() == IdentifierType::INTEGER)
  {
    int operand1 = getIntOperand(curInstruction->operand1Type, curInstruction->operand1Value);
    int operand2 = getIntOperand(curInstruction->operand2Type, curInstruction->operand2Value);

    int operationResult = calculate(operand1, operand2);
    resultSymbol.setValue(operationResult);
  }
  else
  {
    double operationResult = 0.0;

    if (curInstruction->operation == TokenType::INT_TO_FLOAT)
    {
      int operand1 = getIntOperand(curInstruction->operand1Type, curInstruction->operand1Value);
      
      operationResult = calculateI2F(operand1);
    }
    else
    {
      double operand1 = getFloatOperand(curInstruction->operand1Type, curInstruction->operand1Value);
      double operand2 = getFloatOperand(curInstruction->operand2Type, curInstruction->operand2Value);

      operationResult = calculate(operand1, operand2);
    }

    resultSymbol.setValue(operationResult);
  }
  
}


template<class T>
T InstructionCalculator::calculate(T left, T right)
{
  TokenType operation = curInstruction->operation;

  if (operation == TokenType::OPERATOR_PLUS)
    return left + right;

  if (operation == TokenType::OPERATOR_MINUS)
    return left - right;

  if (operation == TokenType::OPERATOR_MUL)
    return left * right;

  if (operation == TokenType::OPERATOR_DIV)
    return left / right;
}
