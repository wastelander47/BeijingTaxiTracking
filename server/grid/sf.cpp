int nodeselect(lbs_cell_t* cell, ){
	cell_id = cell->dammy_node->id
	//左下 
	if(cell_id == 0){
		int calculate(-1, -1, -1, -1, 1, -1, col_num+2, col_num+3,cell_id);
	}
	//右下 
	else if(cell_id == row_num){
		int calculate(-1, -1, -1, cell_id-1, -1, cell_id+col_num, cell_id+col_num+1, cell_id+col_num+2,cell_id);
	}
	//左上 
	else if(cell_id == (col_num+1)*row_num){
		int calculate(-1, cell_id-col_num-1, cell_id-col_num, -1, cell_id+1, -1, -1, -1,cell_id);
	}
	//右上 
	else if(cell_id == (col_num+1)*(row_num+1)-1){
		int calculate(cell_id-col_num-2, cell_id-col_num-1, -1, cell_id-1, -1, -1, -1, -1,cell_id);
	} 
	//最左列 
	else if(cell_id%(col_num+1 == 0)){
		int calculate(-1, cell_id-col_num-1, cell_id-col_num, -1, cell_id+1, -1, cell_id+col_num+1, cell_id+col_num+2,cell_id);
	}
	//最右列 
	else if((cell_id+1)%(col_num+1) == 0){
		int calculate(cell_id-col_num-2, cell_id-col_num-1, -1, cell_id-1, -1, cell_id+col_num, cell_id+col_num+1, -1,cell_id);
	} 
	//最下列 
	else if(0 < cell_id < col_num){
		int calculate(cell_id-col_num-2, cell_id-col_num-1, cell_id-col_num, cell_id-1, cell_id+1, -1, -1, -1,cell_id);
	} 
	//最上列 
	else if((col_num + 1)*row_num < cell_id < (col_num + 1)*(row_num + 1)-1){
		int calculate(-1, -1, -1, cell_id-1, cell_id+1, cell_id+col_num, cell_id+col_num+1, cell_id+col_num+2,cell_id);
	}
	else{
		int calculate(cell_id-col_num-2, cell_id-col_num-1, cell_id-col_num, cell_id-1, cell_id+1, cell_id+col_num, cell_id+col_num+1, cell_id+col_num+2,cell_id);
	} 
}

int calculate(int a, int b, int c, int d, int f, int g, int h, int i,int cell_id){
	int arr[8] = {a,b,c,d,f,g,h,i}
	if(arr[i]!=-1,i++){
		lbs_nnheap_insert(lbs_nnheap_t* lbs_nnheap, lbs_mov_node_t* lbs_mov_node, cell_id, uint8_t is_grid, double distance);
		
	}
}