#include "heap_slot.h"

HeapSlot::HeapSlot(int b, int step): size(0), capacity(step), extend_step(step), bound(b), fa(step) {
}

void HeapSlot::extend() {
  fa.extend();
  capacity += extend_step;
}
