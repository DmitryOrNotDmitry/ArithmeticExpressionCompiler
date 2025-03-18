#include "format.h"
#include <algorithm>

std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
  std::vector<std::string> tokens;
  size_t start = 0;
  size_t end = 0;

  while ((end = str.find(delimiter, start)) != std::string::npos) {
    // Добавляем подстроку до разделителя
    tokens.push_back(str.substr(start, end - start));
    // Смещаем начало после разделителя
    start = end + delimiter.length();
  }

  // Добавляем последнюю часть строки после последнего разделителя
  tokens.push_back(str.substr(start));

  return tokens;
}


std::string format(const std::string& formatStr, const std::vector<std::string>& params)
{
  std::string result;

  std::vector<std::string> tokens = split(formatStr, "{}");

  for (int i = 0; i < tokens.size() - 1; i++)
    result.append(tokens[i]).append(params[i]);

  return result.append(tokens.back());
}


std::string str_tolower(std::string s)
{
  std::transform(s.begin(), s.end(), s.begin(),
    [](unsigned char c) { return std::tolower(c); }
  );
  return s;
}