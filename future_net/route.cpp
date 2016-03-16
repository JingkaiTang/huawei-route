
#include "route.h"
#include "lib_record.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRINT printf


//分割demand
int get_demand(int s,int d,int pass[50],char *demand);


//你要完成的功能总入口
void search_route(int *topo[5000], int edge_num, char *demand)
{
	int (*p)[600];
	p=(int (*)[600])malloc(360000*sizeof(int)); 
	//PRINT("%d %d %d %d\n",topo[0][0],topo[0][1],topo[0][2],topo[0][3]);
	register int i=0;
	for(i=0;i<edge_num;i++){
		p[topo[i][1]][topo[i][2]]=topo[i][3];
	}
	//PRINT("dmd=%s\n",demand);
	//源、目标点，点集
	int s =0,d=0,v_num=0;
	int pass[50];
	v_num = get_demand(s,d,pass,demand);
	PRINT("v_num=%d\n",v_num);
	
	

	/*
	for(int i=0;i<20;i++){
		for(int j=0;j<20;j++){
			PRINT("%2d ",p[i][j]);
		}
		PRINT(" \n");
	}
	*/


    unsigned short result[] = {2, 6, 3};//示例中的一个解

    for (int i = 0; i < 3; i++)
        record_result(result[i]);
}


//分割demand
int get_demand(int s,int d,int pass[50],char *demand)
{
	s = atoi(strtok(demand,","));
	d = atoi(strtok(NULL, ","));
	//PRINT("s=%d d=%d\n",s,d);
	register int i=0;
	char* tmp = strtok(strtok(NULL, ","), "|");
    while( tmp != NULL )
	{
		pass[i++]=atoi(tmp);
		//PRINT("%d\n", pass[i] );
		//i++;
		tmp = strtok( NULL, "|");
	}
	return i;
}