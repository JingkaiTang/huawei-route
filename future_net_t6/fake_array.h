#ifndef __FAKE_ARRAY_H__
#define __FAKE_ARRAY_H__

template <class T>
class FakeArray {
public:
  int capacity;
  int slot_capacity;
  int slots_size;
  int slots_capacity;
  T **slots;

  FakeArray(int sc);
  ~FakeArray();
  T &operator[] (int index) const;
  int extend();
};

#endif
