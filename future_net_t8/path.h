#ifndef __PATH_H__
#define __PATH_H__

#include "bitmap.h"

struct Path {
  int size;
  int *path;
  Bitmap *bitmap;
  Path *next;
};

#endif
