#include "server/grid/lbs_bitmap.h"
#include "server/grid/lbs_grid.h"
#include <string.h>
//初始化bitmap
int lbs_bitmap_init(lbs_bitmap_t* lbs_bitmap, uint32_t bits_num) //bits位.
{
	lbs_bitmap->bits_num = bits_num;
	int BYTES_num = bits_num / 8 + 1; // 字节.
	lbs_bitmap->bits = (uint8_t *)malloc(BYTES_num * sizeof(uint8_t));
	memset(lbs_bitmap->bits,0,BYTES_num); // 将bits置0.
	return 0;
}
//设置bit
int lbs_bitmap_setbit(lbs_bitmap_t* lbs_bitmap, uint32_t pos)
{
		int x = pos/8;	//字节位置
		int	y = pos%8;	//位的位置	
		int a[]={128,64,32,16,8,4,2,1};//{0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01}
		lbs_bitmap->bits[x] = lbs_bitmap->bits[x] | a[y];
		return 0;
}
//取消设置
int lbs_bitmap_unsetbit(lbs_bitmap_t* lbs_bitmap, uint32_t pos)
{
		int x = pos/8;	//字节位置
		int	y = pos%8;	//位的位置	
		int a[]={127,191,223,239,247,251,253,254};
		lbs_bitmap->bits[x] = lbs_bitmap->bits[x] & a[y];
		return 0;
}
//判定是否设置
int lbs_bitmap_isset(lbs_bitmap_t* lbs_bitmap, uint32_t pos)
{
		int x = pos/8;	//字节位置
		int	y = pos%8;	//位的位置	
		int a[]={128,64,32,16,8,4,2,1};
		if (lbs_bitmap->bits[x] & a[y] == 0)
		{
			return 0;
		} 
		else
		{
			return 1;
		}
}
