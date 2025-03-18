#pragma once
#include <functional>

class GroupRule
{

  std::function<bool(char, char)> ruleChecker;

public:

  GroupRule(std::function<bool(char, char)> ruleChecker)
    : ruleChecker(ruleChecker)
  {
  }

  bool check(char prevChar, char curChar)
  {
    return ruleChecker(prevChar, curChar);
  }

};

