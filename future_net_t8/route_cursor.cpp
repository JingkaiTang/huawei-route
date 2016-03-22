#include "route_cursor.h"

#define NULL 0

bool operator< (const RouteCursor &a, const RouteCursor &b) {
  return a.value < b.value;
}

void RouteCursor::reset() {
  cur_node = 0;
  cost = 0;
  pass_count = 0;
  value = 0;
  path_size = 0;
  path = NULL;
  bitmap = NULL;
}

void RouteCursor::recycle() {
  int *tmp_path = path;
  delete[] tmp_path;
  Bitmap *tmp_bitmap = bitmap;
  delete tmp_bitmap;
  reset();
}
