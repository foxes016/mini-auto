#include "sysopt.h"
#if (NIC_SUPPORT && !ICOMMWIFI_SUPPORT)
#ifndef	__sys_arch_h__
#define	__sys_arch_h__
#include  "ucos_ii.h"

//�M�wNetwork layer���Queue���ƶq. modified by aher 2013/12/12
#define	MBOX_SIZE					20//20		//* ���w�l�c��������������ƶq
//aher 
#define	MBOX_NB						200//10		//* ���w�l�c�ӼơA�]�N�O������

#define	SYS_MBOX_NULL				(void *)0
#define	SYS_SEM_NULL				(void *)0
//*-------------------------------------- ���c�w�q ------------------------------------------------
/* LwIP�l�c���c */
typedef struct stLwIPMBox{
	struct stLwIPMBox 	*pstNext;
	OS_EVENT*			hMBox;
	void 				*pvaMsgs[MBOX_SIZE];
} ST_LWIP_MBOX, *PST_LWIP_MBOX;
//*------------------------------- �@�Ǧ۩w�q���ƾ����� -------------------------------------------
typedef OS_EVENT* 		sys_sem_t;
typedef PST_LWIP_MBOX 	sys_mbox_t;		//* LwIP�l�c
typedef u8_t			sys_thread_t;	//* LwIP�u�{ID

#endif

#endif //#if (NIC_SUPPORT && !ICOMMWIFI_SUPPORT)