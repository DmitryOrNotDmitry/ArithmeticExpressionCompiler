#include "OptimizeThreeAddressGenerator.h"

bool OptimizeThreeAddressGenerator::isTmpIdentifier(shared_ptr<Token>& token)
{
  if (token->getType() != TokenType::IDENTIFIER)
    return false;

  Symbol idSymbol = symbolTable.getSymbol(token->getValue<int>());

  return idSymbol.getName()[0] == '@';
}

pair<bool, shared_ptr<Token>> OptimizeThreeAddressGenerator::findFreeTmpId(IdentifierType needTmpIdType)
{
  for (size_t i = 0; i < freeTmpIdentifier.size(); i++)
  {
    Symbol idSymbol = symbolTable.getSymbol(freeTmpIdentifier[i]->getValue<int>());

    if (idSymbol.getType() == needTmpIdType)
    {
      auto result = make_pair(true, freeTmpIdentifier[i]);
      freeTmpIdentifier.erase(freeTmpIdentifier.begin() + i);

      return result;
    }
  }

  return make_pair(false, nullptr);
}

shared_ptr<Token> OptimizeThreeAddressGenerator::createTmpIdentifier(IdentifierType type)
{
  auto pair = findFreeTmpId(type);

  if (pair.first)
    return pair.second;
  
  return ThreeAddressCodeGenerator::createTmpIdentifier(type);
}

void OptimizeThreeAddressGenerator::operandPostProcess(shared_ptr<Token>& operand)
{
  if (isTmpIdentifier(operand))
    freeTmpIdentifier.push_back(operand);
}

OptimizeThreeAddressGenerator::OptimizeThreeAddressGenerator(SymbolTable& symbolTable)
  : ThreeAddressCodeGenerator(symbolTable)
{
}
