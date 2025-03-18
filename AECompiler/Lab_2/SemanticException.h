#pragma once
#include "LexicalException.h"

class SemanticException : public LexicalException
{
public:
  SemanticException(std::string msg) : LexicalException(msg) {}
};