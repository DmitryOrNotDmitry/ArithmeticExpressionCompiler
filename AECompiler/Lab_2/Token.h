#pragma once

#include "TokenComments.h"
#include "TokenType.h"

#include <string>
#include <variant>

typedef std::variant<int, double, std::string> ValueType;

class Token
{
protected:


  TokenType type;
  ValueType value;
  int position;

  static std::string doubleToStr(double value)
  {
    std::string str = std::to_string(value);

    size_t minLength = str.find('.') + 2;

    while (str.length() > minLength && str.back() == '0')
      str.pop_back();

    return str;
  }

public:

  Token(TokenType type)
    : type(type)
    , position(-1)
    , value("")
  {
  }

  Token(TokenType type, ValueType value, int position)
    : type(type)
    , value(value)
    , position(position)
  {
  }

  int getPosition() const { return position; }

  TokenType getType() const { return type; }

  bool isOperator() const
  {
    return isBinaryOperator()
      || type == TokenType::INT_TO_FLOAT;
  }

  bool isBinaryOperator() const
  {
    return type == TokenType::OPERATOR_PLUS
      || type == TokenType::OPERATOR_MINUS
      || type == TokenType::OPERATOR_DIV
      || type == TokenType::OPERATOR_MUL;
  }

  bool isBracket() const
  {
    return type == TokenType::OPEN_BRACKET
      || type == TokenType::CLOSE_BRACKET;
  }

  bool isOperand() const
  {
    return type == TokenType::INT_CONST
      || type == TokenType::DOUBLE_CONST
      || type == TokenType::IDENTIFIER;
  }

  bool isConstOperand() const
  {
    return type == TokenType::INT_CONST
      || type == TokenType::DOUBLE_CONST;
  }

  template<class T>
  T getValue() const
  {
    return std::get<T>(value);
  }

  virtual std::string toStringWithComment() const
  {
    std::string strView = toString();

    strView.append("\t- ");
    strView.append(TokenComments::getComment(type));

    return strView;
  }

  virtual std::string toString() const
  {
    std::string strView("<");

    if (type == TokenType::DOUBLE_CONST)
      strView.append(doubleToStr(getValue<double>()));

    else
    {
      if (type == TokenType::IDENTIFIER || type == TokenType::INT_CONST)
        strView.append(std::to_string(getValue<int>()));      
      else
        strView.append(getValue<std::string>());
    }

    strView.append(">");

    return strView;
  }

};

