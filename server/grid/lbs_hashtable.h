#ifndef H_HASHTABLE_H_
#define H_HASHTABLE_H_

#include<pthread.h>
#include "server/grid/lbs_defs.h"
#include "server/grid/lbs_queue.h"
#include <stdlib.h>

typedef struct lbs_hashnode_s{
	lbs_queue_t queue; //链表
	lbs_mov_node_t* mov_node;//节点
	int cell_id;
} lbs_hashnode_t;

typedef struct lbs_hashtable_s{
	int capacity;//容量;
	lbs_hashnode_t* hash_nodes;//哈西-链地址
} lbs_hashtable_t;

int lbs_hashtable_init(lbs_hashtable_t* lbs_hashtable);
int lbs_hashtable_set(lbs_hashtable_t* lbs_hashtable, uint32_t id, lbs_mov_node_t* lbs_mov_node, int cell_id);
lbs_hashnode_t* lbs_hashtable_get(lbs_hashtable_t* lbs_hash_table, uint32_t id);

#endif
