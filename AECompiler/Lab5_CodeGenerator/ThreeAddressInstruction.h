#pragma once

#include <string>
#include <IdToken.h>

using namespace std;

class ThreeAddressInstruction
{

  TokenType operation;

  shared_ptr<Token> result;

  shared_ptr<Token> operand1;
  shared_ptr<Token> operand2;

  string getOperationCodeView() const
  {
    if (operation == TokenType::INT_TO_FLOAT)
      return "i2f";

    if (operation == TokenType::OPERATOR_MUL)
      return "mul";

    if (operation == TokenType::OPERATOR_DIV)
      return "div";

    if (operation == TokenType::OPERATOR_PLUS)
      return "add";

    if (operation == TokenType::OPERATOR_MINUS)
      return "sub";
  }

public:

  ThreeAddressInstruction(
    TokenType operation,
    shared_ptr<Token> result,
    shared_ptr<Token> operand1,
    shared_ptr<Token> operand2
  )
    : operation(operation)
    , result(result)
    , operand1(operand1)
    , operand2(operand2)
  {
  }

  ThreeAddressInstruction(
    TokenType operation,
    shared_ptr<Token> result,
    shared_ptr<Token> operand1
  )
    : operation(operation)
    , result(result)
    , operand1(operand1)
  {
  }

  TokenType getOperation() const
  {
    return operation;
  }

  shared_ptr<Token> getResult() const
  {
    return result;
  }

  shared_ptr<Token> getOperand1() const
  {
    return operand1;
  }

  shared_ptr<Token> getOperand2() const
  {
    return operand2;
  }

  string toString() const
  {
    string str = getOperationCodeView() + " "
      + result->toString() + " "
      + operand1->toString();
    
    if (operand2.get())
      str += " " + operand2->toString();

    return str;
  }

};

