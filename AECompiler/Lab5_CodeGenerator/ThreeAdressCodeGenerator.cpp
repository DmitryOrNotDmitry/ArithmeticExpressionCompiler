#include "ThreeAddressCodeGenerator.h"

shared_ptr<Token> ThreeAddressCodeGenerator::createTmpIdentifier(IdentifierType type)
{
  int id = symbolTable.getIdOrCreate(TMP_VAR_NAME_PREFIX + to_string(countTmpVars++), type);

  return make_shared<IdToken>(id, -1);
}

void ThreeAddressCodeGenerator::operandPostProcess(shared_ptr<Token>& operand)
{
  
}

ThreeAddressCodeGenerator::ThreeAddressCodeGenerator(SymbolTable& symbolTable)
  : symbolTable(symbolTable)
  , countTmpVars(0)
{
}

bool ThreeAddressCodeGenerator::isFloatOperand(shared_ptr<Token>& token)
{
  if (!token->isOperand())
    return false;

  if (token->getType() == TokenType::IDENTIFIER)
  {
    if (symbolTable.getSymbol(token->getValue<int>()).getType() == IdentifierType::FLOAT)
    {
      return true;
    }
  }
  
  if (token->getType() == TokenType::DOUBLE_CONST)
  {
    return true;
  }

  return false;
}

shared_ptr<Token> ThreeAddressCodeGenerator::generateInstruction(shared_ptr<Node>& node)
{
  if (node->token->isOperand())
    return node->token;

  if (node->token->isOperator())
  {
    if (node->token->getType() == TokenType::INT_TO_FLOAT)
    {
      shared_ptr<Token> operand1 = generateInstruction(node->left);
     
      shared_ptr<Token> result = createTmpIdentifier(IdentifierType::FLOAT);
      
      operandPostProcess(operand1);

      instructions.push_back(ThreeAddressInstruction(node->token->getType(), result, operand1));

      return result;
    }
    else
    {
      shared_ptr<Token> operand1 = generateInstruction(node->left);
      shared_ptr<Token> operand2 = generateInstruction(node->right);

      IdentifierType tmpVarType = IdentifierType::INTEGER;

      if (isFloatOperand(operand1) || isFloatOperand(operand2))
      {
        tmpVarType = IdentifierType::FLOAT;
      }
      
      shared_ptr<Token> result = createTmpIdentifier(tmpVarType);

      operandPostProcess(operand1);
      operandPostProcess(operand2);

      instructions.push_back(ThreeAddressInstruction(node->token->getType(), result, operand1, operand2));
      
      return result;
    }
  }
}

const vector<ThreeAddressInstruction>& ThreeAddressCodeGenerator::generateCode(shared_ptr<Node>& syntaxTree)
{
  instructions.clear();

  generateInstruction(syntaxTree);

  return instructions;
}
