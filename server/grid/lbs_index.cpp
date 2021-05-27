#include "server/grid/lbs_index.h"
#include <stdio.h>
#include <stdlib.h>
#include "common/log.h"
#include "server/grid/lbs_distance.h"
#include "server/grid/lbs_grid.h"
#include "server/grid/lbs_bitmap.h"
#include "server/grid/lbs_nnheap.h"

#define LBS_LON_MIN 116
#define LBS_LON_MAX 117
#define LBS_LAT_MIN 39
#define LBS_LAT_MAX 41

#define LBS_ROW_NUM 200
#define LBS_COL_NUM 100

static lbs_grid_t lbs_grid;
double getdistance(int *arr, int i,int lon, int lat);

// 初始化网格索引
int lbs_grid_index_init() {
  // TODO: by student

	 lbs_grid_init(&lbs_grid, LBS_LON_MIN, LBS_LON_MAX, LBS_LAT_MIN, LBS_LAT_MAX, LBS_ROW_NUM, LBS_COL_NUM);
  return 0;
}

// 更新接口[出租车位置更新]
int lbs_grid_index_update(double lon, double lat, uint64_t timestamp, uint32_t id) {
  // TODO: by student
	lbs_grid_update(&lbs_grid, lon, lat,  timestamp, id);
	  return 0;
}

// 范围查询接口[查询某一范围内的所有出租车信息]
int lbs_grid_index_range_query(double lon1, double lon2, double lat1, double lat2,lbs_res_node_t* out) {
  // TODO: by student
	int row1,row2,col1,col2;
	row1 = lbs_grid_cell_row(&lbs_grid,lat1);
	row2 = lbs_grid_cell_row(&lbs_grid,lat2);
	col1 = lbs_grid_cell_col(&lbs_grid,lon1);
	col2 = lbs_grid_cell_col(&lbs_grid,lon2);
        printf("row=%d %d col= %d %d\n", row1, row2, col1, col2);
	for (int row =row1; row<=row2; row++){
		for (int col= col1; col<=col2;col++){			
				int cell_id=lbs_grid_cell_id(&lbs_grid,row,col);
				lbs_cell_t* cell=lbs_grid_cell(&lbs_grid,cell_id);
				lbs_queue_t* head=&(cell->dammy_node.queue);
				lbs_queue_t* current=head->next;
			for (; current!=head;current=current->next){
				lbs_mov_node_t* mov_node=(lbs_mov_node_t*)current;
				if(mov_node->lon>=lon1 && mov_node->lon<=lon2&&
					mov_node->lat>=lat1 && mov_node->lat<=lat2){
						
						lbs_res_node_t* indexnode = (lbs_res_node_t *)malloc(sizeof(lbs_res_node_t));
						
						indexnode->lon=mov_node->lon;
						indexnode->lat=mov_node->lat;
						indexnode->id=mov_node->id;
						indexnode->timestamp=mov_node->timestamp;
						lbs_queue_insert_head(&(out->queue),&(indexnode->queue));
                                   
				}
			}
		}
		}
		
	
  return 0;
}

