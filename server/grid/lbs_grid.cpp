#include "server/grid/lbs_grid.h"

int lbs_grid_init(lbs_grid_t* lbs_grid, double lon1, double lon2, double lat1, double lat2, int row_num, int col_num) {
	lbs_grid->lon_min = lon1;
	lbs_grid->lat_min = lat1;
	lbs_grid->cell_height = (lat2 - lat1) / row_num;
	lbs_grid->cell_width = (lon2 - lon1) / col_num;
	lbs_grid->row_num = row_num;
	lbs_grid->col_num = col_num;
	//init hashtable
	
	lbs_hashtable_init(&lbs_grid->hash_table);
	//malloc to cell 
	lbs_grid->cell = (lbs_cell_t*)malloc(sizeof(lbs_cell_t)*(row_num * col_num));
	//init head
    for(int i=0;i<row_num * col_num;i++)
    {
        lbs_queue_init(&(lbs_grid->cell[i].dammy_node.queue));
    }
	return 0;
};


int lbs_grid_update(lbs_grid_t* lbs_grid, double lon, double lat, uint64_t timestamp, uint32_t id) {
	lbs_hashnode_t* hash_nodes;
    int temp_row=lbs_grid_cell_row(lbs_grid,lat);
    int temp_col=lbs_grid_cell_col(lbs_grid,lon);
    int temp_cell_id=lbs_grid_cell_id(lbs_grid,temp_row,temp_col);
    hash_nodes = lbs_hashtable_get(&lbs_grid->hash_table, id);
    
    if(hash_nodes == NULL){
        lbs_mov_node_t *temp_node = (lbs_mov_node_t *)malloc(sizeof(lbs_mov_node_t));
        temp_node->id=id;
        temp_node->lon=lon;
        temp_node->lat=lat;
        temp_node->timestamp=timestamp;
        lbs_queue_insert_head(&(lbs_grid->cell[temp_cell_id].dammy_node.queue),&(temp_node->queue));
        lbs_hashtable_set(&(lbs_grid->hash_table),id,temp_node,temp_cell_id);
    }
    else{
        hash_nodes->mov_node->lat=lat;
        hash_nodes->mov_node->lon=lon;
        hash_nodes->mov_node->timestamp=timestamp;
        if(temp_cell_id==hash_nodes->cell_id){ 
            return 0;
            } 
            
        else{
            hash_nodes->cell_id=temp_cell_id;
            lbs_queue_remove(&(hash_nodes->mov_node->queue));
            lbs_queue_insert_head(&(lbs_grid->cell[temp_cell_id].dammy_node.queue),&(hash_nodes->mov_node->queue));
        }
    }
    
    return 0;
};

int lbs_grid_cell_row(lbs_grid_t* lbs_grid, double lat) {
	int cell_row = (lat - lbs_grid->lat_min) / lbs_grid->cell_height;
	return cell_row;
};

int lbs_grid_cell_col(lbs_grid_t* lbs_grid, double lon){
	int cell_col = (lon - lbs_grid->lon_min) / lbs_grid->cell_width;
	return cell_col;
};

int lbs_grid_cell_id(lbs_grid_t* lbs_grid, int cell_row, int cell_col) {
	int cell_id = lbs_grid->col_num * cell_row + cell_col;
	return cell_id;
};

void lbs_grid_cell_row_col(lbs_grid_t* lbs_grid, int cell_id, int* cell_row, int* cell_col) {
    *cell_row = cell_id / lbs_grid->col_num;
    *cell_col = cell_id % lbs_grid->col_num;
};

lbs_cell_t* lbs_grid_cell(lbs_grid_t* lbs_grid, int cell_id) {
	return lbs_grid->cell + cell_id;
};
