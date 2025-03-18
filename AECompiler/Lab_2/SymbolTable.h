#pragma once
#include <map>
#include <string>
#include "Symbol.h"
#include "IdentifierType.h"
#include "format.h"
#include "SemanticException.h"

class SymbolTable
{
  static int idPool;

  static int getUniqueId()
  {
    return idPool++;
  }

  std::map<int, Symbol> symbols;

public:

  SymbolTable( ) {}

  int getIdOrCreate(const std::string& varName, IdentifierType idType = IdentifierType::INTEGER)
  {
    for (const auto& [id, symbol] : symbols)
      if (symbol == varName)
      {
        if (idType != symbol.getType())
          throw SemanticException(format("»дентификатор '{}' объ€влен с различными типами", { varName } ));

        return id;
      }

    int newId = getUniqueId();
    symbols[newId] = Symbol(varName, idType);

    return newId;
  }

  Symbol& getSymbol(int id)
  {
    auto symbolIter = symbols.find(id);

    if (symbolIter != symbols.end())
      return symbolIter->second;
  }

  const Symbol& getSymbol(int id) const
  {
    auto symbolIter = symbols.find(id);

    if (symbolIter != symbols.end())
      return symbolIter->second;
  }

  const std::map<int, Symbol>& allSymbols() const
  {
    return symbols;
  }

  std::map<int, Symbol>& allSymbols()
  {
    return symbols;
  }

};

