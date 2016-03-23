#ifndef __BITMAP_H__
#define __BITMAP_H__

class Bitmap {
public:
  int size;
  int data_size;
  int *data;

  Bitmap(int _size);
  Bitmap(Bitmap *bitmap);
  ~Bitmap();
  void set(int location);
  void unset(int location);
  bool test(int location);
  bool conflict(Bitmap *bitmap);
  bool contain(Bitmap *bitmap);
  static Bitmap *_and(Bitmap *bm1, Bitmap *bm2);
  static Bitmap *_or(Bitmap *bm1, Bitmap *bm2);
};

#endif
