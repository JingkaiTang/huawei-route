#ifndef __BITMAP_H__
#define __BITMAP_H__

class Bitmap {
public:
  int size;
  int data_size;
  int *data;

  Bitmap(int size);
  void set(int location);
  void unset(int location);
  bool test(int location);
};

#endif
