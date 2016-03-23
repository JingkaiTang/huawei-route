#include "bitmap.h"

#include <string.h>

Bitmap::Bitmap(int _size) {
  size = _size;
  data_size = (size+sizeof(int)*8-1) / (sizeof(int)*8);
  data = new int[data_size]();
}

Bitmap::Bitmap(Bitmap *bitmap) {
  size = bitmap->size;
  data_size = bitmap->data_size;
  data = new int[data_size];
  memcpy(data, bitmap->data, sizeof(int)*data_size);
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

bool Bitmap::conflict(Bitmap *bitmap) {
  int min_data_size = data_size < bitmap->data_size ? data_size : bitmap->data_size;
  int accumulate = 0;
  for (int i = 0; i < min_data_size; i ++) {
    accumulate |= data[i] & bitmap->data[i];
  }
  return accumulate;
}

bool Bitmap::contain(Bitmap *bitmap) {
  int accumulate = 0;
  for (int i = 0; i < bitmap->data_size; i ++) {
    accumulate |= bitmap->data[i] - (data[i] & bitmap->data[i]);
  }
  return !accumulate;
}

Bitmap *Bitmap::join(Bitmap *bm1, Bitmap *bm2) {
  if (bm1->data_size != bm2->data_size) {
    return NULL;
  }
  Bitmap *bm = new Bitmap(bm1);
  for (int i = 0; i < bm->data_size; i ++) {
    bm->data[i] |= bm2->data[i];
  }
  return bm;
}
