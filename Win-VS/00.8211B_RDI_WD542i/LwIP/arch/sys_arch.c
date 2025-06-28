#include "sysopt.h"
#if (NIC_SUPPORT && !ICOMMWIFI_SUPPORT)

//#include    "INCLUDES.H"
#include	"ucos_ii.h"
#include    "task.h"
#include	"./lwip/lwipsys.h"
#include	"./lwip/def.h"
#include	"./arch/cc.h"
#include	"./arch/sys_arch.h" 
//*------------------------------------- �`�q�B�ܶq�B�� -------------------------------------------
static ST_LWIP_MBOX __staLwIPMBoxs[MBOX_NB];
PST_LWIP_MBOX pstCurFreeMBox;  //�O�s�l�c�s�����a�}�C

//* LwIP�u�{�ϥΪ����
OS_STK T_LWIP_THREAD_STK[T_LWIP_THREAD_MAX_NB][T_LWIP_THREAD_STKSIZE];

//* sys_timeouts�ƲաA�Ω�O�stimeouts������a�}
static struct sys_timeouts __staSysTimeouts[T_LWIP_THREAD_MAX_NB + 1];
static struct sys_timeouts timeouts_null;
//*================================================================================================
//*�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��@�ơ@��
//*================================================================================================
//*------------------------------------------------------------------------------------------------
//* ��ƦW�� : sys_sem_new
//* �\��y�z : �إߨê�^�@�ӷs���H���q
//* �J�f�Ѽ� : <count>[in] ���w�H���q����l���A
//* �X�f�Ѽ� : ��^�s���H���q
//*------------------------------------------------------------------------------------------------
sys_sem_t sys_sem_new(u8_t count)
{
	return OSSemCreate(count);
}
//*------------------------------------------------------------------------------------------------
//* ��ƦW�� : sys_sem_signal
//* �\��y�z : �o�e�H��
//* �J�f�Ѽ� : <sem>[in] sem���w�n�o�e���H��
//* �X�f�Ѽ� : �L
//*------------------------------------------------------------------------------------------------
void sys_sem_signal(sys_sem_t sem)
{
	OSSemPost(sem);
}
//*------------------------------------------------------------------------------------------------
//* ��ƦW�� : sys_sem_free
//* �\��y�z : ����H���q
//* �J�f�Ѽ� : <sem>[in] ���w�n���񪺫H���q
//* �X�f�Ѽ� : �L
//*------------------------------------------------------------------------------------------------
void sys_sem_free(sys_sem_t sem)
{
	INT8U	__u8Err;
	
	while(NULL != OSSemDel(sem, OS_DEL_ALWAYS, &__u8Err))
		OSTimeDlyHMSM(0, 0, 0, 100);
}
//*------------------------------------------------------------------------------------------------
//* ��ƦW�� : __MSToOSTicks
//* �\��y�z : �N�@�����ܦ������`��
//* �J�f�Ѽ� : <u16MS>[in] �`���
//* �X�f�Ѽ� : �@���
//*------------------------------------------------------------------------------------------------
static INT16U __MSToOSTicks(INT16U u16MS)
{
	INT16U	__u16DelayTicks;
	
	if(u16MS != 0)
	{
		__u16DelayTicks = (u16MS * OS_TICKS_PER_SEC)/1000;
		if(__u16DelayTicks < 1)
			__u16DelayTicks = 1;
		else if(__u16DelayTicks > 65535)
			__u16DelayTicks = 65535;
		else;
	}
	else
		__u16DelayTicks = 0;		
		
	return __u16DelayTicks;
}
//*------------------------------------------------------------------------------------------------
//* ��ƦW�� : sys_arch_sem_wait
//* �\��y�z : ���ݥѰѼ�sem���w���H���ê���u�{
//* �J�f�Ѽ� :     <sem>[in] sem���w�n�o�e���H��
//*          : <timeout>[in] ���w���ݪ��̪��ɶ��]��쬰�@��^�C��0�A�u�{�|�@���Q���몽�ܦ�����w���H���F�D0�A���w�u
//*          :               �{�̪����ݮɶ�
//* �X�f�Ѽ� : -                0: �b���w�ɶ���������w�H��
//*          : - SYS_ARCH_TIMEOUT: �b���w�ɶ����S��������w�H��
//*------------------------------------------------------------------------------------------------
u32_t sys_arch_sem_wait(sys_sem_t sem, u32_t timeout)
{	
	INT8U	__u8RtnVal;	
		
	OSSemPend(sem, __MSToOSTicks(timeout), &__u8RtnVal);

	if(__u8RtnVal == OS_NO_ERR)
		return 0;
	else
		return SYS_ARCH_TIMEOUT;
}
//*------------------------------------------------------------------------------------------------
//* ��ƦW�� : sys_init
//* �\��y�z : �ھ�sys_arch.txt���إߡA�\�ର��l��sys_arch�h
//* �J�f�Ѽ� : �L
//* �X�f�Ѽ� : �L
//*------------------------------------------------------------------------------------------------
void sys_init(void)
{
	INT8U i; 
	
	//* ����ƲղM0
	memset(__staLwIPMBoxs, 0, sizeof(__staLwIPMBoxs));
	
	//* �إ����M�l�c
	for(i=0; i<(MBOX_NB-1); i++)
	{
		//* ��Ʋդ����U�����챵�b�@�_
		__staLwIPMBoxs[i].pstNext = &__staLwIPMBoxs[i+1];
				
		//* �إ߶l�c�A�t�Υ����O�Ҷl�c������Q�إߡA�p�G�X�{���~�A���O�{��BUG�A���Ӧb�ոն��q�ư�
		__staLwIPMBoxs[i].hMBox = OSQCreate(__staLwIPMBoxs[i].pvaMsgs, MBOX_SIZE);
	} 
	
	//* �O�ѤF�Ʋդ��̫�@�Ӥ����A���٨S���إ߶l�c�O
	__staLwIPMBoxs[MBOX_NB-1].hMBox = OSQCreate(__staLwIPMBoxs[MBOX_NB-1].pvaMsgs, MBOX_SIZE);
	
	//* �O�s����a�}
	pstCurFreeMBox = __staLwIPMBoxs;
	
	//* ��l��sys_timeouts�ƲաA�N�C�ӼƲզ����O�s�����a�}�]�m��NULL
	for(i=0; i<(T_LWIP_THREAD_MAX_NB + 1); i++)
		__staSysTimeouts[i].next = NULL;
}
//*------------------------------------------------------------------------------------------------
//* ��ƦW�� : sys_mbox_new
//* �\��y�z : �إߤ@�ӪŪ��l�c
//* �J�f�Ѽ� : �L
//* �X�f�Ѽ� : - != SYS_MBOX_NULL : �l�c�ӽЦ��\�A��^�@�ӫ��V�Q�ӽжl�c�����w
//*          : - = SYS_MBOX_NULL  : �l�c�S���ӽЦ��\
//*------------------------------------------------------------------------------------------------
sys_mbox_t sys_mbox_new(void)
{
	PST_LWIP_MBOX	__pstMBox = SYS_MBOX_NULL;
	
#if OS_CRITICAL_METHOD == 3                     
    OS_CPU_SR  		cpu_sr = 0;
#endif	
	
	OS_ENTER_CRITICAL();
	//{
		if(pstCurFreeMBox != NULL)
		{
			__pstMBox = pstCurFreeMBox;
			pstCurFreeMBox = __pstMBox->pstNext;
		}
	//}
	OS_EXIT_CRITICAL();
	
	return __pstMBox;
}
//*------------------------------------------------------------------------------------------------
//* ��ƦW�� : sys_mbox_free
//* �\��y�z : ����l�c�A�N�l�c�k�ٵ����
//* �J�f�Ѽ� : <mbox>[in] �n�k�٪��l�c
//* �X�f�Ѽ� : �L
//*------------------------------------------------------------------------------------------------
void sys_mbox_free(sys_mbox_t mbox)
{	
#if OS_CRITICAL_METHOD == 3                     
    OS_CPU_SR  		cpu_sr = 0;
#endif	
	
	//* ���F����N�~���p�o�͡A�A�D�ʲM�Ť@���l�c
	OSQFlush(mbox->hMBox);

	OS_ENTER_CRITICAL();
	//{
		mbox->pstNext = pstCurFreeMBox;
		pstCurFreeMBox = mbox;
	//}
	OS_EXIT_CRITICAL();
}
//*------------------------------------------------------------------------------------------------
//* ��ƦW�� : sys_mbox_post
//* �\��y�z : �N�����뻼����w���l�c
//* �J�f�Ѽ� : <mbox>[in] ���w�n�뻼���l�c
//*          :  <msg>[in] ���w�n�뻼������
//* �X�f�Ѽ� : �L
//*------------------------------------------------------------------------------------------------
void sys_mbox_post(sys_mbox_t mbox, void *msg)
{
	int Q_FULL=0;
	int i;
	
	i=0;
	while(OSQPost(mbox->hMBox, msg) == OS_Q_FULL)
	{
		if(Q_FULL==0)
		{	
			i++;
			printf("MSG:OS_Q_FULL-%d.\n",i);
			/*Free mbox*/
			if(i>10)	
			{
				sys_mbox_free(mbox);	
				Q_FULL=1;

			}	
		}
		OSTimeDlyHMSM(0, 0, 0, 100);
	}	
}
//*------------------------------------------------------------------------------------------------
//* ��ƦW�� : sys_arch_mbox_fetch
//* �\��y�z : �b���w���l�c���������A�Ө�Ʒ|����u�{���ܶW�ɩΪ̶l�c�ܤ֦���@������
//* �J�f�Ѽ� :    <mbox>[in]  ���w�����������l�c
//*          :     <msg>[out] ���G�ѼơA�O�s�����쪺�������w
//*          : <timeout>[in]  ���w���ݱ������̪��ɶ��A��0����@�����ݪ��ܱ���������A��쬰�@��
//* �X�f�Ѽ� : -                0: �b���w�ɶ����������
//*          : - SYS_ARCH_TIMEOUT: �b���w�ɶ����S���������
//*------------------------------------------------------------------------------------------------
u32_t sys_arch_mbox_fetch(sys_mbox_t mbox, void **msg, u32_t timeout)
{
	INT8U	__u8RtnVal;
	
	if(msg != NULL)
		*msg = OSQPend(mbox->hMBox, __MSToOSTicks(timeout), &__u8RtnVal);
	else 
		OSQPend(mbox->hMBox, __MSToOSTicks(timeout), &__u8RtnVal);

	if(__u8RtnVal == OS_NO_ERR)
		return 0;
	else
		return SYS_ARCH_TIMEOUT;
}
//*------------------------------------------------------------------------------------------------
//* ��ƦW�� : sys_thread_new
//* �\��y�z : �إߤ@�ӷs�u�{
//* �J�f�Ѽ� : <thread>[in] �s�u�{���J�f�a�}
//*          :    <arg>[in] �ǻ����s�u�{���Ѽ�
//*          :   <prio>[in] ��LwIP���w���s�u�{�u���šA�o���u���űq1�}�l
//* �X�f�Ѽ� : ��^�u�{�u���šA�`�N�o�Pprio���P�C�o�ӭȹ�ڵ���T_LWIP_THREAD_START_PRIO + prio�A
//*          : �p�G�إߤ����\�h��^0
//*------------------------------------------------------------------------------------------------
sys_thread_t sys_thread_new(void(*thread)(void *arg), void *arg, int prio)
{
	INT8U __u8Prio = 0;
	
	//* �p�G�u���ũw�q�S���W�X�t�Τ��\���d��
	if(prio > 0 && prio <= T_LWIP_THREAD_MAX_NB)
	{
		__u8Prio = T_LWIP_THREAD_START_PRIO + (prio - 1);
	
		if(OS_NO_ERR == OSTaskCreate(thread, arg, &T_LWIP_THREAD_STK[prio-1][T_LWIP_THREAD_STKSIZE-1], __u8Prio))
			return __u8Prio;
		else
            return 0;
	}
	else	
		return 0;
}
//*------------------------------------------------------------------------------------------------
//* ��ƦW�� : sys_arch_timeouts
//* �\��y�z : �����e�u�{�ϥΪ�sys_timeouts���c�����w
//* �J�f�Ѽ� : �L
//* �X�f�Ѽ� : ��^�@�ӫ��V��e�u�{�ϥΪ�sys_timeouts���c�����w
//*------------------------------------------------------------------------------------------------
struct sys_timeouts *sys_arch_timeouts(void)
{
	INT8S __s8Idx;
	
 //   printf("<%d>", OSTCBCur->OSTCBPrio);
        
	timeouts_null.next = NULL;
	//* ��h�_�l�q��o�����q�A�]�N�OLwIP�������u���ũw�q
	__s8Idx = OSTCBCur->OSTCBPrio - T_LWIP_THREAD_START_PRIO;
	
	//* ��e�u�{�b���w��LwIP�u�{�u���Ÿ��d�򤧤�
	if(__s8Idx >= 0 && __s8Idx < T_LWIP_THREAD_MAX_NB)
		return &__staSysTimeouts[__s8Idx];
	
	//* �p�G�X�{�N�~���p�A��e�u�{�ä��b���w��LwIP�u�{�u���Ÿ귽�����A�h��^__staSysTimeouts�Ʋժ��̫�@�Ӧ���
	 //  return &__staSysTimeouts[T_LWIP_THREAD_MAX_NB];
	 return &timeouts_null;
}
#endif //#if (NIC_SUPPORT && !ICOMMWIFI_SUPPORT)