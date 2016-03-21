#ifndef __FAKE_ARRAY_H__
#define __FAKE_ARRAY_H__

#include <string.h>

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

template <class T>
FakeArray<T>::FakeArray(int sc): capacity(0), slot_capacity(sc), slots_size(0), slots_capacity(0) {
  extend();
}

template <class T>
FakeArray<T>::~FakeArray() {
}

template <class T>
T &FakeArray<T>::operator[] (int index) const {
  return slots[index/slot_capacity][index%slot_capacity];
}

template <class T>
int FakeArray<T>::extend() {
  if (slots_size < slots_capacity) {
    slots[slots_size] = new T[slot_capacity]();
    slots_size ++;
    capacity += slot_capacity;
    return capacity;
  } else {
    T **old_slots = slots;
    int old_slots_capacity = slots_capacity;
    slots_capacity += 10;
    slots = new T*[slots_capacity];
    memcpy(slots, old_slots, sizeof(T **)*old_slots_capacity);
    delete[] old_slots;
    return extend();
  }
}

#endif
