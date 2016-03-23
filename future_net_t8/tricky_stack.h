#ifndef __TRICKY_STACK_H__
#define __TRICKY_STACK_H__

class TrickyStack {
public:
  int *stack;
  int top;
  TrickyStack(int cap);
  ~TrickyStack();
  void push(int elem);
  int pop(int elem);
};

#endif
