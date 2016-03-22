#include "route.h"
#include "lib_record.h"
#include "naive_debug.h"

#define FOUND 0
#define NOT_FOUND 1
#define BAD_NODE 2

TopoNode *topo = NULL;
DemandSet *demand = NULL;
int *path = NULL;
int path_size = 0;
Bitmap *path_bitmap = NULL;

int search(int pass_count, int node) {
  path_bitmap->set(node);
  if (demand->bitmap->test(node)) {
    pass_count ++;
  }

  if (pass_count == demand->pass_size && node == demand->end) {
    return FOUND;
  }

  TopoNode *cur_node = &topo[node];
  TopoArrow *cur_arrow;
  for (int i = 0; i < cur_node->out_degree; i++) {
    cur_arrow = &cur_node->arrows[i];

    if (path_bitmap->test(cur_arrow->target)) {
      continue;
    }

    path[path_size] = cur_arrow->number;
    path_size++;

    int flag = search(pass_count, cur_arrow->target);

    if (flag == FOUND) {
      return FOUND;
    }

    if (flag == BAD_NODE) {
      if (i < cur_node->out_degree-1) {
        cur_arrow[i] = cur_arrow[cur_node->out_degree-1];
        i--;
      }
      cur_node->out_degree --;
    }

    path_size--;
  }

  path_bitmap->unset(node);

  if (cur_node->out_degree == 0 && node != demand->end) {
    if (demand->bitmap->test(node)) {
      path_size = 0;
      return FOUND;
    }
    return BAD_NODE;
  }

  return NOT_FOUND;
}

void search_route(TopoNode *t, int node_scope, DemandSet *d) {
  topo = t;
  demand = d;
  path = new int[node_scope];
  path_size = 0;
  path_bitmap = new Bitmap(node_scope);

  LOG("DEMAND => ");
  BITMAP_SHOW(demand->bitmap);

  search(0, demand->start);

  for (int i = 0; i < path_size; i ++) {
    record_result(path[i]);
  }

  delete[] path;
  delete path_bitmap;
}
