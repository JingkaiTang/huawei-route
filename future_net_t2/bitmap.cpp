#include "bitmap.h"

#include <string.h>

Bitmap::Bitmap(int size):size(size) {
  data_size = (size+sizeof(int)*8-1) / (sizeof(int)*8);
  data = new int[data_size]();
}

Bitmap::Bitmap(Bitmap *bitmap) {
  size = bitmap->size;
  data_size = bitmap->data_size;
  data = new int[data_size];
  memcpy(data, bitmap->data, size);
}

Bitmap::~Bitmap() {
  delete[] data;
}

void Bitmap::set(int location) {
  if (location >= size) {
    return;
  }
  data[location/(sizeof(int)*8)] |= 0x1 << (location % (sizeof(int)*8));
}

void Bitmap::unset(int location) {
  if (location >= size) {
    return;
  }
  data[location/(sizeof(int)*8)] &= (0x1 << (location % (sizeof(int)*8))) ^ -1;
}

bool Bitmap::test(int location) {
  if (location >= size) {
    return false;
  }
  return data[location/(sizeof(int)*8)] & (0x1 << (location % (sizeof(int)*8)));
}
