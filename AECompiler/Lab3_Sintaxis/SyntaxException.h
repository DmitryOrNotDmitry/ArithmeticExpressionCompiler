#pragma once
#include "LexicalException.h"


class SyntaxException : public LexicalException
{
public:
  SyntaxException(std::string msg) : LexicalException(msg) {}
};