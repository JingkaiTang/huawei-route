#ifndef __TRICKY_HEAP_H__
#define __TRICKY_HEAP_H__

#include "route_cursor.h"
#include "fake_array.h"

// root index is 1, 0 for pop top
class TrickyHeap {
public:
  int size;
  FakeArray<RouteCursor> fa;
  TrickyHeap();
  ~TrickyHeap();
  RouteCursor &pop();
  RouteCursor &get();
  void balance();
  int spin_up(int index);
  int spin_down(int index);
};

#endif
