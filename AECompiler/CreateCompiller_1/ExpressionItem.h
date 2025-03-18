#pragma once
#include <string>

using namespace std;

class ExpressionItem
{
  string view;
  string wordView;

public:

  ExpressionItem(string view, string wordView) : view(view), wordView(wordView)
  {

  }

  string View() const { return view; }
  string WordView() const { return wordView; }

};

