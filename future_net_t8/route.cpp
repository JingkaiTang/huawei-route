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
Bitmap *bad_nodes = NULL;


bool search(int pass_count, int node) {
  path_bitmap->set(node);
  if (demand->bitmap->test(node)) {
    pass_count ++;
  }

  if (pass_count == demand->pass_size && node == demand->end) {
    return true;
  }

  if (!bad_nodes->test(node)) {
    TopoNode *cur_node = &topo[node];

    if (cur_node->out_degree == 0) {
      if (demand->bitmap->test(node)) {
        path_size = 0;
        return true;
      }
      bad_nodes->set(node);
    } else {

      TopoArrow *cur_arrow;
      for (int i = 0; i < cur_node->out_degree; i++) {
        cur_arrow = &cur_node->arrows[i];

        if (path_bitmap->test(cur_arrow->target)) {
          continue;
        }

        path[path_size] = cur_arrow->number;
        path_size++;

        if (search(pass_count, cur_arrow->target)) {
          return true;
        }

        path_size--;
      }
    }
  }

  path_bitmap->unset(node);
  return false;
}

void search_route(TopoNode *t, int node_scope, DemandSet *d) {
  topo = t;
  demand = d;
  path = new int[node_scope];
  path_size = 0;
  path_bitmap = new Bitmap(node_scope);
  bad_nodes = new Bitmap(node_scope);

  LOG("DEMAND => ");
  BITMAP_SHOW(demand->bitmap);

  search(0, demand->start);

  for (int i = 0; i < path_size; i ++) {
    record_result(path[i]);
  }

  delete[] path;
  delete path_bitmap;
  delete bad_nodes;
}
