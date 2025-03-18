#pragma once
#include <map>
#include <exception>

#include <TokenType.h>
#include <IdentifierType.h>

class BinaryCodes
{

  static std::map<TokenType, uint8_t> operationCodes;
  static std::map<TokenType, uint8_t> operandCodes;
  static std::map<IdentifierType, uint8_t> idTypeCodes;

  template<class TKey, class TValue, class TMap>
  static TKey findKeyByValue(TValue needValue, TMap& map)
  {
    for (const auto& [key, value] : map)
      if (needValue == value)
        return key;

    throw std::exception("Не найдено значения в map");
  }

public:

  static uint8_t operationCode(TokenType operationType)
  {
    return operationCodes[operationType];
  }

  static TokenType operationByCode(uint8_t operationCode)
  {
    return findKeyByValue<TokenType, uint8_t, decltype(operationCodes)>(operationCode, operationCodes);
  }


  static uint8_t operandCode(TokenType operandType)
  {
    return operandCodes[operandType];
  }

  static TokenType operandByCode(uint8_t operandCode)
  {
    return findKeyByValue<TokenType, uint8_t, decltype(operandCodes)>(operandCode, operandCodes);
  }


  static uint8_t idTypeCode(IdentifierType idType)
  {
    return idTypeCodes[idType];
  }

  static IdentifierType idTypeByCode(uint8_t idTypeCode)
  {
    return findKeyByValue<IdentifierType, uint8_t, decltype(idTypeCodes)>(idTypeCode, idTypeCodes);
  }

};