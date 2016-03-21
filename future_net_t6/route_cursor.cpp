#include "route_cursor.h"

bool operator< (const RouteCursor &a, const RouteCursor &b) {
  return a.value < b.value;
}
