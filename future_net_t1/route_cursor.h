#ifndef __ROUTE_CURSOR_H__
#define __ROUTE_CURSOR_H__

#include <vector>

using namespace std;

struct RouteCursor {
  int cur_node;
  int cost;
  int pass_count;
  int value;
  vector<int> *path;
};

struct rc_greater {
  bool operator() (const RouteCursor *a, const RouteCursor *b) {
    return a->value > b->value;
  }
};

#endif
