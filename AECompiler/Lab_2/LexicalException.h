#pragma once
#include <exception>
#include <string>


class LexicalException : public std::exception {
private:
  std::string message;

public:

  LexicalException(const std::string& msg) : message(msg) {}

  virtual const char* what() const noexcept override {
    return message.c_str();
  }
};