#ifndef SERVER_GRID_LBS_BITMAP_H_
#define SERVER_GRID_LBS_BITMAP_H_

#include <stdint.h>
#include <stdlib.h>

typedef struct lbs_bitmap_s 
{
	uint8_t *bits;
	uint32_t bits_num;
}lbs_bitmap_t;

//��ʼ��bitmap
int lbs_bitmap_init(lbs_bitmap_t* lbs_bitmap, uint32_t bits_num);
//����bit
int lbs_bitmap_setbit(lbs_bitmap_t* lbs_bitmap, uint32_t pos);
//ȡ������
int lbs_bitmap_unsetbit(lbs_bitmap_t* lbs_bitmap, uint32_t pos);
//�ж��Ƿ�����
int lbs_bitmap_isset(lbs_bitmap_t* lbs_bitmap, uint32_t pos);

#endif  //SERVER_GRID_BITMAP_H_
