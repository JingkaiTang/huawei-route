#ifndef __TRICKY_HEAP_H__
#define __TRICKY_HEAP_H__

#include "route_cursor.h"
#include "fake_array.h"

class TrickyHeap {
public:
  int size;
  TrickyHeap();
  ~TrickyHeap();
  RouteCursor &pop();
  RouteCursor &get();
  void balance();

private:
  FakeArray<RouteCursor> fa;
  int spin_up(int index);
  int spin_down(int index);

};

#endif
