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
int node_scope = 0;
DemandSet *demand = NULL;
Edge *path = NULL;
int path_size = 0;
Bitmap *path_bitmap = NULL;
TopoNode *dangling = NULL;
Path **good = NULL;
Bitmap *visited = NULL;

void add_dangling_edge(int node, TopoArrow *arrow);

void add_good_edge(int node, TopoArrow *arrow);

int search_node(int pass_count, int node);

int search_edge(int pass_count, int node, TopoArrow *arrow);

int search_good_node(int pass_count, int node);

int search_dangling_node(int pass_count, int node);

void search_route(TopoNode *t, int n, DemandSet *d) {
  topo = t;
  node_scope = n;
  demand = d;
  path = new Edge[node_scope]();
  path_size = 0;
  path_bitmap = new Bitmap(node_scope);
  good = new Path *[node_scope]();
  dangling = new TopoNode[node_scope]();
  visited = new Bitmap(node_scope);

  topo[demand->end].out_degree = 0;

  Path *end = new Path();
  end->size = 0;
  end->pass_count = 0;
  end->path = new Edge[1]();
  end->bitmap = new Bitmap(node_scope);
  end->bitmap->set(demand->end);
  end->next = NULL;
  good[demand->end] = end;

  LOG("DEMAND => ");
  BITMAP_SHOW(demand->bitmap);

  LOG("ROUTE SEARCH START ...\n");
  search_node(0, demand->start);
  LOG("ROUTE SEARCH END ...\n");

  for (int i = 0; i < path_size; i++) {
    record_result(path[i].arrow->number);
  }

  /**
  delete[] path;
  delete path_bitmap;
  delete good_nodes;
  delete good;
  delete dangling;
  **/
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
    index++;
  }
  for (; index < path_size; index++) {
    _add_dangling_edge(path[index].from, path[index].arrow);
  }
}

int search_node(int pass_count, int node) {
  LOG("SEARCH NODE %d\n", node);
  int ret = BASE_STATE;

  if (visited->test(node)) {
    // good node
    if (good[node] != NULL) {
      ret |= search_good_node(pass_count, node);
    }

    if (ret & FOUND) {
      return FOUND;
    }

    // dangling node
    if (dangling[node].out_degree > 0) {
      ret |= search_dangling_node(pass_count, node);
    }

    return ret;
  }


  path_bitmap->set(node);
  if (demand->bitmap->test(node)) {
    pass_count++;
  }

  if (pass_count == demand->pass_size && node == demand->end) {
    LOG("PATH FOUND!!!\n");
    return FOUND;
  }

  if (node == demand->end) {
    path_bitmap->unset(node);
    visited->set(node);
    return GOOD_NODE;
  }

  TopoNode *cur_node = &topo[node];
  TopoArrow *cur_arrow;
  bool is_bad_node = true;
  for (int i = 0; i < cur_node->out_degree; i++) {
    LOG("BEGIN TO EXTEND EDGE[%d]\n", i);
    cur_arrow = &cur_node->arrows[i];

    ret |= search_edge(pass_count, node, cur_arrow);

    if (ret & FOUND) {
      return FOUND;
    }

    if (ret & (BAD_NODE ^ -1)) {
      is_bad_node = false;
    }
  }

  path_bitmap->unset(node);
  visited->set(node);

  if (is_bad_node) {
    LOG("BAD NODE %d FOUND!!!\n", node);
    if (demand->bitmap->test(node)) {
      path_size = 0;
      return FOUND;
    }
    return BAD_NODE;
  }

  ret &= BAD_NODE ^ -1;

  return ret;
}

int search_edge(int pass_count, int node, TopoArrow *arrow) {
  LOG("SEACHING EDGE:\n");
  ARROW_SHOW(arrow);
  // dangling target node check
  if (path_bitmap->test(arrow->target)) {
    LOG("DANGLING EDGE FOUND!!!\n");
    add_dangling_edge(node, arrow);
    TOPO_SHOW(dangling, node_scope);
    return DANGLING_NODE;
  }

  int ret = BASE_STATE;

  // push edge to path
  LOG("ADD node %d to path\n", node);
  path[path_size].from = node;
  path[path_size].arrow = arrow;
  path_size++;
  EDGE_ARRAY_SHOW(path, path_size);

  ret |= search_node(pass_count, arrow->target);

  if (ret & FOUND) {
    return FOUND;
  }

  if (ret & GOOD_NODE) {
    LOG("ADDING GOOD NODE ...\n");
    add_good_edge(node, arrow);
    PATH_LIST_SHOW(good, node_scope);
  }

  // remove edge from path
  LOG("REMOVE node %d from path\n", node);
  path_size--;
  EDGE_ARRAY_SHOW(path, path_size);
  return ret;
}

void _add_good_edge(int node, Path *path) {
  Path *tmp = good[node];
  good[node] = path;
  path->next = tmp;
}

void add_good_edge(int node, TopoArrow *arrow) {
  Path *next_path = good[arrow->target];
  Path *good_path = NULL;
  while (next_path != NULL) {
    good_path = new Path();
    good_path->size = next_path->size + 1;
    good_path->pass_count = next_path->pass_count;
    if (demand->bitmap->test(node)) {
      good_path->pass_count ++;
    }
    good_path->path = new Edge[good_path->size];
    good_path->path[0].from = node;
    good_path->path[0].arrow = arrow;
    memcpy(&good_path->path[1], next_path->path, sizeof(Edge) * next_path->size);
    good_path->bitmap = new Bitmap(next_path->bitmap);
    good_path->bitmap->set(node);
    _add_good_edge(node, good_path);
    next_path = next_path->next;
  }
}

int search_good_node(int pass_count, int node) {
  LOG("SEARCH GOOD NODE %d\n", node);
  Path *good_path = good[node];
  while (good_path != NULL) {
    if (!path_bitmap->conflict(good_path->bitmap) && pass_count+good_path->pass_count==demand->pass_size) {
      LOG("ROUTE FOUND IN GOOD NODES!!!\n");
      memcpy(&path[path_size], good_path->path, sizeof(Edge)*good_path->size);
      path_size += good_path->size;
      return FOUND;
    }
    good_path = good_path->next;
  }
  return GOOD_NODE;
}

int search_dangling_node(int pass_count, int node) {
  int ret = BASE_STATE;

  if (path_bitmap->test(node)) {
    return ret;
  }

  path_bitmap->set(node);
  if (demand->bitmap->test(node)) {
    pass_count ++;
  }
  TopoNode *cur_node = &dangling[node];
  TopoArrow *cur_arrow;
  for (int i = 0; i < cur_node->out_degree; i++) {
    cur_arrow = &cur_node->arrows[i];
    path[path_size].from = node;
    path[path_size].arrow = cur_arrow;
    path_size ++;

    if (good[cur_arrow->target] != NULL) {
      ret |= search_good_node(pass_count, cur_arrow->target);

      if (ret & FOUND) {
        return FOUND;
      }
    }

    ret |= search_dangling_node(pass_count, cur_arrow->target);

    if (ret & FOUND) {
      return FOUND;
    }

    path_size --;
  }
  path_bitmap->unset(node);
  return ret;
}