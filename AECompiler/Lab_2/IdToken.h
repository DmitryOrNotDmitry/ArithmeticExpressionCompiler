#pragma once
#include "Token.h"
#include "SymbolTable.h"

class IdToken : public Token
{

public:

  static const SymbolTable* currentTable;

  IdToken(int id, int position)
    : Token(TokenType::IDENTIFIER, id, position)
  {
  }

  std::string toStringWithComment() const override
  {
    int id = std::get<int>(value);
    Symbol idSymbol = currentTable->getSymbol(id);
    return toString() + "\t - " + TokenComments::getComment(type) + idSymbol.getName() + TokenComments::getIdentifierComment(idSymbol.getType());
  }

  std::string toString() const override
  {
    int id = std::get<int>(value);
    return "<id, " + std::to_string(id) + ">";
  }

};

