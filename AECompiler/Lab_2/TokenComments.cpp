#include "TokenComments.h"


std::string TokenComments::getComment(TokenType type)
{
   auto commentIter = availableComments.find(type);

   if (commentIter != availableComments.end())
     return commentIter->second;

   return "[Не найден комментарий для лексемы]";
 }


std::string TokenComments::getIdentifierCommentToSymbol(IdentifierType type)
{
  if (type == IdentifierType::INTEGER)
    return " [целый]";

  if (type == IdentifierType::FLOAT)
    return " [вещественный]";

  return " [неизвествный]";
}


std::string TokenComments::getIdentifierComment(IdentifierType type)
{
  if (type == IdentifierType::INTEGER)
    return " целого типа";

  if (type == IdentifierType::FLOAT)
    return " вещественного типа";

  return " неизвествного типа";
}


const std::map<TokenType, std::string> TokenComments::availableComments = {
  {TokenType::OPERATOR_PLUS, "Операция сложения"},
  {TokenType::OPERATOR_MINUS, "Операция вычитания"},
  {TokenType::OPERATOR_MUL, "Операция умножения"},
  {TokenType::OPERATOR_DIV, "Операция деления"},
    
  {TokenType::OPEN_BRACKET, "Открывающая скобка"},
  {TokenType::CLOSE_BRACKET, "Закрывающая скобка"},

  {TokenType::INT_CONST, "Константа целого типа"},
  {TokenType::DOUBLE_CONST, "Константа вещественного типа"},
  
  {TokenType::IDENTIFIER, "Идентификатор с именем "},
};