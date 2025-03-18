#pragma once
#include "Token.h"
#include <memory>
#include <iostream>
#include <cmath>



class Node {
public:
  std::shared_ptr<Token> token;

  std::shared_ptr<Node> left;
  std::shared_ptr<Node> right;

  Node(std::shared_ptr<Token>& token) : token(token) {}

  void setLeft(std::shared_ptr<Node>& leftChild) {
    left = leftChild;
  }

  void setRight(std::shared_ptr<Node>& rightChild) {
    right = rightChild;
  }

  template<class Output>
  void print(Output& out, int depth = 0) const {
    out << std::string(std::max(depth - 1, 0) * 4, ' ');
    
    if (depth > 0)
      out << "|---";

    out << token->toString() << "\n";
    
    if (left.get())
      left->print(out, depth + 1);
    
    if (right.get())
      right->print(out, depth + 1);
  }
};