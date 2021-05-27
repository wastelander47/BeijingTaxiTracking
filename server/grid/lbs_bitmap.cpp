#include "server/grid/lbs_bitmap.h"
#include "server/grid/lbs_grid.h"
#include <string.h>
//��ʼ��bitmap
int lbs_bitmap_init(lbs_bitmap_t* lbs_bitmap, uint32_t bits_num) //bitsλ.
{
	lbs_bitmap->bits_num = bits_num;
	int BYTES_num = bits_num / 8 + 1; // �ֽ�.
	lbs_bitmap->bits = (uint8_t *)malloc(BYTES_num * sizeof(uint8_t));
	memset(lbs_bitmap->bits,0,BYTES_num); // ��bits��0.
	return 0;
}
//����bit
int lbs_bitmap_setbit(lbs_bitmap_t* lbs_bitmap, uint32_t pos)
{
		int x = pos/8;	//�ֽ�λ��
		int	y = pos%8;	//λ��λ��	
		int a[]={128,64,32,16,8,4,2,1};//{0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01}
		lbs_bitmap->bits[x] = lbs_bitmap->bits[x] | a[y];
		return 0;
}
//ȡ������
int lbs_bitmap_unsetbit(lbs_bitmap_t* lbs_bitmap, uint32_t pos)
{
		int x = pos/8;	//�ֽ�λ��
		int	y = pos%8;	//λ��λ��	
		int a[]={127,191,223,239,247,251,253,254};
		lbs_bitmap->bits[x] = lbs_bitmap->bits[x] & a[y];
		return 0;
}
//�ж��Ƿ�����
int lbs_bitmap_isset(lbs_bitmap_t* lbs_bitmap, uint32_t pos)
{
		int x = pos/8;	//�ֽ�λ��
		int	y = pos%8;	//λ��λ��	
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
