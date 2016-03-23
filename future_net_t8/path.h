#ifndef __PATH_H__
#define __PATH_H__

#include "bitmap.h"
#include "data.h"

struct Path {
  int size;
  int pass_count;
  Edge *path;
  Bitmap *bitmap;
  Path *next;
};

#endif
