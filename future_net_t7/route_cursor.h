#ifndef __ROUTE_CURSOR_H__
#define __ROUTE_CURSOR_H__

#include "bitmap.h"

using namespace std;

class RouteCursor {
public:
  int cur_node;
  int cost;
  int pass_count;
  int value;
  int path_size;
  int *path;
  Bitmap *bitmap;

  void reset();
  void recycle();
};

bool operator< (const RouteCursor &a, const RouteCursor &b);

#endif
