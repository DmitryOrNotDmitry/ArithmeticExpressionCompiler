#include "TokenComments.h"


std::string TokenComments::getComment(TokenType type)
{
   auto commentIter = availableComments.find(type);

   if (commentIter != availableComments.end())
     return commentIter->second;

   return "[�� ������ ����������� ��� �������]";
 }


std::string TokenComments::getIdentifierCommentToSymbol(IdentifierType type)
{
  if (type == IdentifierType::INTEGER)
    return " [�����]";

  if (type == IdentifierType::FLOAT)
    return " [������������]";

  return " [������������]";
}


std::string TokenComments::getIdentifierComment(IdentifierType type)
{
  if (type == IdentifierType::INTEGER)
    return " ������ ����";

  if (type == IdentifierType::FLOAT)
    return " ������������� ����";

  return " ������������� ����";
}


const std::map<TokenType, std::string> TokenComments::availableComments = {
  {TokenType::OPERATOR_PLUS, "�������� ��������"},
  {TokenType::OPERATOR_MINUS, "�������� ���������"},
  {TokenType::OPERATOR_MUL, "�������� ���������"},
  {TokenType::OPERATOR_DIV, "�������� �������"},
    
  {TokenType::OPEN_BRACKET, "����������� ������"},
  {TokenType::CLOSE_BRACKET, "����������� ������"},

  {TokenType::INT_CONST, "��������� ������ ����"},
  {TokenType::DOUBLE_CONST, "��������� ������������� ����"},
  
  {TokenType::IDENTIFIER, "������������� � ������ "},
};