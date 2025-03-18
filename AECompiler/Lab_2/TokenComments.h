#pragma once
#include <string>
#include <map>

#include "TokenType.h"
#include "IdentifierType.h"

class TokenComments
{

  TokenComments() = delete;
  TokenComments(const TokenComments&) = delete;
  TokenComments(TokenComments&&) = delete;

  static const std::map<TokenType, std::string> availableComments;

public:

  static std::string getComment(TokenType type);

  static std::string getIdentifierComment(IdentifierType type);

  static std::string getIdentifierCommentToSymbol(IdentifierType type);

};
