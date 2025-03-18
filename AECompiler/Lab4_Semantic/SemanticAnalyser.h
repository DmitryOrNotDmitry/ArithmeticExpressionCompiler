#pragma once
#include <vector>

#include "Node.h"
#include "Token.h"
#include "SymbolTable.h"


class SemanticAnalyser
{

  SymbolTable& symbolTable;

  void zeroCheck(std::shared_ptr<Node> syntaxTree);

  bool containsFloatNode(std::shared_ptr<Node> node);

  void addNodeConvertInt2Float(std::shared_ptr<Node> node, IdentifierType rightNodeType);

  void checkFloats(std::shared_ptr<Node> node);

  IdentifierType getNodeType(std::shared_ptr<Node> node);

public:

  SemanticAnalyser(SymbolTable& symbolTable)
    : symbolTable(symbolTable)
  {
  }

  std::shared_ptr<Node> modifySyntaxTree(std::shared_ptr<Node> syntaxTree);

};

