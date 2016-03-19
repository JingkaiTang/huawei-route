#ifndef __ROUTE_CURSOR_H__
#define __ROUTE_CURSOR_H__

#include "data.h"

#include <vector>

using namespace std;

struct RouteCursor {
  int cur_node;
  int cost;
  int pass_count;
  int value;
  vector<int> path;
};

bool operator> (RouteCursor a, RouteCursor b);

#endif
