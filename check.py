#! /usr/bin/env python
# need python3

import sys
import os
import functools

TOPO_FILE = 'topo.csv'
DEMAND_FILE = 'demand.csv'
RESULT_FILE = 'result.csv'


def main(tf, df, rf):
    topo = get_topo(tf)
    demand = get_demand(df)
    result = get_result(rf)
    return check(topo, demand, result)


def get_topo(tf):
    topo = {}
    for line in open(tf):
        path = line.strip().split(',')
        topo[path[0]] = path
    return topo


def get_demand(df):
    d = open(df).read().strip().split(',')
    demand = {
        'Start': d[0],
        'Pass': d[2].split('|'),
        'End': d[1]
    }
    return demand


def get_result(rf):
    return open(rf).read().strip().split('|')


def check(topo, demand, result):
    NO_ANSWER = 'No Answer'
    INFINITY = 'Inf'
    WRONG_ANSWER = 'Wrong'

    if len(result) == 0:
        return NO_ANSWER

    if result[0] == 'NA':
        return INFINITY

    if topo[result[0]][1] != demand['Start'] or topo[result[-1]][2] != demand['End']:
        return WRONG_ANSWER
    cost = int(topo[result[0]][3])
    result = result[1:]

    for path in result:
        if topo[path][1] in demand['Pass']:
            demand['Pass'].remove(topo[path][1])
            cost += int(topo[path][3])

    if len(demand['Pass']) > 0:
        return WRONG_ANSWER

    return cost

if __name__ == '__main__':
    if len(sys.argv) == 3 and sys.argv[1] == '-d':
        work_dir = sys.argv[2]
        if not os.path.isdir(work_dir):
            print('Dir does not exist')
            sys.exit(2)
        topo_file, demand_file, result_file = map(lambda x: os.path.join(work_dir, x), [TOPO_FILE, DEMAND_FILE, RESULT_FILE])
    elif len(sys.argv) == 4:
        topo_file, demand_file, result_file = sys.argv[1:]
    else:
        print('Args Err')
        sys.exit(1)

    if not functools.reduce(lambda a, b: a and b, map(os.path.exists, [topo_file, demand_file, result_file])):
        print('File dose not exist')
        sys.exit(3)

    print(main(topo_file, demand_file, result_file))