// NN查询接口[查询离lon,lat最近的出租车]
int lbs_grid_index_nn_query(double lon, double lat, lbs_res_node_t* out) {
	int row,col;
	lbs_nnheap_t lbs_nnheap;
	lbs_bitmap_t lbs_bitmap;

	row=lbs_grid_cell_row(&lbs_grid, lat);
	col=lbs_grid_cell_col(&lbs_grid,lon);

	int cellid = lbs_grid_cell_id(&lbs_grid,row, col);

	lbs_nnheap_init(&lbs_nnheap);

	lbs_nnheap_insert(&lbs_nnheap,&lbs_grid.cell[cellid].dammy_node,cellid,1, 0);

	lbs_heapnode_t* top= lbs_nnheap_top(&lbs_nnheap);

	lbs_bitmap_init(&lbs_bitmap, LBS_ROW_NUM*LBS_COL_NUM);
	lbs_bitmap_setbit(&lbs_bitmap, cellid);

	while(top->is_grid==1)
	{
		

		int top_cell_id = top->cell_id;
		lbs_mov_node_t* head=&lbs_grid.cell[top_cell_id].dammy_node;
		lbs_mov_node_t* current=(lbs_mov_node_t*)head->queue.next;
		lbs_nnheap_pop(&lbs_nnheap);

		while(current!=head)
		{
			double distance1=lbs_distance(lon, lat, current->lon , current->lat);
			lbs_nnheap_insert(&lbs_nnheap, current,top_cell_id,0, distance1);

			current = (lbs_mov_node_t*)current->queue.next;
		}

		int arr[8]={-1,-1,-1,-1,-1,-1,-1,-1};//store cellid
		int cellid0=top_cell_id;

		if(cellid0 % LBS_COL_NUM != 0 && cellid0 + LBS_COL_NUM < LBS_ROW_NUM*LBS_COL_NUM)//left up
		//if(cellid0 + LBS_COL_NUM-1 >= 0 && cellid0 + LBS_COL_NUM-1 < LBS_ROW_NUM*LBS_COL_NUM)
			arr[0]=cellid0 + LBS_COL_NUM-1;

		if(cellid0 + LBS_COL_NUM < LBS_ROW_NUM*LBS_COL_NUM)//up
			arr[1]=cellid0 + LBS_COL_NUM;

		if(cellid0 + LBS_COL_NUM < LBS_ROW_NUM*LBS_COL_NUM && cellid0 % LBS_COL_NUM != LBS_COL_NUM - 1)//right up
			arr[2]=cellid0 + LBS_COL_NUM+1;

		if(cellid0 % LBS_COL_NUM!=0)//left
			arr[3]=cellid0-1;

		if(cellid0 % LBS_COL_NUM!=LBS_COL_NUM-1)//right
			arr[4]=cellid0+1;

		if(cellid0 % LBS_COL_NUM!=0 && cellid0 -LBS_COL_NUM>=0)//left down
			arr[5]=cellid0 - LBS_COL_NUM -1;

		if(cellid0-LBS_COL_NUM>=0)//down
			arr[6]= cellid0 - LBS_COL_NUM;

		if(cellid0 - LBS_COL_NUM>=0 && cellid0 % LBS_COL_NUM!= LBS_COL_NUM - 1)//right down
			arr[7]= cellid0 - LBS_COL_NUM + 1;

		for(int i=0;i<8;i++)
		{
			if(arr[i] != -1)
				if(lbs_bitmap_isset(&lbs_bitmap,arr[i])==0)
				{
					double distance=getdistance(arr,i,lon,lat);

					lbs_nnheap_insert(&lbs_nnheap,&lbs_grid.cell[arr[i]].dammy_node ,arr[i],1, distance);
					lbs_bitmap_setbit(&lbs_bitmap, arr[i]);
				}
		}

		for(int i=0;i<8;i++)
			arr[i]=-1;

		top=(lbs_heapnode_t*)lbs_nnheap_top(&lbs_nnheap);

	}

	lbs_res_node_t* q=(lbs_res_node_t*)malloc(sizeof(lbs_res_node_t));

	q->id=top->node->id;
	q->lat=top->node->lat;
	q->lon=top->node->lon;
	q->timestamp=top->node->timestamp;

	lbs_queue_insert_head(&(out->queue), &(q->queue));

	return 0;
}


double getdistance(int *arr, int i,int lon, int lat)
{
	int row, col;
	lbs_grid_cell_row_col(&lbs_grid, arr[i], &row, &col);

	//car
	int row1=lbs_grid_cell_row(&lbs_grid, lat);
	int col1=lbs_grid_cell_col(&lbs_grid,lon);

	int lon2,lat2;

	if(row1<row&&col1>col)
	{
		lat2=row*(lbs_grid.cell_height);
		lon2=(col+1)*(lbs_grid.cell_width);
	}
	if(row1<row&&col1==col)
	{
		lat2=row*(lbs_grid.cell_height);
		lon2=lon;
	}
	if(row1>row&&col1<col)
	{
		lat2=row*(lbs_grid.cell_height);
		lon2=col*(lbs_grid.cell_width);
	}
	if(row1==row && col1>col)
	{
		lat2=lat;
		lon2=(col+1)*(lbs_grid.cell_width);
	}
	if(row1==row && col1<col)
	{
		lat2=lat;
		lon2=col*(lbs_grid.cell_width);
	}
	if(row1>row && col1 > col)
	{
		lat2=(row+1)*lbs_grid.cell_height;
		lon2=(col+1)*lbs_grid.cell_width;
	}
	if(row1 > row && col1==col)
	{
		lat2=(row+1)*lbs_grid.cell_height;
		lon2=lon;
	}
	if(row1 > row && col1 < col )
	{
		lat2=(row+1)*lbs_grid.cell_height;
		lon2=col*lbs_grid.cell_width;
	}

	return lbs_distance(lon, lat, lon2, lat2);
}
