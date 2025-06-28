//*------------------------------------------------------------------------------------------------
//* ���W				: lib_emac.c
//* �\��y�z			: EMAC�~�]����Ʈw
//* �@��    			: ����§
//* ����				: 0.1
//* �إߤ���B�ɶ�		: 2006/10/08 10:25
//* �̪�ק����B�ɶ�	: 
//* �ק��]			: 
//*------------------------------------------------------------------------------------------------
//*------------------------------------------ �Y��� ----------------------------------------------
#include "arch/cc.h"
#include "netif/Dm9000.h"
#include "netif/etharp.h"
#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/lwipsys.h"
#include "ucos_ii.h"

u8_t  Host_ipaddr[4]={192,168,0,168};	
u8_t  my_hwaddr[6]= { 0x6c,0xf0,0x49,0xa2,0x66,0x68};
u8_t  broadcast_hwaddr[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

u8_t  my_ipaddr[5]={192,168,2,152};	
u8_t  my_subnet[5]={255,255,255,0};	
u8_t  gateway_ipaddr[5]={255,255,255,254};	
//*------------------------------------------------------------------------------------------------
//* ��ƦW�� : GetInputPacketLen
//* �\��y�z : �����F���H���]������
//* �J�f�Ѽ� : �L
//* �X�f�Ѽ� : �L
//*------------------------------------------------------------------------------------------------
//u16_t GetInputPacketLen(void)
//{
//	    u32_t *rx_status_addr;
//	    u16_t  Rxlen;
//	    
//	    rx_status_addr = (u32_t *)(RX_STATUS_ADDR + MAC_RXCONSUMEINDEX * 8); //#define RX_STATUS_ADDR		(RX_DESCRIPTOR_ADDR + RX_DESCRIPTOR_SIZE), �C�Ӵy�z��2�Ӫ��A�H�s��8�Ӧr�`�C 
//	    Rxlen = (*rx_status_addr & DESC_SIZE_MASK) ;//#define DESC_SIZE_MASK	0x000007FF 11 bits for both TX and RX 
//	    if((Rxlen>1500)||(Rxlen==0))
//	    	{
//	    		Rxlen=0;
//	    		MAC_RXCONSUMEINDEX=MAC_RXPRODUCEINDEX;
//	    	}
//	    return (Rxlen+1);
//}
//*------------------------------------------------------------------------------------------------
//* ��ƦW�� : EMACReadPacket
//* �\��y�z : �qEMACŪ���H���]��ӽЪ�pbuf��A�Ө�ƭn�DPBUF_POOL_BUFSIZE�j��ε��󱵦��w�İϡA�̦n�O��
//*			 : �ƭ��A�o�˨�ƳB�z��²��C�`�N�A�b�եθӨ�Ƥ��e�������ե�GetInputPacketLen()��ơA�o�ˤ~
//*			 : ��������T��Ū����m
//* �J�f�Ѽ� :          <pbTo>[in] ���Vpbuf�����w
//*			 : <u16SegmentLen>[in] pbuf���ݭn�s�x����ڼƾڪ��סA�]���b�ӽ�pbuf�ɡApbuf_alloc()�w�g�ھ�
//*			 :                     ��ڪ��V���N����ζi�F�h��pbuf�զ���pbuf�줤�A��pstPbuf->len�h�O�s�F
//*			 :                     �C��pbuf�ݭn�s�x���ƾڡC���y�ܻ�pbuf�줤���̫�@��pbuf��len�r�q������
//*			 :                     �Ӥp��ε���PBUF_POOL_BUFSIZE�A�ӫe����pbuf�h����PBUF_POOL_BUFSIZE
//*			 :  <blIsLastPbuf>[in] �O�_�O�̫�@��Pbuf
//* �X�f�Ѽ� : �L
//*------------------------------------------------------------------------------------------------
//void EMACReadPacket(struct pbuf *__Pstbuf ,u16_t Rlen) //��^�������סC
//{ 
//	    u8_t * REC_BUFF_PTR_DWORDS;		
//	    u8_t *data_start_addr;
//	    u32_t *__pp;
//	    struct pbuf     *__pstCurPbuf ;
//	    u16_t  i;
//	    u16_t			__u16LenToRead , 					       //* �r�`��
//					        __u16RemainLenToRead;						//* �ٳѦh�֦r�`�S��Ū����pbuf��
//      __pstCurPbuf =__Pstbuf ;
//      __u16RemainLenToRead=Rlen;
//      
//      for(; __pstCurPbuf!=NULL; __pstCurPbuf=__pstCurPbuf->next)
//      {
//      if(__u16RemainLenToRead >0)
//      {
//	    if(__u16RemainLenToRead>__pstCurPbuf->len)
//	    	{
//	      	__u16LenToRead=__pstCurPbuf->len;
//	      	__u16RemainLenToRead-=__pstCurPbuf->len;
//	      }
//	    else 
//	    	{
//	    	__u16LenToRead=__u16RemainLenToRead;
//	    	__u16RemainLenToRead=0;
//	      }
//	    		
//	    __pp = (u32_t *)(RX_DESCRIPTOR_ADDR + MAC_RXCONSUMEINDEX * 8);
//		  data_start_addr = (u8_t *)(*__pp);	  
//	  	REC_BUFF_PTR_DWORDS=(u8_t *)__pstCurPbuf->payload;   //�]�w�����w�İϪ��_�l�a�}	  //�������A�����H������356���C
//		  for(i=0;i<__u16LenToRead;i++)              // ���F�קK�r�`����a�Ӫ����D�o�̥γ�r�`Ū���C �p��ݭn���Φh��4�r�`���ƲաC
//			REC_BUFF_PTR_DWORDS[i]=*data_start_addr++;
//	    }
//	    else 
//	    	{break;}
//	    } 
//	//�ո� ��f��X�C
//	__u16RemainLenToRead=Rlen;
//	__pstCurPbuf =__Pstbuf ;
//	
//	while(__u16RemainLenToRead>0)
//	{
//		  if(__u16RemainLenToRead>__pstCurPbuf->len)
//		  	{
//		    __u16LenToRead=__pstCurPbuf->len;
//		    __u16RemainLenToRead-=__pstCurPbuf->len;
//		    }
//		else
//			{
//				__u16LenToRead=__u16RemainLenToRead;
//				__u16RemainLenToRead=0;
//				}
//			REC_BUFF_PTR_DWORDS=(u8_t *)__pstCurPbuf->payload;
//			for(i=0;i<__u16LenToRead;i++)
//		  {
//		  while((U0LSR&0x20)==0);
//		  U0THR	=REC_BUFF_PTR_DWORDS[i];
//		  }
//		  __pstCurPbuf=__pstCurPbuf->next;
//	}    
//	   
//     MAC_RXCONSUMEINDEX=MAC_RXPRODUCEINDEX;
//}

/******************************************************************************
** Function name:		EMAC_TxEnable/EMAC_TxDisable
**
** Descriptions:		EMAC TX API modules
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
__inline void EMAC_TxEnable( void )
{
//    MAC_COMMAND |= 0x02;  //�R�O�H�s���A bit1 �o�ͨϯ�C
 //   return;
}

__inline void EMAC_TxDisable( void )
{
//    MAC_COMMAND &= ~0x02;//�R�O�H�s���A bit1 �o�ͨϯ�C
//    return;
}

/******************************************************************************
** Function name:		EMAC_RxEnable/EMAC_RxDisable
**
** Descriptions:		EMAC RX API modules
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
__inline void EMAC_RxEnable( void )
{
//    MAC_COMMAND |= 0x01;//�R�O�H�s���A bit1 �o�ͨϯ�C
 //   MAC_MAC1 |= 0x01; //MAC�t�m�H�s��1 bit0 �m�줹�\�ﱵ���V�i�汵���C
//    return;    
}

__inline void EMAC_RxDisable( void )
{
//    MAC_COMMAND &= ~0x01;
//    MAC_MAC1 &= ~0x01;
//    return;
}
//void WritePHY( u32_t PHYReg, uint32 PHYData )
//{
//    MAC_MCMD = 0x0000;			/* write command */
//    MAC_MADR = 0X0300 | PHYReg;	/* [12:8] == PHY addr, [4:0]=0x00(BMCR) register addr */
//    MAC_MWTD = PHYData; //MII Mgmt�g�ƾڱH�s���C
//    while ( MAC_MIND != 0 );//MII MGMt ���A���ܱH�s�� bit0 ��^1���MII ���b����Ū�g�g���C
//    return;
//}

void Write_PHY (u16_t phyadd,int PhyReg, int Value)
{

}
/*****************************************************************************
** Function name:		ReadPHY
**
** Descriptions:		Read data from the PHY port
**
** parameters:			PHY register
** Returned value:		PHY data
** 
*****************************************************************************/

static u16_t
phy_read(int reg )
{
        DM9000_iow(DM9000_EPAR, DM9000_PHY | reg);

        DM9000_iow(DM9000_EPCR, 0x0C);  
        udelay(1000);          
        DM9000_iow(DM9000_EPCR, 0x00);  
        
       return ((DM9000_ior(DM9000_EPDRH) << 8)| DM9000_ior(DM9000_EPDRL));
}


/*****************************************************************************
** Function name:		EMACTxDesciptorInit
**
** Descriptions:		initialize EMAC TX descriptor table
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void EMACTxDescriptorInit( void )
{

}

/*****************************************************************************
** Function name:		EMACRxDesciptorInit
**
** Descriptions:		initialize EMAC RX descriptor table
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void EMACRxDescriptorInit( void ) 
{
    
}
 
 /**********************************************************************
**��ƭ쫬�G    void 	SetMacID()
**�J�f�Ѽ�:		*mac_ptr
**�X�f�Ѽ�:		�L
**�� �^ �ȡG	�L            
**��    ���G	�]�m������z�a�},���z�a�}�w�g�s�x�b�{�ǪŶ��� 
************************************************************************/
void SetMacID()   
{
int i, oft;
	
  for (i = 0, oft = 0x10; i < 6; i++, oft++)
                DM9000_iow(oft, my_hwaddr[i]);
	
}

//u32_t intcnt;
extern OS_EVENT  		*hEthernetInput; 
void Ethernet_Exception(void)  //���f�������_�A�ȵ{�ǡC
{
//	if( MAC_INTSTATUS & EMAC_INT_RXDONE )
	OSSemPost(hEthernetInput);
//	MAC_INTCLEAR = 0XFFFF; 
//	MAC_RXCONSUMEINDEX=MAC_RXPRODUCEINDEX;
//	VICVectAddr = 0;            //interrupt close �q�����_������_����
}

void EMACInit()  
{
int i, oft;
u8_t IO_mode;
u16_t  lnk;
		
        if(dm9000_probe()== FALSE)
            {
              if(dm9000_probe()== FALSE)
                {
                   DEBUG_GPIU(" DM9000 Init Fail\n");
		  		    return ;
 	  		      }
 	  		}
		/* set the internal PHY power-on */
        DM9000_iow(DM9000_GPR, 0x00);   
		dm9000_reset();
 
 		IO_mode = DM9000_ior(DM9000_ISR)&0x80;

		if(IO_mode)
			DEBUG_GPIU("8 bit mode\n"); 
		else
			DEBUG_GPIU("16 bit mode\n");
					/* Set PHY */
		phy_write(4, 0x01e1);
		phy_write(0, 0x1200); /* N-way */
		
        DM9000_iow(DM9000_NCR, 0); 
        DM9000_iow(DM9000_TCR, 0x1E);      
        DM9000_iow(DM9000_BPTR, 0x3f);/* Less 3kb, 600us */  
        DM9000_iow(DM9000_FCTR, FCTR_HWOT(3) | FCTR_LWOT(8));  
        DM9000_iow(DM9000_FCR, 0x0);   
        DM9000_iow(DM9000_SMCR, 0); /* Special Mode */   
        DM9000_iow(DM9000_NSR, NSR_WAKEST | NSR_TX2END | NSR_TX1END); /* clear TX status */  
        DM9000_iow(DM9000_ISR, 0x3f);/* Clear interrupt status */  
		DM9000_iow(DM9000_CSCR, CSCR_UDPCSE|CSCR_TCPCSE|CSCR_IPCSE);/* Enable TCP/IP/UDP Check Sum */ 

        for (i = 0, oft = 0x10; i < 6; i++, oft++)
                DM9000_iow(oft, my_hwaddr[i]);
        for (i = 0, oft = 0x16; i < 8; i++, oft++)
                DM9000_iow(oft, 0xff);

      
        DM9000_iow(DM9000_RCR, RCR_DIS_LONG | RCR_DIS_CRC | RCR_RXEN);  /* RX enable */
       // DM9000_iow(DM9000_IMR, IMR_PAR);        
        DM9000_iow(DM9000_IMR, IMR_PAR|IMR_PRM|IMR_PTM);        /* Enable TX/RX interrupt mask */
#if 1
        /* see what we've got */
        lnk = phy_read(17) >> 12;
        printf("operating at ");
        switch (lnk) {
        case 1:
                printf("10M half duplex \n");
                break;
        case 2:
                printf("10M full duplex \n");
                break;
        case 4:
                printf("100M half duplex \n");
                break;
        case 8:
                printf("100M full duplex\n ");
                break;
        default:
                printf("unknown: %x \n ", lnk);
                break;
        }
        i= DM9000_ior(DM9000_NSR);
        if(i && 0x40)
            printf("Link ok\n ");
        else
            printf("Link fail\n ");
        
        if(i && 0x80)
            printf("10Mbps \n ");
        else
            printf("100Mbps\n ");

        if(i && 0x08)
            printf("FUll duplex\n ");
        else
            printf("half duplex\n ");
        //printf("mode\n");
#endif
    	DMiSemRv= OSSemCreate(0);  
	    DMiSemTran= OSSemCreate(0);
       //DEBUG_GPIU("operating at 100M full duplex mode\n");
       return ;
}

