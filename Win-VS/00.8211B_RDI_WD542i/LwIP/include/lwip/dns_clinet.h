#include "sysopt.h"
#if (NIC_SUPPORT && !ICOMMWIFI_SUPPORT)
//*------------------------------------------------------------------------------------------------
//* ���W				: dns_clinet.h
//* �\��y�z			: dns�Ȃ��݉����
//* �@��    			: �J���i
//* ����				: 0.1
//* �إߤ���B���}		: 2006/08/10 15:27
//* �̪�ק����B���}	: 
//* �ק��]			: 
//*------------------------------------------------------------------------------------------------

#ifndef	__dns_client_h__
#define	__dns_client_h__

//*---------------------------------------- �C���^�Χ��w�� -----------------------------------------
#define	DNS_PORT					53			//* ���w�n�������ݤf�A
#define	DNS_PACKET_QUERY_TYPE		0x01		//* ���w���d�R�ݫ��oIP�a�}�d�R
#define	DNS_PACKET_QUERY_CLASS		0x01		//* ���w���d�R�݃o���T�I�a�}

#define	DNS1_IP_ADDR	0x08080808				//* �DDNS�A�ﾹ�a�}�]0xC0A80A47�^
#define	DNS2_IP_ADDR	0xA85FC001				//* �¥�DNS�A�ﾹ�a�}
#define	DNS_OK			0						//* �d�R���\
#define DNS_NOT_EXIST	1						//* ���w����W���s�b
#define DNS_NOT_CONN	2						//* �Ӫk�������w����W�A�ﾹ�]�D�B�ªA�ﾹ�����i�Ρ^
#define DNS_OTHER		3						//* �����}�R

#define ST_NETCONN struct netconn
#define ST_NETBUF struct netbuf
#define ST_PBUF struct pbuf
#define ST_UDP_PCB   struct udp_pcb 
  /* source IP address */
#define ST_IP_ADDR   struct ip_addr 


//#define ST_ID_AND_FLAGS struct BST_ID_AND_FLAGS


typedef struct {
	INT32U bitID		:16;
	INT32U bitIsRD		:1;        				//* �O�_����c�Ьd�R�A�d�R���o1�A�_�h�A�bDNS�A�ﾹ�Ӫk����
	                          		 			//* �װ�W�Ӊ����Ȃ��ݥu��o��@���s�b�G�Ӊ�����W�A�ﾹ�a�}
	
	INT32U bitIsTC		:1;						//* �O�_�����I��		
	INT32U bitOpcode	:4;						//* 0�A�ዿ���d�R�F1�O�ϦV�d�R�F2�o�A�ﾹ�]�Ȉ[�D		
	INT32U bitIsAA		:1;						//* �O�_�o�Ц^��                               		 		
	INT32U bitCmd		:1;        				//* 0�O�d�R�A1�O����
	                               		
	INT32U bitOptResult	:4;        				//* ���w�d�R����W�A�ﾹ��^���ާ@�C�G
	INT32U bitRsrved    :3;        				//* �O�d�r�q�A�����o0
	INT32U bitIsRA      :1;        				//* ���w��W�A�ﾹ����c�Ьd�R
}BST_ID_AND_FLAGS;

//PACK_STRUCT_BEGIN
typedef struct {
	INT32U u32IDAndFlags;						//* �ዿ�M���
	INT16U u16QCount;							//* ��_�� 
	INT16U u16ACount;							//* �^����
	INT16U u16AuthCount;						//* �ІV���۶q
	INT16U u16ARC;								//* �]�~�V���Ӊ���
}ST_DNS_PACKET_HDR;

//PACK_STRUCT_END

#define DNS_PACKET_HDR_LEN	12					//* DNS�]�����c��

//* UDP������ۤJ�f���
typedef struct {
	INT32U u32RtnCode;
	INT32U *pu32IP;
	INT32S s32DNLen;
}ST_RECV_FUN_ARG;

//*-------------------------------------- ��ۭ쫬�R�� ---------------------------------------------
extern INT32U u32DNToIP(INT8S *pszDN, INT32S s32DNLen, INT32U *pu32IP);
#endif
#endif //#if (NIC_SUPPORT && !ICOMMWIFI_SUPPORT)