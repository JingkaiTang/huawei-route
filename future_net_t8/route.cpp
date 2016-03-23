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
  end->path = new Edge[0];
  end->bitmap = new Bitmap(node_scope);
  end->bitmap->set(demand->end);
  end->next = NULL;
  good[demand->end] = end;

  LOG("DEMAND => ");
  BITMAP_SHOW(demand->bitmap);

  LOG("ROUTE SEARCH START ...\n");
  search_node(0, demand->start);

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

  bool is_visted = visited->test(node);

  path_bitmap->set(node);
  visited->set(node);
  if (demand->bitmap->test(node)) {
    pass_count++;
  }

  if (pass_count == demand->pass_size && node == demand->end) {
    LOG("PATH FOUND!!!\n");
    return FOUND;
  }

  if (node == demand->end) {
    path_bitmap->unset(node);
    return GOOD_NODE;
  }

  if (is_visted) {
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

  TopoNode *cur_node = &topo[node];
  TopoArrow *cur_arrow;
  for (int i = 0; i < cur_node->out_degree; i++) {
    cur_arrow = &cur_node->arrows[i];

    int flag = search_edge(pass_count, node, cur_arrow);

    if (flag & FOUND) {
      return FOUND;
    }

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

  if (flag & GOOD_NODE) {
    add_good_edge(node, arrow);
  }

  ret |= flag;

  // remove edge from path
  path_size--;
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
  int ret = BASE_STATE;
  return ret;
}

int search_dangling_node(int pass_count, int node) {
  int ret = BASE_STATE;
  return ret;
}