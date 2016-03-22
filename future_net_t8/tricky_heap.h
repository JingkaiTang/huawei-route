#ifndef __TRICKY_HEAP_H__
#define __TRICKY_HEAP_H__

#include "route_cursor.h"
#include "heap_slot.h"

class TrickyHeap {
public:
  int MAX_BOUND;
  int size;
  RouteCursor top_cursor;
  RouteCursor new_cursor;
  int min_bound_distance;
  int slot_extend_step;
  HeapSlot *slots;
  TrickyHeap(int mbd = 512, int ses = 64);
  RouteCursor &pop();
  RouteCursor &get();
  void balance();
private:
  HeapSlot *divide(HeapSlot *slot);
  HeapSlot *remove(HeapSlot *slot);
};

#endif
