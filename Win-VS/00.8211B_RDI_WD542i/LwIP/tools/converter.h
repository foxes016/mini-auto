//*-------------------------------------------------------------------------------------------------
//* ���W			   : converter.h
//* �\��y�z		   : �s�X��Ƥu��
//* �@��    		   : �s�@
//* ����			   : 1.0
//* �إߤ���B�ɶ�	   : 2005/06/24 10:21
//* �̪�ק����B�ɶ� : 
//* �ק��]		   : 
//*-------------------------------------------------------------------------------------------------
#ifndef	__converter_h
#define	__converter_h
//*---------------------------------------- ���c��Χ��w�q -----------------------------------------
#define	macHighToLowForWord(uwHex)		((uwHex & 0xFF) << 8) | ((uwHex & 0xFF00) >> 8)
//*-------------------------------------- ��ƭ쫬�n�� ---------------------------------------------
extern INT32S s32PowerCal(INT32S s32Original, INT32S s32Power);
extern void vConvertArabNumToHexArray(INT8U *pu8Dest, const INT8S *pszSrc, INT32S s32Len);
extern INT32S s32GetIntFromArray(INT8S *ps8Src, INT32S s32Len);
extern INT8U u8ConvertStringToOneByte_Hex(INT8S *pszSrc, INT8S s8Len);

#endif