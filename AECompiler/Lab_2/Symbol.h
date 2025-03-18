#pragma once

#include "IdentifierType.h"

#include <string>
#include <variant>

class Symbol {
public:
  typedef std::variant<int, double> TValue;


private:

  std::string name;
  IdentifierType type;

  TValue value;

public:


  Symbol() : name("null"), type(IdentifierType::UNKNOWN)
  { }
    
  Symbol(const std::string& name, IdentifierType type) : name(name), type(type)
  { }

  const std::string& getName() const
  {
    return name;
  }

  IdentifierType getType() const
  {
    return type;
  }

  void setValue(TValue newValue)
  {
    value = newValue;
  }

  template<class T>
  T getValue() const
  {
    return std::get<T>(value);
  }

  bool operator==(const Symbol& other) const
  {
    return name == other.name;
  }

  bool operator==(const std::string& otherName) const
  {
    return name == otherName;
  }

};