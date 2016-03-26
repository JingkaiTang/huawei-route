#include "route.h"
#include "lib_io.h"
#include "lib_time.h"
#include "data_parse.h"
#include "naive_debug.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    print_time("Begin");
    char *topo_lines[5000];
    int edge_num;
    char *demand_line;
    int demand_num;

    char *topo_file = argv[1];
    edge_num = read_file(topo_lines, 5000, topo_file);
    if (edge_num == 0)
    {
        fprintf(stderr, "Please input valid topo file.\n");
        return -1;
    }

    char *demand_file = argv[2];
    demand_num = read_file(&demand_line, 1, demand_file);
    if (demand_num != 1)
    {
        fprintf(stderr, "Please input valid demand file.\n");
        return -1;
    }

    TopoNode topo[600];
    int topo_max_index = topo_parse(topo, 600, topo_lines, edge_num);
    TOPO_SHOW(topo, topo_max_index+1);

    DemandSet *demand = demand_parse(demand_line);
    DEMAND_SHOW(demand);

    search_route(topo, topo_max_index+1, demand);

    char *result_file = argv[3];
    write_result(result_file);
    release_buff(topo_lines, edge_num);
    release_buff(&demand_line, 1);

    print_time("End");

    return 0;
}
