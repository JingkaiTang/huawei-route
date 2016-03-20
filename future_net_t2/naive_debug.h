#ifndef __NAIVE_DEBUG_H__
#define __NAIVE_DEBUG_H__

// #define __DEBUG__

#include "route_cursor.h"
#include "data.h"

#include <stdio.h>

#ifdef __DEBUG__
#define LOG(fmt, args...) printf(fmt, ##args)
#else
#define LOG(fmt, args...)
#endif

#ifdef __DEBUG__
void cursor_show(RouteCursor *cursor);
#define CURSOR_SHOW(cursor) cursor_show(cursor)
#else
#define CURSOR_SHOW(cursor)
#endif

#ifdef __DEBUG__
void topo_show(TopoNode *topo, int topo_size);
#define TOPO_SHOW(topo, topo_size) topo_show(topo, topo_size)
#else
#define TOPO_SHOW(topo, topo_size)
#endif

#ifdef __DEBUG__
void demand_show(DemandSet *demand);
#define DEMAND_SHOW(demand) demand_show(demand)
#else
#define DEMAND_SHOW(demand)
#endif

#endif
