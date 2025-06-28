#if (NIC_SUPPORT && !ICOMMWIFI_SUPPORT)

//*------------------------------------------------------------------------------------------------
//* ���W			: dns_clinet.c
//* �\��y�z			: dns�Ȃ���
//* �@��    			: �J���i
//* ����				: 0.1
//* �إߤ���B���}		: 2006/08/10 15:23
//* �̪�ק����B���}	: 
//* �ק��]			: 
//*------------------------------------------------------------------------------------------------
//*------------------------------------------ ����� -----------------------------------------------
#include	"ucos_ii.h"
#include	"os_cpu.h"
#include	"../LwIP/include/lwip/tcpip.h"
#include	"../LwIP/include/lwip/api.h"

#include	"../LwIP/tools/stringExtAPI.h"
#include	"../LwIP/tools/search.h"
#include	"../LwIP/tools/converter.h"

#include	"../LwIP/include/lwip/dns_clinet.h"

//*================================================================================================
//*�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@��@�ۡ@��
//*================================================================================================
//*------------------------------------------------------------------------------------------------
//* ��ۦW�� : __ilvPacketDNSQuest
//* �\��y�z : �ʆEDNS�d�R�]
//*          :     <pszDN>[in] ���V��W������
//*          :  <s32DNLen>[in] ��W�c��
//*          :   <pstPbuf>[in] ���Vst_pbuf�C�۪�����A�������C�۫O�s�n���e��DNS���d�R�]
//* �X�f��� : ��
//*------------------------------------------------------------------------------------------------
__inline void __ilvPacketDNSQuest(INT8S *pszDN, INT32S s32DNLen, ST_PBUF *pstPbuf)
{
	ST_DNS_PACKET_HDR*	__pstDNSPacket;
	BST_ID_AND_FLAGS* __pbstIDAndFlags;
	 
	__pstDNSPacket = (ST_DNS_PACKET_HDR*)pstPbuf->payload;
	/*Do query recursively.  Modified by aher 2014/04/02*/
	//__pstDNSPacket->u32IDAndFlags = 0x00000000;
	__pstDNSPacket->u32IDAndFlags = 0x00010000;		
	//
	__pbstIDAndFlags =(BST_ID_AND_FLAGS*)__pstDNSPacket->u32IDAndFlags;
	__pbstIDAndFlags->bitID = 0x0608;
	__pbstIDAndFlags->bitIsRD = 0x1;
	__pstDNSPacket->u16QCount = 0x0100;
	__pstDNSPacket->u16ACount = 0x0000;
	__pstDNSPacket->u16AuthCount = 0x0000;
	__pstDNSPacket->u16ARC = 0x0000;
	memcpy((INT8U*)pstPbuf->payload + DNS_PACKET_HDR_LEN, pszDN, s32DNLen);
	*((INT8U*)pstPbuf->payload + DNS_PACKET_HDR_LEN + s32DNLen) = 0x00;
	*((INT8U*)pstPbuf->payload + DNS_PACKET_HDR_LEN + s32DNLen + 1) = DNS_PACKET_QUERY_TYPE;
	*((INT8U*)pstPbuf->payload + DNS_PACKET_HDR_LEN + s32DNLen + 2) = 0x00;
	*((INT8U*)pstPbuf->payload + DNS_PACKET_HDR_LEN + s32DNLen + 3) = DNS_PACKET_QUERY_CLASS;
}
//*------------------------------------------------------------------------------------------------
//* ��ۦW�� : __ilu32ParseRespDNSPacket
//* �\��y�z : �ѪRDNS�A�ﾹ���ت����u�]
//* �J�f��� :  <pstPbuf>[in] ���VST_NETBUF������
//*          :   <pu32IP>[in] ���V�����G�L��]�O�sIP�a�}�^������
//*          : <s32DNlen>[in] �n�d�R��W���c��
//* �X�f��� : - DNS_OK	       : �d�R���\
//*          : - DNS_NOT_EXIST : ���w����W���s�b
//*          : - DNS_OTHER     : �����}�R
//*------------------------------------------------------------------------------------------------
__inline INT32U __ilu32ParseRespDNSPacket(ST_PBUF *pstPbuf, INT32U *pu32IP, INT32S s32DNLen)
{
	ST_DNS_PACKET_HDR	*__pstDNSPacket;

	BST_ID_AND_FLAGS	*__pbstIDAndFlags;
	void 				*__pvData;
	INT16U				__u16ACount, __u16AnswerDataLen, __u16Offset;
	
	__pstDNSPacket = (ST_DNS_PACKET_HDR	*)pstPbuf->payload;

	__pbstIDAndFlags = (BST_ID_AND_FLAGS*)__pstDNSPacket->u32IDAndFlags;
	

/*Sometimes the value of bitOptResult was error, but the IP address was right,shouldn't return error.     Modified by AHER 2014/04/07*/
	if(__pbstIDAndFlags->bitOptResult == 3)
	{
		return  DNS_NOT_EXIST;
	}
	else// if(__pbstIDAndFlags->bitOptResult == 0)
	{
		__u16ACount = macHighToLowForWord(__pstDNSPacket->u16ACount);
		__u16Offset = s32DNLen + 14;
		while(__u16ACount > 0)
		{
			__pvData = (INT8U*)pstPbuf->payload + DNS_PACKET_HDR_LEN + __u16Offset;
			__u16AnswerDataLen = macHighToLowForWord(*((__packed INT16U*)__pvData));
			
			if((__u16AnswerDataLen == 4) && macHighToLowForWord(*((__packed INT16U*)((INT8U*)__pvData - 8))) == DNS_PACKET_QUERY_TYPE)
			{
				*pu32IP = *((__packed INT32U *)((INT8U*)__pvData + 2));				
				
				return DNS_OK;
			}
			
			__u16Offset += 12 + __u16AnswerDataLen;
			
			__u16ACount--;
		}
		
		return DNS_OTHER;
	}
	/*
	else if(__pbstIDAndFlags->bitOptResult == 3)
	{
		return  DNS_NOT_EXIST;
	}
	
	return DNS_OTHER;
	*/
}
//*------------------------------------------------------------------------------------------------
//* ��ۦW�� : __vDNSRecv
//* �\��y�z : �����}�ѪRDNS�A�ﾹ���ت����u�]�A���O�@���^�`��ۡA��udp_recv()���m
//* �J�f��� :    <pstArg>[in] ���VST_RECV_FUN_ARG�C�۪�����A�����C�۫O�s�M�G��ۻݭn���J�f���
//*          : <pstUDPPCB>[in] ���VST_UDP_PCB�C�۪�����
//*          :   <pstPbuf>[in] ���Vst_pbuf�C�۪�����A���O�s�M���쪺�X�e
//*          :     <pstIP>[in] IP�a�}
//*          :   <u16Port>[in] �ݤf�A
//* �X�f��� : - DNS_OK	       : �d�R���\
//*          : - DNS_NOT_EXIST : ���w����W���s�b
//*          : - DNS_OTHER     : �����}�R
//*------------------------------------------------------------------------------------------------
static void __vDNSRecv(ST_RECV_FUN_ARG *pstArg, ST_UDP_PCB *pstUDPPCB, 
						ST_PBUF *pstPbuf, ST_IP_ADDR *pstIP, INT16U u16Port)
{
	if(pstPbuf != NULL)
	{
		pstArg->u32RtnCode = __ilu32ParseRespDNSPacket(pstPbuf, pstArg->pu32IP, pstArg->s32DNLen);
	
		pbuf_free(pstPbuf);
	}
}
//*------------------------------------------------------------------------------------------------
//* ��ۦW�� : __u32GetIP
//* �\��y�z : �d�RDNS�A�ﾹ�A�������w��W��w��IP�a�}�A�����u��ϥ΃V�V�窽�����f���c�A�ϥΨ䴣��
//*          : ��API�Ӫk�֨��ݨD�C�p�G����DNS���إ]�ϥ�netconn_recv��۫D�`�e������t�i�A�]�oUDP
//*          : ���i�a�C
//*          :  <pstPbuf>[in] ���Vst_pbuf�C�۪�����A�������C�۫O�s�n���e��DNS���d�R�]
//*          :   <pu32IP>[in] ���V�����G�L��]�O�sIP�a�}�^������
//*          : <s32DNLen>[in] �n�d�R����W�c��
//*          : <u32DNSIP>[in] DNS�A�ﾹ��IP�a�}
//* �X�f��� : - DNS_OK	       : �d�R���\
//*          : - DNS_NOT_EXIST : ���w����W���s�b
//*          : - DNS_NOT_CONN  : �Ӫk�������w����W�A�ﾹ�]�D�B�ªA�ﾹ�����i�Ρ^
//*          : - DNS_OTHER     : �����}�R
//*------------------------------------------------------------------------------------------------
INT32U __u32GetIP(ST_PBUF *pstPbuf, INT32U *pu32IP, INT32S s32DNLen, INT32U u32DNSIP)
{
	ST_IP_ADDR 			__stIPAddr;
	ST_UDP_PCB			*__pstUDPPCB;
	ST_RECV_FUN_ARG		__stRecvArg;
	INT32S				i, k = 0;
	INT32U				__u32RtnCode;
	
	__pstUDPPCB = udp_new();
	if(__pstUDPPCB == NULL)
		return DNS_OTHER;
	
	__stIPAddr.addr = htonl(u32DNSIP);
	if(udp_connect(__pstUDPPCB, &__stIPAddr, DNS_PORT) != ERR_OK)
		return DNS_OTHER;
		
	memset(&__stRecvArg, 0, sizeof(ST_RECV_FUN_ARG));
	__stRecvArg.u32RtnCode = -1;
	__stRecvArg.pu32IP = pu32IP;
	__stRecvArg.s32DNLen = s32DNLen;
	
	udp_recv(__pstUDPPCB, __vDNSRecv, &__stRecvArg);
	
	//* ���e�M����
	while(k<15)
	{
		udp_send(__pstUDPPCB, pstPbuf);		

		i = 0;	
		while(i < 30)
		{
			__u32RtnCode = __stRecvArg.u32RtnCode;
			
			if(__u32RtnCode != -1)
				goto __lblEnd;
			
			OSTimeDlyHMSM(0, 0, 0, 100);
			i++;
		}
		
		k++;
	}				
	
__lblEnd:
	//* �����u�@�A�^���e�Ϊ��V��
	udp_disconnect(__pstUDPPCB);
	udp_remove(__pstUDPPCB);
	
	if(__u32RtnCode != -1)
		return __stRecvArg.u32RtnCode;
	else
		return DNS_NOT_CONN;
}
//*------------------------------------------------------------------------------------------------
//* ��ۦW�� : u32DNToIP
//* �\��y�z : �d�RDNS�A�ﾹ�A����w����W�H��oIP�a�}
//* �J�f��� :    <pszDN>[in] ���V��W������
//*          : <s32DNLen>[in] ��W�c��
//*          :   <pu32IP>[in] ���V�����G�L��]�O�sIP�a�}�^������
//* �X�f��� : - DNS_OK	       : �d�R���\
//*          : - DNS_NOT_EXIST : ���w����W���s�b
//*          : - DNS_NOT_CONN  : �Ӫk�������w����W�A�ﾹ�]�D�B�ªA�ﾹ�����i�Ρ^
//*          : - DNS_OTHER     : �����}�R
//*------------------------------------------------------------------------------------------------
INT32U u32DNToIP(INT8S *pszDN, INT32S s32DNLen, INT32U *pu32IP)
{
	ST_PBUF         	*__pstPbuf = NULL;
	INT32S				__s32TotLen;
	INT32U				__u32RtnCode;

	__s32TotLen = DNS_PACKET_HDR_LEN + s32DNLen + 4;
	__pstPbuf = pbuf_alloc(PBUF_RAW, __s32TotLen, PBUF_POOL);
	if(__pstPbuf == NULL)
		return DNS_OTHER;
		
	__ilvPacketDNSQuest(pszDN, s32DNLen, __pstPbuf);

	__u32RtnCode = __u32GetIP(__pstPbuf, pu32IP, s32DNLen, DNS1_IP_ADDR);
	if(__u32RtnCode == DNS_NOT_CONN || __u32RtnCode == DNS_OTHER)
		__u32RtnCode = __u32GetIP(__pstPbuf, pu32IP, s32DNLen, DNS2_IP_ADDR);
	
	pbuf_free(__pstPbuf);
		
	return __u32RtnCode;
}
#endif //#if (NIC_SUPPORT && !ICOMMWIFI_SUPPORT)