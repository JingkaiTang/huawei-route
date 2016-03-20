#include "bitmap.h"

Bitmap::Bitmap(int size):size(size) {
  data_size = (size+sizeof(int)*8-1) / (sizeof(int)*8);
  data = new int[data_size]();
}

void Bitmap::set(int location) {
  data[location/(sizeof(int)*8)] |= 0x1 << (location % (sizeof(int)*8));
}

void Bitmap::unset(int location) {
  data[location/(sizeof(int)*8)] &= (0x1 << (location % (sizeof(int)*8))) ^ -1;
}

bool Bitmap::test(int location) {
  return data[location/(sizeof(int)*8)] & (0x1 << (location % (sizeof(int)*8)));
}
