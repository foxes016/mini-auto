//*----------------------------------------------------------------------------
//* ���W			   : converter.c
//* �\��y�z		   : �s�X��Ƥu��
//* �@��    		   : �s�@
//* ����			   : 1.0
//* �إߤ���B�ɶ�	   : 2005/06/24 10:21
//* �̪�ק����B�ɶ� : 
//* �ק��]		   : 
//*----------------------------------------------------------------------------
//*------------------------------ include file --------------------------------
#include	"ucos_ii.h"
#include	"converter.h"
//*--------------------- Constants & Variable definition ----------------------
//*----------------------------------------------------------------------------
//* ��ƦW�� : PowerCal
//* �\��y�z : ���B��
//* �J�f�Ѽ� : <s32Original>[in] �Q�����
//*			 :    <s32Power>[in] ���B�⦸��
//* �X�f�Ѽ� : �L
//*----------------------------------------------------------------------------
INT32S s32PowerCal(INT32S s32Original, INT32S s32Power)
{
	INT32S i, __s32RtnVal = 0;
	
	if(s32Power > 0)
	{
		__s32RtnVal = s32Original;
	
		for(i=1; i<s32Power; i++)
		{
			__s32RtnVal *= s32Original;
		}
	}	
	else
		__s32RtnVal = 1;
	
	return __s32RtnVal;
}
//*----------------------------------------------------------------------------
//* ��ƦW�� : ConvertArabNumToHexArray
//* �\��y�z : �ഫ���ԧB�Ʀr��16�i��
//* �J�f�Ѽ� : <pu8Dest>[out] ���V�����a�}�����w
//*			 :  <pszSrc>[in]  ���V�Q�ഫ���e�����w
//*			 :  <u32Len>[in]  �Q�ഫ���e������
//* �X�f�Ѽ� : �L
//*----------------------------------------------------------------------------
void vConvertArabNumToHexArray(INT8U *pu8Dest, const INT8S *pszSrc, INT32S s32Len)
{
	INT32S 	i;
	
	for(i=0; i<s32Len; i++)
	{
		*(pu8Dest+i) = *(pszSrc + i) - 0x30;
	}
}
//*----------------------------------------------------------------------------
//* ��ƦW�� : s32GetIntFromArray
//* �\��y�z : �q�@�Ӧr�`�Ʋ���o�㫬�ƾڡA��p{0x01, 0x02, 0x03}�ഫ���Q�i�123
//* �J�f�Ѽ� : <lpszSrc>[in] ���V���Ʋ�
//*			 :  <s32Len>[in] �Ʋժ���
//* �X�f�Ѽ� : ��^�㫬�ƾ�
//*----------------------------------------------------------------------------
INT32S s32GetIntFromArray(INT8S *ps8Src, INT32S s32Len)
{
	INT32S 	i, __s32RtnVal = 0, __s32Len = s32Len;
	
	for(i=0; i<s32Len; i++)
		__s32RtnVal += ((ps8Src[i] - 0x30) * s32PowerCal(10, --__s32Len));
		
	return __s32RtnVal;
}
//*----------------------------------------------------------------------------
//* ��ƦW�� : ConvertStringToOneByte_Hex
//* �\��y�z : �ഫ�@�Ӧr�Ŧ�]�p"255"�A���W�L�K����ס^���@��16�i��r�`
//* �J�f�Ѽ� : <pszSrc>[in] ���V�����e�����w
//*			 :  <s8Len>[in] �Q�ഫ���e������(�H�r�`�����)
//* �X�f�Ѽ� : ��^�@��8�쪺16�i���
//*----------------------------------------------------------------------------
INT8U u8ConvertStringToOneByte_Hex(INT8S *pszSrc, INT8S s8Len)
{
	INT8S		i, k;
	INT16U		__u16RtnVal = 0;
	
	if(s8Len > 4)
		return 0xFF;
	
	for(i=s8Len, k=0; i>0; i--, k++)
	{
		__u16RtnVal += ((*(pszSrc + k) - 0x30) * (s32PowerCal(10, i-1)));
		if(__u16RtnVal > 0xFF)
			return 0xFF;
	}
	
	return (INT8U)__u16RtnVal;
}
