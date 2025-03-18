#pragma once

#include "Token.h"
#include "TokenType.h"

class Int2FloatToken : public Token
{
public:

  Int2FloatToken()
    : Token(TokenType::INT_TO_FLOAT)
  {
  }

  std::string toString() const override
  {
    return "<i2f>";
  }
};