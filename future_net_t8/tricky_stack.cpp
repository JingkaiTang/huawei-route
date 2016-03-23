#include "tricky_stack.h"

TrickyStack::TrickyStack(int cap): top(0) {
  stack = new int[cap+1];
  stack[0] = cap;
}

TrickyStack::~TrickyStack() {
  delete[] stack;
}

void TrickyStack::push(int elem) {
  top ++;
  stack[top] = elem;
}

int TrickyStack::pop(int elem) {
  if (elem == stack[top]) {
    top --;
  }
  return top;
}
