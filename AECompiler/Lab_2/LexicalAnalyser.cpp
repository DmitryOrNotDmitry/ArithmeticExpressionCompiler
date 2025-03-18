#include "LexicalAnalyser.h"
#include "format.h"
#include "IdToken.h"

#include <ctype.h>
#include "LexicalException.h"
#include "IdentifierType.h"


bool isIdentifierChar(char c)
{
  return isalnum(c) || c == '_';
}


vector<GroupRule> LexicalAnalyser::rules = {
    GroupRule(
      [](char prev, char cur)
      {
        return isdigit(prev) && isdigit(cur);
      }
    ),

    GroupRule(
      [](char prev, char cur)
      {
        return (isdigit(prev) && cur == '.') || (prev == '.' && isdigit(cur));
      }
    ),

    GroupRule(
      [](char prev, char cur)
      {
        return isIdentifierChar(prev) && isIdentifierChar(cur);
      }
    ),

    GroupRule(
      [](char prev, char cur)
      {
        return isIdentifierChar(prev) && cur == '['
          || prev == '[' && isIdentifierChar(cur)
          || isIdentifierChar(prev) && cur == ']';
      }
    )
};


vector<pair<regex, TokenType>> LexicalAnalyser::lexemeRegexToTypes = {
  {regex(R"([+])"), TokenType::OPERATOR_PLUS},
  {regex(R"([\-])"), TokenType::OPERATOR_MINUS},
  {regex(R"([*])"), TokenType::OPERATOR_MUL},
  {regex(R"([/])"), TokenType::OPERATOR_DIV},

  {regex(R"([)])"), TokenType::CLOSE_BRACKET},
  {regex(R"([(])"), TokenType::OPEN_BRACKET},

  {regex(R"(\d+)"), TokenType::INT_CONST},

  {regex(R"(\d+(\.\d+)*)"), TokenType::DOUBLE_CONST},

  {regex(R"(\w+(\[\w+\])?)"), TokenType::IDENTIFIER},
};


IdentifierType getIDTypeByString(const std::string& str)
{
  if (str_tolower(str) == "f")
    return IdentifierType::FLOAT;

  if (str_tolower(str) == "i")
    return IdentifierType::INTEGER;

  return IdentifierType::UNKNOWN;
}


IdentifierType getIdLexemeType(const std::string& idLexeme)
{
  size_t typeStart = idLexeme.find('[');
  if (typeStart != std::string::npos)
  {
    std::string idTypeString = idLexeme.substr(typeStart + 1, idLexeme.size() - (typeStart + 1) - 1);

    return getIDTypeByString(idTypeString);
  }

  return IdentifierType::INTEGER;
}


map<TokenType, function<pair<bool, string>(const string&)>> LexicalAnalyser::tokenTypeValids = {
  {
    TokenType::DOUBLE_CONST, 
    [](const string& lexeme) -> pair<bool, string> {
      if (std::count(lexeme.begin(), lexeme.end(), '.') > 1)
        return make_pair(false, "Лексическая ошибка! Неправильно задана константа '{}' на позиции {}");

      return make_pair(true, "");
    }
  },

  {
    TokenType::IDENTIFIER, 
    [](const string& lexeme) -> pair<bool, string> {
      if (isdigit(lexeme[0]))
        return make_pair(false, "Идентификатор '{}' не может начинаться с цифры на позиции {}");

      if (getIdLexemeType(lexeme) == IdentifierType::UNKNOWN)
        return make_pair(false, "Неизвестный тип у идентификатора '{}' на позиции {}");

      return make_pair(true, "");
    }
  },
};


vector<string> LexicalAnalyser::separateToLexemes(const string& arithmeticExpr)
{
  vector<string> lexemes;

  vector<int> groups;
  groups.resize(arithmeticExpr.size() + 1, -1);
  int groupId = 0;

  char predChar = ' ';
  for (int i = 0; i < arithmeticExpr.size(); i++)
  {
    char curChar = arithmeticExpr[i];

    if (isspace(curChar))
    {
      groups[i] = -1;
    }
    else
    {
      if (!fitAnyRule(predChar, curChar))
        groupId++;

      groups[i] = groupId;

    }

    predChar = curChar;
  }


  int from = 0;
  int predGroup = -1;
  for (int i = 0; i < groups.size(); i++)
  {
    if (predGroup == groups[i])
      continue;

    if (i > 0)
      lexemes.push_back(arithmeticExpr.substr(from, i - from));

    from = i;
    predGroup = groups[i];
  }

  return lexemes;
}


TokenType LexicalAnalyser::defineTokenType(const string& lexeme)
{
  for (const auto& item : lexemeRegexToTypes)
  {
    auto regExpr = item.first;
    TokenType tokenType = item.second;

    if (regex_match(lexeme, regExpr))
    {
      return tokenType;
    }
  }

  return TokenType::UNKNOWN;
}


vector<shared_ptr<Token>> LexicalAnalyser::process(const string& arithmeticExpr)
{
  vector<string> lexemes = separateToLexemes(arithmeticExpr);

  vector<shared_ptr<Token>> tokens;
  int lexemePosition = 0;

  for (auto lexeme : lexemes)
  {
    if (isspace(lexeme[0]))
    {
      lexemePosition += lexeme.size();
      continue;
    }

    TokenType tokenType = defineTokenType(lexeme);

    if (tokenType == TokenType::UNKNOWN)
      throw LexicalException(format("Недопустимая лексема '{}' на позиции {}", { lexeme, to_string(lexemePosition) }));


    auto lexemeValidator = tokenTypeValids.find(tokenType);
    if (lexemeValidator != tokenTypeValids.end())
    {
      auto proveResult = lexemeValidator->second(lexeme);

      bool succes = proveResult.first;
      string messageError = proveResult.second;

      if (not succes)
      {
        throw LexicalException(format(messageError, { lexeme, to_string(lexemePosition) }));
      }
    }


    if (tokenType == TokenType::IDENTIFIER)
    {
      std::string idLexeme = lexeme;
      IdentifierType idType = getIdLexemeType(idLexeme);

      size_t typeStrStart = idLexeme.find('[');
      if (typeStrStart != std::string::npos)
        idLexeme.erase(idLexeme.begin() + typeStrStart, idLexeme.end());

      tokens.push_back(make_shared<IdToken>(symbolTable.getIdOrCreate(idLexeme, idType), lexemePosition));

    }
    
    else if (tokenType == TokenType::INT_CONST)
      tokens.push_back(make_shared<Token>(tokenType, stoi(lexeme), lexemePosition));
    
    else if (tokenType == TokenType::DOUBLE_CONST)
      tokens.push_back(make_shared<Token>(tokenType, stod(lexeme), lexemePosition));
    
    else
      tokens.push_back(make_shared<Token>(tokenType, lexeme, lexemePosition));

    lexemePosition += lexeme.size();
  }

  return tokens;
}


bool LexicalAnalyser::fitAnyRule(char prev, char cur)
{
  for (auto& rule : rules)
    if (rule.check(prev, cur))
      return true;

  return false;
}
