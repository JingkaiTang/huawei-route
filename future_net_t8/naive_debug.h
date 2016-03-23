#ifndef __NAIVE_DEBUG_H__
#define __NAIVE_DEBUG_H__

#define __DEBUG__

#include "route_cursor.h"
#include "data.h"
#include "bitmap.h"
#include "tricky_heap.h"
#include "path.h"

#include <stdio.h>

#ifdef __DEBUG__
void path_list_show(Path **path_list, int size);
#define PATH_LIST_SHOW(path_list, size) path_list_show(path_list, size)
#else
#define PATH_LIST_SHOW(path_list, size)
#endif

#ifdef __DEBUG__
void path_show(Path *path);
#define PATH_SHOW(path) path_show(path)
#else
#define PATH_SHOW(path)
#endif

#ifdef __DEBUG__
void edge_show(Edge *edge);
#define EDGE_SHOW(edge) edge_show(edge)
#else
#define EDGE_SHOW(edge)
#endif

#ifdef __DEBUG__
void edge_array_show(Edge *edges, int size);
#define EDGE_ARRAY_SHOW(edge, size) edge_array_show(edge, size)
#else
#define EDGE_ARRAY_SHOW(edge, size)
#endif

#ifdef __DEBUG__
void arrow_show(TopoArrow *arrow);
#define ARROW_SHOW(arrow) arrow_show(arrow)
#else
#define ARROW_SHOW(arrow)
#endif

#ifdef __DEBUG__
void heap_show(TrickyHeap *heap);
#define HEAP_SHOW(heap) heap_show(heap)
#else
#define HEAP_SHOW(heap)
#endif

#ifdef __DEBUG__
void bitmap_show(Bitmap *bitmap);
#define BITMAP_SHOW(bitmap) bitmap_show(bitmap)
#else
#define BITMAP_SHOW(bitmap)
#endif

#ifdef __DEBUG__
#define LOG(args...) printf(args)
#else
#define LOG(args...)
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
