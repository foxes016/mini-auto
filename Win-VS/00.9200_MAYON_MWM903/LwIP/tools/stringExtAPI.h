//*----------------------------------------------------------------------------
//* ���W			   : stringExtAPI.c
//* �\��y�z		   : �r�Ŧ��X�i��Ʈw
//* �@��    		   : �s�@
//* ����			   : 1.0
//* �إߤ���B�ɶ�	   : 2006/01/10 12:03
//* �̪�ק����B�ɶ� : 
//* �ק��]		   : 
//*----------------------------------------------------------------------------
#ifndef	stringExtAPI_h
#define	stringExtAPI_h

//*------------------------------ include file -------------------------------

//*----------------------------- Constants definition ------------------------

//*-------------------- Structure & Variable definition ----------------------

//*---------------------------- Function Prototyping -------------------------
extern INT8S *strcatExt(INT8S *pbDest, INT32U u32DestLen, const INT8S *pbSrc, INT32U u32SrcLen);
extern INT32S strlenExt(const INT8S *pbSrc);

#endif