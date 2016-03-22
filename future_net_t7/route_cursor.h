#ifndef __ROUTE_CURSOR_H__
#define __ROUTE_CURSOR_H__

#include "bitmap.h"

using namespace std;

struct RouteCursor {
  int cur_node;
  int cost;
  int pass_count;
  int value;
  int path_size;
  int *path;
  Bitmap *bitmap;
};

bool operator< (const RouteCursor &a, const RouteCursor &b);

struct rc_greater {
  bool operator() (const RouteCursor *a, const RouteCursor *b) {
    return a->value > b->value;
  }
};

#endif
