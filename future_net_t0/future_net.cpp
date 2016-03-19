#include "route.h"
#include "lib_io.h"
#include "lib_time.h"
#include "stdio.h"

#include "data_parse.h"

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
        printf("Please input valid topo file.\n");
        return -1;
    }

    char *demand_file = argv[2];
    demand_num = read_file(&demand_line, 1, demand_file);
    if (demand_num != 1)
    {
        printf("Please input valid demand file.\n");
        return -1;
    }

    TopoNode topo[600];
    int topo_max_index = topo_parse(topo, 600, topo_lines, edge_num);
    topo_show(topo, topo_max_index);

    DemandSet *demand = demand_parse(demand_line);
    demand_show(demand);

    search_route(topo, demand);

    char *result_file = argv[3];
    write_result(result_file);
    release_buff(topo_lines, edge_num);
    release_buff(&demand_line, 1);

    print_time("End");

    return 0;
}
