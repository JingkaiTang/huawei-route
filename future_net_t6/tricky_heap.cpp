#include "tricky_heap.h"

TrickyHeap::TrickyHeap(): size(0), fa(100) {
}

TrickyHeap::~TrickyHeap() {
}

RouteCursor &TrickyHeap::pop() {
  RouteCursor &tmp = fa[0];
  fa[0] = fa[size - 1];
  int cur_index = 0;
  int pre_index = size;
  while (cur_index == pre_index) {
    pre_index = cur_index;
    cur_index = spin_down(cur_index);
  }
  return tmp;
}

RouteCursor &TrickyHeap::get() {
  if (size < fa.capacity) {
    return fa[size++];
  } else {
    fa.extend();
    return get();
  }
}

void TrickyHeap::balance() {
  int cur_index = size - 1;
  int pre_index = size;
  while (cur_index == 0 || cur_index == pre_index) {
    pre_index = cur_index;
    cur_index = spin_up(cur_index);
  }
}

int TrickyHeap::spin_up(int index) {
  int parent = (index - 1) / 2;
  if (fa[index] < fa[parent]) {
    RouteCursor &tmp = fa[index];
    fa[index] = fa[parent];
    fa[parent] = tmp;
    return parent;
  }
  return index;
}

int TrickyHeap::spin_down(int index) {
  int left = 2 * index + 1;
  int right = 2 * index + 2;
  int min = fa[left] < fa[right] ? left : right;
  if (fa[index] < fa[min]) {
    RouteCursor &tmp = fa[index];
    fa[index] = fa[min];
    fa[min] = tmp;
    return min;
  }
  return index;
}
