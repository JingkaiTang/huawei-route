#include "route.h"
#include "lib_record.h"
#include "data.h"
#include "route_cursor.h"

#include <stdio.h>
#include <queue>

using namespace std;

#define ALPHA 1
#define BETA 100

void search_route(TopoNode *topo, DemandSet *demand) {
  priority_queue<RouteCursor, vector<RouteCursor>, greater<RouteCursor> > explorer;

  // start:
  RouteCursor *start = new RouteCursor();
  start->cur_node = demand->start;
  start->path.push_back(demand->start);
  start->cost = 0;
  start->pass_count = 0;
  start->value = 0;

  explorer.push(*start);

  RouteCursor *result = NULL;
  RouteCursor *cursor = NULL;
  RouteCursor *new_cursor = NULL;
  TopoNode *cur_node = NULL;
  TopoArrow *cur_arrow = NULL;
  bool conflict = false;
  while (!explorer.empty()) {
    RouteCursor tmp_cursor = explorer.top();
    cursor = &tmp_cursor;
    explorer.pop();
    if (cursor->cur_node == demand->end) {
      result = cursor;
      break;
    }

    cur_node = &topo[cursor->cur_node];
    for (int i = 0; i < cur_node->out_degree; i ++) {
      cur_arrow = &(cur_node->arrows[i]);
      conflict = false;

      for (vector<int>::iterator it = cursor->path.begin(); it != cursor->path.end(); it ++) {
        if (*it == cur_arrow->target) {
          conflict = true;
        }
      }

      if (conflict) {
        continue;
      }

      new_cursor = new RouteCursor();
      new_cursor->cur_node = cur_arrow->target;
      new_cursor->cost = cursor->cost + cur_arrow->cost;
      new_cursor->pass_count = cursor->pass_count;
      for (int j = 0; j < demand->pass_size; j ++) {
        if (demand->pass[i] == cur_arrow->target) {
          new_cursor->pass_count ++;
        }
      }
      new_cursor->path = cursor->path;
      new_cursor->path.push_back(cur_arrow->target);
      new_cursor->value = (ALPHA * new_cursor->cost + BETA * demand->pass_size) / (new_cursor->pass_count + 1);

      // test if find
      if (new_cursor->pass_count == demand->pass_size && new_cursor->cur_node == demand->end) {
        result = new_cursor;
        break;
      }

      explorer.push(*new_cursor);
    }
  }

  if (result != NULL && !result->path.empty()) {
    vector<int>::iterator it = result->path.begin();
    int pre_node = *it;
    int cur_node = 0;
    for (it ++; it != result->path.end(); it ++) {
      cur_node = *it;
      for (int i = 0; i < topo[pre_node].out_degree; i ++) {
        if (topo[pre_node].arrows[i].target == cur_node) {
          record_result(topo[pre_node].arrows[i].number);
        }
      }
      pre_node = cur_node;
    }
  }
}
