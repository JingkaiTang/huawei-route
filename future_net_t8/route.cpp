#include "route.h"
#include "lib_record.h"
#include "naive_debug.h"

#include <stdio.h>
#include <string.h>

TopoNode *topo = NULL;
DemandSet *demand = NULL;
int *path = NULL;
int path_size = 0;
Bitmap *path_bitmap = NULL;


bool search(int pass_count, int node, int edge) {
  TopoNode *cur_node = &topo[node];
  TopoArrow *cur_arrow = &cur_node->arrows[edge];

  if (path_bitmap->test(cur_arrow->target)) {
    return false;
  }

  path[path_size] = cur_arrow->number;
  path_size ++;
  path_bitmap->set(cur_arrow->target);
  if (demand->bitmap->test(cur_arrow->target)) {
    pass_count ++;
  }

  if (pass_count == demand->pass_size && cur_arrow->target == demand->end) {
    return true;
  }

  for (int i = 0; i < topo[cur_arrow->target].out_degree; i ++) {
    bool find = search(pass_count, cur_arrow->target, i);
    if (find) {
      return true;
    }
  }

  path_size --;
  path_bitmap->unset(cur_arrow->target);
  return false;
}

void search_route(TopoNode *t, int node_scope, DemandSet *d) {
  topo = t;
  demand = d;
  path = new int[node_scope];
  path_size = 0;
  path_bitmap = new Bitmap(node_scope);

  LOG("DEMAND => ");
  BITMAP_SHOW(demand->bitmap);

  int start = demand->start;
  path_bitmap->set(start);
  for (int i = 0; i < topo[start].out_degree; i ++) {
    bool find = search(0, start, i);
    if (find) {
      break;
    }
  }

  for (int i = 0; i < path_size; i ++) {
    record_result(path[i]);
  }

}
