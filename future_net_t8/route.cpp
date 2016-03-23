#include "route.h"
#include "lib_record.h"
#include "naive_debug.h"
#include "path.h"

#define BASE_STATE    0b00000000
#define FOUND         0b00000001
#define BAD_NODE      0b00000010
#define GOOD_NODE     0b00000100
#define DANGLING_NODE 0b00001000

TopoNode *topo = NULL;
DemandSet *demand = NULL;
Edge *path = NULL;
int path_size = 0;
Bitmap *path_bitmap = NULL;
Bitmap *good_nodes = NULL;
TopoNode *dangling = NULL;
Path *good = NULL;

void _add_dangling_edge(int node, TopoArrow *arrow);
void _add_dangling_edge(int node, TopoArrow *arrow);
int search_node(int pass_count, int node);
int search_edge(int pass_count, int node, TopoArrow *arrow);

void search_route(TopoNode *t, int node_scope, DemandSet *d) {
  topo = t;
  demand = d;
  path = new Edge[node_scope]();
  path_size = 0;
  path_bitmap = new Bitmap(node_scope);
  good = new Path[node_scope]();
  good_nodes = new Bitmap(node_scope);
  dangling = new TopoNode[node_scope]();

  topo[demand->end].out_degree = 0;

  LOG("DEMAND => ");
  BITMAP_SHOW(demand->bitmap);

  search_node(0, demand->start);

  for (int i = 0; i < path_size; i++) {
    record_result(path[i].arrow->number);
  }

  delete[] path;
  delete path_bitmap;
  delete good_nodes;
  delete good;
  delete dangling;
}

void _add_dangling_edge(int node, TopoArrow *arrow) {
  bool is_added = false;
  for (int i = 0; i < dangling[node].out_degree; i++) {
    if (dangling[node].arrows[i].number == arrow->number) {
      is_added = true;
      break;
    }
  }
  if (!is_added) {
    dangling[node].arrows[dangling[node].out_degree] = *arrow;
    dangling[node].out_degree++;
  }
}

void add_dangling_edge(int node, TopoArrow *arrow) {
  _add_dangling_edge(node, arrow);
  int index = 0;
  while (index < path_size && path[index].from != arrow->target) {
    index ++;
  }
  for (; index < path_size; index ++) {
    _add_dangling_edge(path[index].from, path[index].arrow);
  }
}

int search_node(int pass_count, int node) {
  int ret = BASE_STATE;

  path_bitmap->set(node);
  if (demand->bitmap->test(node)) {
    pass_count++;
  }

  if (pass_count == demand->pass_size && node == demand->end) {
    return FOUND;
  }

  TopoNode *cur_node = &topo[node];
  TopoArrow *cur_arrow;
  for (int i = 0; i < cur_node->out_degree; i++) {
    cur_arrow = &cur_node->arrows[i];

    int flag = search_edge(pass_count, node, cur_arrow);

    if (flag & FOUND) {
      return FOUND;
    }

    ret |= flag;

    if (flag & BAD_NODE) {
      if (i < cur_node->out_degree - 1) {
        cur_arrow[i] = cur_arrow[cur_node->out_degree - 1];
        i--;
      }
      cur_node->out_degree--;
    }
  }
  path_bitmap->unset(node);

  if (cur_node->out_degree == 0 && node != demand->end) {
    if (demand->bitmap->test(node)) {
      path_size = 0;
      return FOUND;
    }
    return BAD_NODE;
  }

  return ret;
}

int search_edge(int pass_count, int node, TopoArrow *arrow) {
  // dangling target node check
  if (path_bitmap->test(arrow->target)) {
    add_dangling_edge(node, arrow);
    return DANGLING_NODE;
  }

  int ret = BASE_STATE;

  // push edge to path
  path[path_size].from = node;
  path[path_size].arrow = arrow;
  path_size++;

  int flag = search_node(pass_count, arrow->target);

  if (flag & FOUND) {
    return FOUND;
  }

  ret |= flag;

  // remove edge from path
  path_size--;
  return ret;
}

