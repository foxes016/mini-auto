//*----------------------------------------------------------------------------
//* ���W			   : search.h
//* �\��y�z		   : ���s�d���Ʈw�Y���
//* �@��    		   : �s�@
//* ����			   : 1.0
//* �إߤ���B�ɶ�	   : 2004/07/17 10:21
//* �̪�ק����B�ɶ� : 
//* �ק��]		   : 
//*----------------------------------------------------------------------------
#ifndef	__search_h
#define	__search_h

//*---------------------------- Function Prototyping -------------------------
extern void *MemSearch(void *pvMem, INT32U u32MemSize, INT32U u32Data, INT32U u32DataSize);
extern INT8S *MemChr(INT8S *pbMem, INT8S bBYTE, INT32U u32MemSize);
extern BOOLEAN memstr(INT8S *pbMem, INT8S *pbStr, INT32U u32StrSize, INT32S s32MemSize);
extern INT8S *memstrExt(INT8S *pbMem, INT8S *pbStr, INT32U u32StrSize, INT32U u32MemSize);

#endif