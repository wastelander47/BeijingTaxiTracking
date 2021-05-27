
#include "server/grid/lbs_hashtable.h"
//初始化
int lbs_hashtable_init(lbs_hashtable_t* lbs_hashtable)
{
		 lbs_hashtable->capacity = 20000;
		 
		lbs_hashtable->hash_nodes=(lbs_hashnode_t*)malloc(lbs_hashtable->capacity * sizeof(lbs_hashnode_t));
		for(int i=0;i<lbs_hashtable->capacity;i++)
		{
			lbs_hashnode_t *head = lbs_hashtable->hash_nodes + i;
			lbs_queue_init(&(head->queue));
		}
		 return 0;
}
//设置
int lbs_hashtable_set(lbs_hashtable_t* lbs_hashtable, uint32_t id, lbs_mov_node_t* lbs_mov_node, int cell_id)
{
		int key=0;
		key=id % lbs_hashtable->capacity;  
		lbs_hashnode_t *head = lbs_hashtable->hash_nodes + key;
		lbs_hashnode_t *insertnode =(lbs_hashnode_t*)malloc(sizeof(lbs_hashnode_t));
		insertnode->cell_id = cell_id;
		insertnode->mov_node = lbs_mov_node;
		lbs_queue_insert_head(&(head->queue), &insertnode->queue);
		return 0;
}
//提取
lbs_hashnode_t* lbs_hashtable_get(lbs_hashtable_t* lbs_hash_table, uint32_t id)
{
		int key=0;
		key = id % lbs_hash_table->capacity;
		lbs_hashnode_t *head = lbs_hash_table->hash_nodes +key;

		lbs_hashnode_t *current=(lbs_hashnode_t*)head->queue.next;
		while(current!=head)
		{
			if (current->mov_node->id == id)
			{
				return current;
			} 
			else
			{
				current=(lbs_hashnode_t*)current->queue.next;
			}

		}
		return NULL;
}
