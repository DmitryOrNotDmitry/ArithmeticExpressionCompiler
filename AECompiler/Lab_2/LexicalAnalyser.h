#pragma once
#include <vector>
#include <memory>
#include <regex>

#include "GroupRule.h"
#include "Token.h"
#include "SymbolTable.h"

using namespace std;


class LexicalAnalyser
{
  
  static vector<GroupRule> rules;
  
  static vector<pair<regex, TokenType>> lexemeRegexToTypes;

  static map<TokenType, function<pair<bool, string>(const string&)>> tokenTypeValids;

  static bool fitAnyRule(char prev, char cur);
  

  SymbolTable& symbolTable;

  vector<string> separateToLexemes(const string& arithmeticExpr);

  TokenType defineTokenType(const string& lexeme);

public:

  LexicalAnalyser(SymbolTable& symbolTable) 
    : symbolTable(symbolTable) 
  {}

  vector<shared_ptr<Token>> process(const string& arithmeticExpr);

};
