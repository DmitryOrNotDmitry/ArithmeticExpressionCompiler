#include "BinaryCodes.h"



std::map<TokenType, uint8_t> BinaryCodes::operationCodes = {
  {TokenType::OPERATOR_PLUS, 0},
  {TokenType::OPERATOR_MINUS, 1},
  {TokenType::OPERATOR_MUL, 2},
  {TokenType::OPERATOR_DIV, 3},
  {TokenType::INT_TO_FLOAT, 4},
};

std::map<TokenType, uint8_t> BinaryCodes::operandCodes = {
  {TokenType::IDENTIFIER, 0},
  {TokenType::INT_CONST, 1},
  {TokenType::DOUBLE_CONST, 2},
};

std::map<IdentifierType, uint8_t> BinaryCodes::idTypeCodes= {
  {IdentifierType::INTEGER, 0},
  {IdentifierType::FLOAT, 1},
};