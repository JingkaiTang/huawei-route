#include "route_cursor.h"

#include <stdio.h>

bool operator> (RouteCursor a, RouteCursor b) {
  return a.value > b.value;
}
