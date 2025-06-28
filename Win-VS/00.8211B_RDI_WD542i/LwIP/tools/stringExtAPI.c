//*----------------------------------------------------------------------------
//* ���W			   : stringExtAPI.c
//* �\��y�z		   : �r�Ŧ��X�i��Ʈw
//* �@��    		   : �s�@
//* ����			   : 1.0
//* �إߤ���B�ɶ�	   : 2006/01/10 12:03
//* �̪�ק����B�ɶ� : 
//* �ק��]		   : 
//*----------------------------------------------------------------------------
//*------------------------------ include file --------------------------------
#include	"ucos_ii.h"
#include	"stringExtAPI.h"
//*----------------------------------------------------------------------------
//* ��ƦW�� : strcatExt
//* �\��y�z : strcat�зǮw��ƪ��X�R���
//* �J�f�Ѽ� :     <pbDest>[out] ���V�������s�����w
//*			 : <u32DestLen>[in]  ���F�קK���s���|�A�Ө�ƻݭn�b�l�[���e�P�_��
//*			 :		     		 ���w�İϪ�����
//* 		 :      <pbSrc>[in]  ���V�Q�l�[���e�����w
//*			 :  <u32SrcLen>[in]  �Q�l�[���e������
//* �X�f�Ѽ� : �Q�l�[���̫��m
//*----------------------------------------------------------------------------
INT8S *strcatExt(INT8S *pbDest, INT32U u32DestLen, const INT8S *pbSrc, INT32U u32SrcLen)
{
	INT8S		*__pbStartAddr;
	INT8S		*__pbEndAddr;
	INT32S		i;

	//* �������Q�l�[���}�l�a�}�A�p�G�����a�}�w�g��F�����w�İϪ������h���A�l�[
	__pbStartAddr = pbDest + strlenExt(pbDest);
	__pbEndAddr = pbDest + u32DestLen;	
	i = 0;
	while((__pbStartAddr + i) < __pbEndAddr && i < u32SrcLen)
	{
		*(__pbStartAddr + i) = *(pbSrc + i);		
		i++;
	}
	
	return pbDest;
}
//*----------------------------------------------------------------------------
//* ��ƦW�� : strlenExt
//* �\��y�z : strlenExt�зǮw��ƪ��X�R���
//* �J�f�Ѽ� : <pbSrc>[in] ���V�Q�p�⪺�r�Ŧꪺ���w
//* �X�f�Ѽ� : �r�Ŧ����
//*----------------------------------------------------------------------------
INT32S strlenExt(const INT8S *pbSrc)
{
	INT32S 		i = 0;
	const INT8S	*__pbNext = pbSrc;
	
	while(*(__pbNext+i) != 0x00)
		i++;
		
	return i;
}
