#include "server/grid/lbs_nnheap.h"
#include <stdlib.h>
#define CAP 20

int lbs_nnheap_init(lbs_nnheap_t* lbs_nnheap){
	lbs_nnheap -> capacity = CAP;
	lbs_nnheap -> size = 0;
	lbs_nnheap -> heap_nodes = (lbs_heapnode_t*)malloc(lbs_nnheap->capacity * sizeof(lbs_heapnode_t));
	
}

int lbs_nnheap_insert(lbs_nnheap_t* lbs_nnheap, lbs_mov_node_t* lbs_mov_node, int cell_id, uint8_t is_grid, double distance){
	if(lbs_nnheap->size == lbs_nnheap->capacity){
		lbs_nnheap->capacity = lbs_nnheap->capacity * 2;
		lbs_nnheap -> heap_nodes = (lbs_heapnode_t*)realloc(lbs_nnheap->heap_nodes, lbs_nnheap ->capacity * sizeof(lbs_heapnode_t));
		 //return 0;
	}
	lbs_nnheap -> heap_nodes[lbs_nnheap -> size].cell_id = cell_id;
	lbs_nnheap -> heap_nodes[lbs_nnheap -> size].is_grid = is_grid;
	lbs_nnheap -> heap_nodes[lbs_nnheap -> size].distance = distance;
	lbs_nnheap -> heap_nodes[lbs_nnheap -> size].node = lbs_mov_node;
	lbs_nnheap ->size++;
	
	int child = lbs_nnheap -> size;
	while(child/2 >= 1){
		int parent = child/2;
		if(lbs_nnheap -> heap_nodes[parent-1].distance > lbs_nnheap -> heap_nodes[child-1].distance){
			lbs_heapnode_t a = lbs_nnheap -> heap_nodes[parent-1];
			lbs_nnheap -> heap_nodes[parent-1] = lbs_nnheap -> heap_nodes[child-1];
			lbs_nnheap -> heap_nodes[child-1] = a;
		}
		else{
			break;
		}
		child = parent;
	}
	return 0;
}

lbs_heapnode_t* lbs_nnheap_top(lbs_nnheap_t* lbs_nnheap){
	return &lbs_nnheap ->heap_nodes[0];
}

void lbs_nnheap_pop(lbs_nnheap_t* lbs_nnheap){
	int min = lbs_nnheap -> size;
	lbs_nnheap ->heap_nodes[0] = lbs_nnheap ->heap_nodes[min-1];
	lbs_nnheap -> size--;
	int parent = 1;
	int minchild;
	while(parent *2 <= lbs_nnheap -> size){
		int lchild = parent * 2;
		minchild = lchild;
		if(lchild + 1 <= lbs_nnheap -> size){
			int rchild = parent * 2 + 1; 
			if(lbs_nnheap -> heap_nodes[lchild-1].distance > lbs_nnheap -> heap_nodes[rchild-1].distance){
				 minchild = rchild;
			}
		}
		if(lbs_nnheap -> heap_nodes[parent-1].distance > lbs_nnheap -> heap_nodes[minchild-1].distance){
			lbs_heapnode_t a = lbs_nnheap -> heap_nodes[parent-1];
			lbs_nnheap -> heap_nodes[parent-1] = lbs_nnheap -> heap_nodes[minchild-1];
			lbs_nnheap -> heap_nodes[minchild-1] = a;
		}
		else{
			break;
		}
		parent = minchild;
	}
	return;
}
