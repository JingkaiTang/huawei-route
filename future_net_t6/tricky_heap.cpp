#include "tricky_heap.h"

TrickyHeap::TrickyHeap(): size(0), fa(100) {
}

TrickyHeap::~TrickyHeap() {
}

RouteCursor &TrickyHeap::pop() {
  fa[0] = fa[1];
  fa[1] = fa[size];
  int cur_index = 1;
  int pre_index = size;
  size --;
  while (cur_index != pre_index) {
    pre_index = cur_index;
    cur_index = spin_down(cur_index);
  }
  return fa[0];
}

RouteCursor &TrickyHeap::get() {
  if (size+2 < fa.capacity) {
    size ++;
    return fa[size];
  } else {
    fa.extend();
    return get();
  }
}

void TrickyHeap::balance() {
  int cur_index = size;
  int pre_index = 1;
  while (cur_index != pre_index) {
    pre_index = cur_index;
    cur_index = spin_up(cur_index);
  }
}

int TrickyHeap::spin_up(int index) {
  int parent = index / 2;
  if (parent < 1) {
    return index;
  }
  if (fa[index] < fa[parent]) {
    fa[size+1] = fa[index];
    fa[index] = fa[parent];
    fa[parent] = fa[size+1];
    return parent;
  }
  return index;
}

int TrickyHeap::spin_down(int index) {
  int left = 2 * index;

  if (left > size) {
    return index;
  }

  int min = left;
  if (left < size) {
    int right = left + 1;
    min = fa[left] < fa[right] ? left : right;
  }

  if (fa[min] < fa[index]) {
    fa[size+1] = fa[index];
    fa[index] = fa[min];
    fa[min] = fa[size+1];
    return min;
  }

  return index;
}
