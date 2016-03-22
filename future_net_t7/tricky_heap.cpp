#include "tricky_heap.h"

TrickyHeap::TrickyHeap(int mbd, int ses): MAX_BOUND(-1^(1<<(sizeof(int)*8-1))), size(0), min_bound_distance(mbd), slot_extend_step(ses) {
  slots = NULL;
}

RouteCursor &TrickyHeap::pop() {
  int target = size%slots->size;
  top_cursor = slots->fa[target];
  slots->size --;
  if (slots->size == 0) {
    slots = remove(slots);
  } else {
    slots->fa[target] = slots->fa[slots->size];
  }
  size --;
  return top_cursor;
}

RouteCursor &TrickyHeap::get() {
  return new_cursor;
}

void TrickyHeap::balance() {
  if (slots == NULL) {
    slots = divide(NULL);
  }
  HeapSlot *slot = slots;
  while (new_cursor.value > slot->bound) {
    slot = slot->next;
  }
  if (slot->size >= slot->capacity) {
    divide(slot);
    return balance();
  }
  slot->fa[size] = new_cursor;
  slot->size ++;
  size ++;
}

HeapSlot *TrickyHeap::divide(HeapSlot *slot) {
  if (slot == NULL) {
    slot = new HeapSlot(MAX_BOUND, slot_extend_step);
    slot->next = NULL;
    return slot;
  }
  int next_bound = MAX_BOUND;
  int this_bound = slot->bound;
  if (slot->next != NULL) {
    if (slot->next->next != NULL) {
      if (slot->next->bound - slot->bound < min_bound_distance) {
        return slot;
      } else {
        next_bound = (slot->bound + slot->next->bound) / 2;
      }
    } else {
      next_bound = slot->bound * 2;
    }
  } else {
    this_bound = min_bound_distance;
  }
  HeapSlot *next = new HeapSlot(next_bound, slot_extend_step);
  next->next = slot->next->next;
  slot->next = next;
  slot->bound = this_bound;
  slot->extend();
  return next;
}

HeapSlot *TrickyHeap::remove(HeapSlot *slot) {
  if (slot == NULL) {
    return NULL;
  }
  HeapSlot *next = slot->next;
  delete slot;
  return next;
}
