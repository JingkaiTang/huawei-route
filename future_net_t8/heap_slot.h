#ifndef __HEAP_SLOT_H__
#define __HEAP_SLOT_H__

#include "route_cursor.h"
#include "fake_array.h"

class HeapSlot {
public:
  int size;
  int capacity;
  int extend_step;
  int bound;
  FakeArray<RouteCursor> fa;
  HeapSlot *next;
  HeapSlot(int b, int step = 64);
  void extend();
};

#endif
