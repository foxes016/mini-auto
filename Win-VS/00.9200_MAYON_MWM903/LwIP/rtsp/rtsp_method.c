#include "general.h" 
#include "../LwIP/include/lwip/net.h"
#include "../LwIP/include/lwip/rtp.h"
#include "../LwIP/include/lwip/rtsp.h"
#include "../LwIP/include/lwip/sdp.h"
#include "../LwIP/include/lwip/api.h"
#if 0
#include <alloca.h>
#define my_alloca(x) alloca(x)
#define my_free(x)
#else
#define my_alloca(x) malloc(x)
#define my_free(x) free(x)
#endif
extern CONNECTION  rtsp_conxn[];
extern u8  my_ipaddr[];
extern u8  my_hwaddr[];
extern int AV_CH;
/* Add supported format extensions here*/
int is_supported_url(char *p)
{ 
    if (strcmp(p,".SD")==0) {
        return 1;
    }
    if (strcmp(p,".WAV")==0) {
        return 1;
    }
    if (strcmp(p,".MP3")==0) {
        return 1;
    }
    if (strcmp(p,".GSM")==0) {
        return 1;
    }
    if (strcmp(p,".GSM-FLO")==0) {
        return 1;
    }
    if (strcmp(p,".L16M8K")==0) {
        return 1;
    }
    if (strcmp(p,".26l")==0) {
        return 1;
    }
    if (strcmp(p,".mpg")==0) {
        return 1;
    }
    if (strcmp(p,".mpeg")==0) {
        return 1;
    }
    if (strcmp(p,".m1v")==0) {
        return 1;
    }
    if (strcmp(p,".m2v")==0) {
        return 1;
    }
    if (strcmp(p,".m4v")==0) {
        return 1;
    }
    if (strcmp(p,".mp4")==0) {
        return 1;
    }
    if (strcmp(p,".xvid")==0) {
        return 1;
    }
    if (strcmp(p,".rtp")==0) {
        return 1;
    }
    return 0;
}

/*
 *�p�G�ǤJ��URL�D�k�A��^1�A�X�k��^0�A�������~(�p�w�ĤӤp��)��^-1
 *��g�������a�}�B�ݤf���B���W���Ѽ��ܶq�A�ǥX
 *
 */
int parse_url(const char *url, char *server, size_t server_len, unsigned short *port, char *file_name, size_t file_name_len)
/*Note: this routine comes from OMS*/
{
    /* expects format '[rtsp://server[:port/]]filename' */

    int not_valid_url = 1;
    
    /*����URL */
    char *full = my_alloca(strlen(url) + 1);
    strcpy(full, url);
//aher
/*
if (full == NULL)
	printf("aher : malloc fail \n");
printf("aher : full = %s\nURL = %s\n",full,url);
*/
//aher , �]��malloc����, �Ȯɬٲ�parsing URL.
return 0;
//
    /*�ˬd�e��O�_���T*/
    if (strncmp(full, "rtsp://", 7) == 0) 
    {
        char *token;
        int has_port = 0;
        
        /* BEGIN Altered by Ed Hogan, Trusted Info. Sys. Inc. */
        /* Need to look to see if it has a port on the first host or not. */
        
        /*�����e����䪺����*/
        char *aSubStr = my_alloca(strlen(url) + 1);
        strcpy(aSubStr, &full[7]);

        
        /*���/ �e�䪺�����a�}��aSubStr*/
        if (strchr(aSubStr, '/')) 
        {
            int len = 0;
            unsigned short i = 0;
            char *end_ptr;
            end_ptr = strchr(aSubStr, '/');
            len = end_ptr - aSubStr;
            for (; (i < strlen(url)); i++)
                aSubStr[i] = 0;
            strncpy(aSubStr, &full[7], len);
        }   

        /*�d�ݬO�_���ݤf��*/
        if (strchr(aSubStr, ':'))
            has_port = 1;
            my_free(aSubStr);
        /* END   Altered by Ed Hogan, Trusted Info. Sys. Inc. */

         /* expects format '[rtsp://server[:port/]]filename' */
        token = strtok(&full[7], " :/\t\n");
        if (token) 
        {
            strncpy(server, token, server_len);
            /*�ˬdstrncpy�i�ಣ�ͪ����~*/
            if (server[server_len-1]) 
            {
                my_free(full);
                return -1; 
            }

            /*��o�ݤf���A�g�J�ܶq*/
            if (has_port) 
            {
                char *port_str = strtok(&full[strlen(server) + 7 + 1], " /\t\n");
                if (port_str)
                    *port = (unsigned short) atol(port_str);
                else
                    *port = RTSP_PORT;
            } else
                    *port = RTSP_PORT;
                
            /* �ѪR�í������W*/
            not_valid_url = 0;
            token = strtok(NULL, " ");
            if (token) 
            {
                strncpy(file_name, token, file_name_len);
                if (file_name[file_name_len-1]) 
                {
                    my_free(full);
                    return -1; 
                }
            } 
            else
            file_name[0] = '\0';
        }
    } 
    else 
    {
        /* �ȶȭ������W */
        char *token = strtok(full, " \t\n");
        if (token) 
        {
            strncpy(file_name, token, file_name_len);
            if (file_name[file_name_len-1])
            {
                my_free(full);
                return -1; // internal error
            }
            server[0] = '\0';
            not_valid_url = 0;
        }
    }

    /*����Ŷ�*/
    my_free(full);
    return not_valid_url;
}

/*�K�[�ɶ��W*/
void add_time_stamp(char *b, int crlf)
{
    struct tm *t;
    time_t now;

    /*
    * concatenates a null terminated string with a
    * time stamp in the format of "Date: 23 Jan 1997 15:35:06 GMT"
    */
  //  now = time(NULL);
  //  t = gmtime(&now);
  //  strftime(b + strlen(b), 38, "Date: %a, %d %b %Y %H:%M:%S GMT"RTSP_EL, t);
    
    if (crlf)
        strcat(b, "\r\n");	/* add a message header terminator (CRLF) */
}

int send_describe_reply(RTSP_buffer * rtsp, char *object, description_format descr_format, char *descr)
{
    char *r,p[2048];            /* �Ω�����T���w�ī��w*/
    u16 body_len;
    /* ���t�Ŷ��A�B�z�������~*/
    DEBUG_RTSP("send_describe_reply()\n");
    //p = malloc( 2048);
#if 0    
    if (!p ) 
    {
        DEBUG_RTSP(" unable to allocate memory\n");
        send_reply(500, 0, rtsp);    /* internal server error */
        if (p) 
        {
            free(p);
        }
        return ERR_ALLOC;
    }
#endif
    r=p;
    /*�c�ydescribe������*/
   // sprintf(r, "%s %d %s"RTSP_EL"CSeq: %d"RTSP_EL"Server: %s/%s"RTSP_EL, RTSP_VER, 200, get_stat(200), rtsp->rtsp_cseq, PACKAGE, VERSION);
    //add_time_stamp(r, 0);                 /*�K�[�ɶ��W*/
   // sprintf(r, "%s %d OK"RTSP_EL"CSeq: %d"RTSP_EL"Server: %s/%s"RTSP_EL, RTSP_VER, 200, rtsp->rtsp_cseq, PACKAGE, R_VERSION);
    
    sprintf(r, "%s 200 OK"RTSP_EL"CSeq: %d"RTSP_EL, RTSP_VER, rtsp->rtsp_cseq);
    /*�Ω�ѪR���餺�۹�url�� ����url*/
    sprintf(r + strlen(r), "Content-Base: rtsp://%d.%d.%d.%d/%s/"RTSP_EL, my_ipaddr[0],my_ipaddr[1],my_ipaddr[2],my_ipaddr[3], object);
    strcat(r, "Content-Type: application/sdp"RTSP_EL);   /*�����Y�A��ܹ�������*/
    sprintf(r + strlen(r), "Content-Length: %d"RTSP_EL, strlen(descr));    /*SDP������*/
    strcat(r, RTSP_EL);
    /*�����Y����*/

    /*�[�W������*/
    strcat(r, descr);    /*describe����*/
//    bwrite(r, (unsigned short) strlen(r), rtsp);     /*�V�w�İϤ���R�ƾ�*/
  //  DEBUG_RTSP("LEN= %d \n %s ",strlen(r),r);
    body_len=strlen(r);

    netconn_write(rtsp->pstConn, (u8 *)p, body_len,NETCONN_COPY);
 //   printf("!!!!!!!!!! body_len = %d\n",body_len);
//    rtsp_send((u8 *)p, 20+body_len ,rtsp->nr);
 //   rtsp_conxn[rtsp->nr].my_sequence += body_len;
	
    //free(p);

    return ERR_NOERROR;
}

/*
****************************************************************
*�B�zDESCRIBE��k
****************************************************************
*/
int RTSP_describe(RTSP_buffer * rtsp)
{
        
    int valid_url, res;
    char object[255], server[255], trash[255];
    char *p;
    unsigned short port;
    char url[255];
    media_entry media, req;
    description_format descr_format = df_SDP_format;    
    char descr[MAX_DESCR_LENGTH];
     DEBUG_RTSP("processing describe method!\n");   /*just for debug,yanf*/

    /*�ھڦ��쪺�ШD,�ШD�����A���L��k�W�A�����XURL*/
    if (!sscanf(rtsp->in_buffer, " %*s %254s ", url)) 
    {
        send_reply(400, 0, rtsp);                       /* bad request */
        return ERR_NOERROR;
    }
#if 1
    /*�����e����䪺����*/
       p= &url[7];
        
        /*���/ �e�䪺�����a�}��aSubStr*/
        if (strchr(p, '/')) 
        {
            char *end_ptr;
            end_ptr = strchr(p, '/');
    
	    AV_CH = end_ptr[1]-48;
		if(AV_CH>2) AV_CH=0;
	//	printf("\n====ch%d\n",AV_CH);
        }   
	else printf("no find \n");	
#endif
    /*����URL */
    switch (parse_url(url, server, sizeof(server), &port, object, sizeof(object))) 
    {
        case 1: /*�ШD���~*/
            send_reply(400, 0, rtsp);
            return ERR_NOERROR;
//            break;
            
        case -1: /*�������~*/
            DEBUG_RTSP("url error while parsing !\n");
            send_reply(500, 0, rtsp);
            return ERR_NOERROR;
 //           break;
            
        default:
            break;
    }
    
    if (strstr(object, "../")) 
    {
        /* �����\���a���|���~���۹���|*/
        send_reply(403, 0, rtsp);   /* Forbidden */
        return ERR_NOERROR;
    }
    
    if (strstr(object, "./")) 
    {
        /*�����\�۹���|�A�]�A�۹���e���۹���| */
        send_reply(403, 0, rtsp);    /* Forbidden */
        return ERR_NOERROR;
    }
  
    /*�T���Y����require�ﶵ*/
    if (strstr(rtsp->in_buffer, HDR_REQUIRE)) 
    {
        /*DEBUG_PRINTF("Has require header!Another describe will be issued maybe!\n");*/   /*just for debug,yanf*/
        send_reply(551, 0, rtsp);    /* ��������ﶵ*/
        return ERR_NOERROR;
    }

    /*DEBUG_PRINTF("using sdp to parse media session description!\n");*/   /*just for debug,yanf*/

    /* ��o�y�z�Ů榡�A����SDP*/
    if (strstr(rtsp->in_buffer, HDR_ACCEPT) != NULL) 
    {
        if (strstr(rtsp->in_buffer, "application/sdp") != NULL) 
        {
            descr_format = df_SDP_format;
        } 
        else
        {
            /* ��L���y�z�Ů榡*/
            send_reply(551, 0, rtsp);    /* ��������C������*/
            return ERR_NOERROR;
        }
    }
    
    /*���o�ǦC��,�åB�������o�ӿﶵ*/
    if ((p = strstr(rtsp->in_buffer, HDR_CSEQ)) == NULL) 
    {
        send_reply(400, 0, rtsp);  /* Bad Request */
        return ERR_NOERROR;
    } 
    else 
    {
        if (sscanf(p, "%254s %d", trash, &(rtsp->rtsp_cseq)) != 2) 
        {
            send_reply(400, 0, rtsp);   /*�ШD���~*/
            return ERR_NOERROR;
        }
    }
    memset(&media, 0, sizeof(media));
    memset(&req, 0, sizeof(req));
    req.flags = ME_DESCR_FORMAT;
    req.descr_format = descr_format;
      /*DEBUG_PRINTF("&&&&&&&&&&&&--getting media description!!!!!!--&&&&&&&&&&&&\n");*/   /*just for debug,yanf*/
    if (get_SDP_descr(descr,object)==-1) 
       {
           DEBUG_RTSP("get_SDP_descr\n");
        	send_reply(500, 0, rtsp);  /*�A�Ⱦ��������~*/
        	return ERR_NOERROR;
        }
 
    /*�p�G�F��F�̤j�s���ơA�h���w�V*/
 //   if(max_connection()==ERR_GENERIC)
 //   {
        /*redirect*/
 //       return send_redirect_3xx(rtsp, object);  
 //   }

 //   DEBUG_RTSP("DESCRIBE %s RTSP/1.0 \n",url);
    
    /*DEBUG_PRINTF("prepare to send describe reply to the client!\n");*/    /*just for debug,yanf*/
    
    send_describe_reply(rtsp, object, descr_format, descr);               /*�o�eDESCRIBE*/

   
    /*DEBUG_PRINTF("end of processing  describe  method!\n");*/   /*just for debug,yanf*/
    return ERR_NOERROR;
}

int send_teardown_reply(RTSP_buffer * rtsp, long session_id, long cseq)
{
    char p[1024],*r;
    char temp[30];
    u16 body_len;
    
    r=p;
    /* �c�ئ^�_����*/
    sprintf(r, "%s %d %s"RTSP_EL"CSeq: %ld"RTSP_EL"Server: %s/%s"RTSP_EL, RTSP_VER, 200, get_stat(200), cseq, PACKAGE, R_VERSION);
    /*�ɶ��W*/
    add_time_stamp(r, 0);
    strcat(r, "Session: ");
    /*�|��ID*/
    sprintf(temp, "%ld", session_id);
    strcat(r, temp);
    
    strcat(r, RTSP_EL RTSP_EL);

    /*�g�J�w�İ�*/
//    bwrite(r, (unsigned short) strlen(r), rtsp);     /*�V�w�İϤ���R�ƾ�*/
    body_len=strlen(r);
    netconn_write(rtsp->pstConn, (u8 *)p, body_len,NETCONN_COPY);	
  //  rtsp_send((u8 *)p, 20+body_len ,rtsp->nr);
 //   rtsp_conxn[rtsp->nr].my_sequence += body_len;
    return ERR_NOERROR;
}

/*
****************************************************************
*TEARDOWN��k�B�z���
****************************************************************
*/

int RTSP_teardown(RTSP_buffer * rtsp)
{
    /*�����ܶq*/
    long int session_id;
    char *p;
    RTSP_session *s;
    RTP_session *rtp_curr, *rtp_prev = NULL, *rtp_temp;
    int valid_url;
    char object[255], server[255], trash[255], *filename;
    unsigned short port;
    char url[255];
    unsigned int cseq;

    /*�ˬd�ǦC��*/ 
    if ((p = strstr(rtsp->in_buffer, HDR_CSEQ)) == NULL) 
    {
        send_reply(400, 0, rtsp);	/* Bad Request */
        return ERR_PARSE;
    } 
    else
    {
        if (sscanf(p, "%254s %d", trash, &(rtsp->rtsp_cseq)) != 2)
        {
            send_reply(400, 0, rtsp);  /* Bad Request */
            return ERR_PARSE;
        }
    }    
    cseq = rtsp->rtsp_cseq;    

    /*�����M�ѪRURL*/
    if (!sscanf(rtsp->in_buffer, " %*s %254s ", url)) 
    {
        send_reply(400, 0, rtsp);   /* bad request */
        return ERR_PARSE;
    }
    
    /*����URL�O�_���T*/
    switch (parse_url(url, server, sizeof(server), &port, object, sizeof(object)))
    {
        case 1:          /*�ШD���~*/
            send_reply(400, 0, rtsp);
            return ERR_PARSE;
            break;
            
        case -1: /*�A�Ⱦ��������~*/
            DEBUG_RTSP("500_RTSP_teardown\n");
            send_reply(500, 0, rtsp);
            return ERR_PARSE;
            break;
            
        default:
            break;
    }
    
  

    /*�T��ϥά۹���|�X��*/
    if (strstr(object, "../")) 
    {
        send_reply(403, 0, rtsp);/* Forbidden */
        return ERR_PARSE;
    }
    if (strstr(object, "./"))
    {
        send_reply(403, 0, rtsp);/* Forbidden */
        return ERR_PARSE;
    }

    /*�ˬd�C���������X�k��*/
    p = strrchr(object, '.');
	//aher
    valid_url = 0;
    //if (p == NULL) 
    //{
    	//printf("aher : 415-1\n");
        //send_reply(415, 0, rtsp);/* Unsupported media type */
        //return ERR_PARSE;
    //} 
    //else 
    //{
        valid_url = is_supported_url(p);
   // }
    if (!valid_url) 
    {
        //aher
    	//printf("aher : 415-2\n");
        //send_reply(415, 0, rtsp);/* Unsupported media type */
        //return ERR_PARSE;
    }

    /*�B�z�|��*/
    if ((p = strstr(rtsp->in_buffer, HDR_SESSION)) != NULL)  /**�ˬd�|���Y*/
    {
        if (sscanf(p, "%254s %ld", trash, &session_id) != 2) 
        {
            send_reply(454, 0, rtsp);	/* Session Not Found */
            return ERR_PARSE;
            // return ERR_NOERROR;
        }
    } 
    else 
    {
        session_id = -1;
    }

    /*�|�ܦC��*/
    s = rtsp->session_list;
    if (s == NULL) 
    {
        //aher
        //send_reply(415, 0, rtsp);
        //return ERR_GENERIC;
    }
    if (s->session_id != session_id)
    {
        send_reply(454, 0, rtsp); /* Session Not Found */
        return ERR_PARSE;
    }

    /*�o�e�^�_*/

    
    send_teardown_reply(rtsp, session_id, cseq);
    
    /*Compatibility with RealOne and RealPlayer */
    if (strchr(object, '!'))	
        filename = strchr(object, '!') + 1;
    else
        filename = object;



    /*����Ҧ���URI RTP�|��*/
    rtp_curr = s->rtp_session;
   

    /*�������Ŷ�*/
    if (s->rtp_session == NULL) 
    {
        free(rtsp->session_list);
        rtsp->session_list = NULL;
    }

    return ERR_NOERROR;
}
int send_play_reply(RTSP_buffer * rtsp, char *object, RTSP_session * rtsp_session)
{
    char b[512];
    //RTP_session *p = rtsp_session->rtp_session;
    char *r;
    u16 body_len;
    /* build a reply message */
    r=b;
    sprintf(r, "%s %d %s"RTSP_EL"CSeq: %d"RTSP_EL"Server: %s/%s"RTSP_EL, RTSP_VER, 200, get_stat(200), rtsp->rtsp_cseq, PACKAGE,R_VERSION);
   // add_time_stamp(r, 0);

    sprintf(r + strlen(r), "Session: 53434834C7A0B7729DDEB8F7E5B554"RTSP_EL);
    /*
    strcat(r, "Session: ");
    sprintf(temp, "%d", rtsp_session->session_id);    
    strcat(r, temp);
    strcat(r, RTSP_EL);
    */
    strcat(r, "Range: npt=now-"RTSP_EL);
    strcat(r, "RTP-info: ");

    //Video
    strcat(r, "url=");
  	sprintf(r + strlen(r), "rtsp://%d.%d.%d.%d/trackID=1;",my_ipaddr[0],my_ipaddr[1],my_ipaddr[2],my_ipaddr[3]);
    sprintf(r + strlen(r), "seq=%u;rtptime=%u, ", RTP_session_for_video.seq, RTP_session_for_video.rtptime);               
    
    //audio
    strcat(r, "url=");
	sprintf(r + strlen(r), "rtsp://%d.%d.%d.%d/trackID=2;",my_ipaddr[0],my_ipaddr[1],my_ipaddr[2],my_ipaddr[3]);
    sprintf(r + strlen(r), "seq=%u;rtptime=%u ", RTP_session_for_audio.seq, RTP_session_for_audio.rtptime);                   
    strcat(r, RTSP_EL);

    sprintf(r + strlen(r), "Content-Length: 0"RTSP_EL"Cache-Control: no-cache"RTSP_EL);    /*SDP������*/        
    strcat(r, RTSP_EL);
   // bwrite(r, (unsigned short) strlen(r), rtsp);
    
    body_len=strlen(r);

    netconn_write(rtsp->pstConn, (u8 *)b, body_len, NETCONN_COPY);	
   // rtsp_send((u8 *)s, 20+body_len ,rtsp->nr);
   // rtsp_conxn[rtsp->nr].my_sequence += body_len;
    return ERR_NOERROR;
}

/*
****************************************************************
*PLAY ��k���B�z
****************************************************************
*/

int RTSP_play(RTSP_buffer * rtsp)
{

    /*�����ܶq*/
    int url_is_file;
    char object[255], server[255], trash[255];
    char url[255];
    unsigned short port;
    char *p = NULL, *q = NULL;
    long int session_id;
    RTSP_session *ptr;
    RTP_session *ptr2;
    play_args args;
    int time_taken = 0;
    
    DEBUG_RTSP("prepare to play!\n");

    send_play_reply(rtsp, object, rtsp->session_list);
    return ERR_NOERROR;
    
    /*�ˬd�O�_�]�t�ǦC���Y*/
    if ((p = strstr(rtsp->in_buffer, HDR_CSEQ)) == NULL) 
    {
        send_reply(400, 0, rtsp);   /* Bad Request */
        return ERR_NOERROR;
    } 
    else 
    {
        if (sscanf(p, "%254s %d", trash, &(rtsp->rtsp_cseq)) != 2) 
        {
            send_reply(400, 0, rtsp);    /* Bad Request */
            return ERR_NOERROR;
        }
    }
    
    /*�������d��*/
    args.playback_time_valid = 0;
    args.start_time_valid = 0;

    /*�ˬdRange�Y*/
    if ((p = strstr(rtsp->in_buffer, HDR_RANGE)) != NULL) 
    {
        q = strstr(p, "npt");   /*�B�zNPT*/
        if (q == NULL) 
        {
            q = strstr(p, "smpte");  /*�B�zsmpte*/
            if (q == NULL) 
            {
                q = strstr(p, "clock");
                
                if (q == NULL) 
                {
                    /*�S�����w�榡�A�ϥ� NeMeSI   �榡*/                                   
                    if ((q = strstr(p, "time")) == NULL) 
                    {
                         
                        double t;
                        q = strstr(p, ":");
                        sscanf(q + 1, "%lf", &t);
                        
                        args.start_time = t * 60 * 60;
                        
                        /*Min*/
                        q = strstr(q + 1, ":");
                        sscanf(q + 1, "%lf", &t);
                        args.start_time += (t * 60);
                        
                        /*Sec*/ 
                        q = strstr(q + 1, ":");
                        sscanf(q + 1, "%lf", &t);
                        args.start_time += t;
                        args.start_time_valid = 1;
                    }
                    else      /*�B�z time*/
                    {
                        args.start_time = 0;
                        args.end_time = 0;
                        time_taken = 1;
                    }
                } 
                else 
                {
                    /*�����clock�A�ϥ��q�{��*/
                    args.start_time = 0;
                    args.end_time = 0;
                }
            } else
            {
                /*snmpte, ������A�ϥ��q�{*/
                args.start_time = 0;
                args.end_time = 0;
            }
        }
        else
        {
            /*�B�znpt*/
            if ((q = strchr(q, '=')) == NULL) 
            {
                send_reply(400, 0, rtsp);/* Bad Request */
                return ERR_NOERROR;
            }
            sscanf(q + 1, "%f", &(args.start_time));          /*�}�l�ɶ�*/
            
            if ((q = strchr(q, '-')) == NULL) 
            {
                send_reply(400, 0, rtsp);/* Bad Request */
                return ERR_NOERROR;
            }
            if (sscanf(q + 1, "%f", &(args.end_time)) != 1)      /*�����ɶ�*/
            {
                args.end_time = 0;                 /*play all the media,until ending ,yanf*/
            }
        }

        /*�d��}�l�^��ɶ�*/
        if ((q = strstr(p, "time")) == NULL) 
        {
            /*�S�����w�A�ߧY�^��*/
            memset(&(args.playback_time), 0, sizeof(args.playback_time));
        } 
        else 
        {
            if (!time_taken) 
            {
                q = strchr(q, '=');
                /*��o�ɶ�*/
               // if (get_UTC_time(&(args.playback_time), q + 1) != ERR_NOERROR)
                {
                    memset(&(args.playback_time), 0, sizeof(args.playback_time));
                }
                args.playback_time_valid = 1;
            }
        }
    } 
    else
    {
        args.start_time = 0;
        args.end_time = 0;
        memset(&(args.playback_time), 0, sizeof(args.playback_time));
    }
    
    /*�ǦC��*/
    if ((p = strstr(rtsp->in_buffer, HDR_CSEQ)) == NULL) 
    {
        send_reply(400, 0, rtsp);   /* Bad Request */
        return ERR_NOERROR;
    }
    
    /* If we get a Session hdr, then we have an aggregate control*/
    if ((p = strstr(rtsp->in_buffer, HDR_SESSION)) != NULL) 
    {
        if (sscanf(p, "%254s %ld", trash, &session_id) != 2) 
        {
            send_reply(454, 0, rtsp);/* Session Not Found */
            return ERR_NOERROR;
        }
    } 
    else 
    {
        send_reply(400, 0, rtsp);/* bad request */
        return ERR_NOERROR;
    }

    
    /*�����XURL */
    if (!sscanf(rtsp->in_buffer, " %*s %254s ", url)) 
    {
        send_reply(400, 0, rtsp);/* bad request */
        return ERR_NOERROR;
    }
    
    /* ����URL�����T��*/
    switch (parse_url(url, server, sizeof(server), &port, object, sizeof(object)))
    {
        case 1: 
            send_reply(400, 0, rtsp);
            return ERR_NOERROR;
            break;
            
        case -1:
             DEBUG_RTSP("500_play!\n");
            send_reply(500, 0, rtsp);
            return ERR_NOERROR;
            break;
            
        default:
            break;
    }



    /*�T��ĥά۹���|�X��*/
    if (strstr(object, "../")) 
    {
        send_reply(403, 0, rtsp);   /* Forbidden */
        return ERR_NOERROR;
    }
    if (strstr(object, "./")) 
    {
        send_reply(403, 0, rtsp); /* Forbidden */
        return ERR_NOERROR;
    }




    /*�d��!*/
#if 0 
    if (q == NULL) 
    {
        /* PLAY <file.sd>*/
        ptr = rtsp->session_list;
        if (ptr != NULL) 
        {
            if (ptr->session_id == session_id) 
            {
                /*�d��RTP session*/ 
                for (ptr2 = ptr->rtp_session; ptr2 != NULL; ptr2 = ptr2->next) 
                {
                   // if (ptr2->current_media->description.priority == 1)
                    {
                        /*����Ҧ��t��*/
                        if (!ptr2->started) 
                        {
                            /*�}�l�s������*/ 
                            DEBUG_RTSP("+++++++++++++++++++++");
                            DEBUG_RTSP("start to play now!");
                            DEBUG_RTSP("+++++++++++++++++++++\n");
                            
                          //  if (schedule_start(ptr2->sched_id, &args) == ERR_ALLOC)
                          //      return ERR_ALLOC;
                        } 
                        else
                        {
                            /*��_�Ȱ��A����*/ 
                            if (!ptr2->pause) 
                            {
                                //fnc_log(FNC_LOG_INFO,"PLAY: already playing\n");
                            } 
                            else 
                            {
                               // schedule_resume(ptr2->sched_id, &args);
                            }
                        }
                    }
                }
            } 
            else 
            {
                send_reply(454, 0, rtsp);	/* Session not found*/
                return ERR_NOERROR;
            }
        } 
        else 
        {
            send_reply(415, 0, rtsp);  /* Internal server error*/
            return ERR_GENERIC;
        }
    }
    else
    {
        if (url_is_file) 
        {
            /*PLAY <file.sd>!<file>    */                     
            ptr = rtsp->session_list;
            if (ptr != NULL) 
            {
                if (ptr->session_id != session_id) 
                {
                    send_reply(454, 0, rtsp);   /*Session not found*/ 
                    return ERR_NOERROR;
                }
                /*�d��RTP�|��*/
				/*
                for (ptr2 = ptr->rtp_session; ptr2 != NULL; ptr2 = ptr2->next) 
                {
                    if (strcmp(ptr2->current_media->filename, q + 1) == 0) 
                    {
                        break;
                    }
                }
                
                if (ptr2 != NULL) 
                {
                   // if (schedule_start(ptr2->sched_id, &args) == ERR_ALLOC)
                     //   return ERR_ALLOC;
                } 
                else 
                {
                    send_reply(454, 0, rtsp);// Session not found
                    return ERR_NOERROR;
                }
			    */
            } 
            else
            {
                send_reply(415, 0, rtsp);	/*Internal server error*/ 
                return ERR_GENERIC;
            }
        } 
        else 
        {
            // PLAY <file.sd>!<aggr>
            ptr = rtsp->session_list;
            if (ptr != NULL) 
            {
                if (ptr->session_id != session_id) 
                {
                    send_reply(454, 0, rtsp);/* Session not found*/
                    return ERR_NOERROR;
                }
                /*����A����Ҧ���RTP*/ 
                for (ptr2 = ptr->rtp_session; ptr2 != NULL; ptr2 = ptr2->next) 
                {
                    if (!ptr2->started)
                    {
                        /*�}�l�s������*/ 
                        DEBUG_RTSP("#----++++++++++++++++--------schedule to playing now --------------+++++++++++\n");
                      //  if (schedule_start(ptr2->sched_id, &args) == ERR_ALLOC)
                        //    return ERR_ALLOC;
                    } 
                    else
                    {
                        /*��_�Ȱ��A����*/ 
                        if (!ptr2->pause) 
                        {
                            /*fnc_log(FNC_LOG_INFO,"PLAY: already playing\n");*/
                        } 
                        else 
                        {
                          //  schedule_resume(ptr2->sched_id, &args);
                        }
                    }
                }
            } 
            else 
            {
                send_reply(415, 0, rtsp);	// Internal server error
                return ERR_GENERIC;
            }
        }
    }
#endif
    /*�o�e�^�_����*/ 
    send_play_reply(rtsp, object, ptr);
    return ERR_NOERROR;
}


/*
****************************************************************
*SETUP ��k���B�z
****************************************************************
*/
int send_setup_reply(RTSP_buffer *rtsp, RTSP_session *session,  RTP_session *rtp_s)
{
    char p[1024];
    u16  body_len;
    char temp[30];
    char ttl[4];
    char *r;

    r=p;
    
    /* �c�ئ^�_���� */
    //sprintf(r, "%s %d %s"RTSP_EL"CSeq: %d"RTSP_EL"Server: %s/%s"RTSP_EL, RTSP_VER, 200, get_stat(200), rtsp->rtsp_cseq, PACKAGE,    R_VERSION);
    sprintf(r, "%s %d %s"RTSP_EL"CSeq: %d"RTSP_EL, RTSP_VER, 200, get_stat(200), rtsp->rtsp_cseq);

    /*�K�[�ɶ��W*/
  //  add_time_stamp(r, 0);
  //  w_pos = strlen(r);

    /*�|��ID*/
   // w_pos += sprintf(r + w_pos, "Session: %d"RTSP_EL, session->session_id);
    
    /*Transport�r�q*/
   sprintf(r + strlen(r), "Transport: ");
    


    sprintf(r + strlen(r), "RTP/AVP;unicast;client_port=%d-%d;", rtp_s->transport.client_ports.RTP, rtp_s->transport.client_ports.RTCP);
    sprintf(r + strlen(r), "server_port=%d-%d;", rtp_s->transport.server_ports.RTP, rtp_s->transport.server_ports.RTCP);        
    sprintf(r + strlen(r), "ssrc=%08x;mode=play", rtp_s->ssrc);       

	
    memcpy(rtp_s->dest_hwaddr,my_hwaddr,6);
    
    
    strcat(r, RTSP_EL);
    sprintf(r + strlen(r), "Session: 53434834C7A0B7729DDEB8F7E5B554"RTSP_EL);
    sprintf(r + strlen(r), "Content-Length: 0"RTSP_EL"Cache-Control: no-cache"RTSP_EL);    /*SDP������*/           
    strcat(r, RTSP_EL);
    /*�g�J�ƾ�*/
   // bwrite(r, (unsigned short) strlen(r), rtsp);
    body_len=strlen(r);
    netconn_write(rtsp->pstConn, (u8 *)p, body_len,NETCONN_COPY);	
  //  rtsp_send((u8 *)p, 20+body_len ,rtsp->nr);
 //   rtsp_conxn[rtsp->nr].my_sequence += body_len;
    return ERR_NOERROR;
    }


int RTSP_setup(RTSP_buffer * rtsp, RTSP_session ** new_session)
{
    /*�����ܶq*/
    char address[16];
    char object[255], server[255];
    char url[255];
    unsigned short port;
    RTSP_session *rtsp_s;
    RTP_session *rtp_s, *rtp_s_prec;
    int SessionID = 0;

    u8 track;
//    struct timeval now_tmp;
    char *p;
    unsigned int start_seq, start_rtptime;
    char transport_str[255];
    media_entry *list, *matching_me, req;
//    struct sockaddr_storage rtsp_peer;
//    socklen_t namelen = sizeof(rtsp_peer);
    unsigned long ssrc;
    unsigned char is_multicast_dad = 1;  
    RTP_transport transport;
    char *saved_ptr, *transport_tkn;
    int max_interlvd;

    /*�ܶq��l��*/
    memset(&req, 0, sizeof(req));
    memset(&transport, 0, sizeof(transport));

    /*�ѪR������J����*/ 

    /* �o��URL */
    if (!sscanf(rtsp->in_buffer, " %*s %254s ", url))
    {
        send_reply(400, 0, rtsp);   /* bad request */        
        return ERR_NOERROR;
    }

	
    /* ����URL���X�k�� */
    switch (parse_url(url, server, sizeof(server), &port, object, sizeof(object))) 
    {
        case 1:                /*�D�k���ШD*/
            send_reply(400, 0, rtsp); 
            return ERR_NOERROR;
            
        case -1:     /*�A�Ⱦ��������~*/
             DEBUG_RTSP("500_setup!\n");
            send_reply(500, 0, rtsp);
            return ERR_NOERROR;
            break;
            
        default:
            break;
    }
   
    if (strstr(url, "trackID=1")) 
    	track =1;
	else if (strstr(url, "trackID=2")) 
		track =2;
	else
    {
        send_reply(400, 0, rtsp);	/* Forbidden */
        return ERR_NOERROR;
    } 

   
    /*�T��ϥά۹���|�W*/
    if (strstr(object, "../")) 
    {

        send_reply(403, 0, rtsp);	/* Forbidden */
        return ERR_NOERROR;
    }    
    if (strstr(object, "./")) 
    {
        send_reply(403, 0, rtsp);   /* Forbidden */
        return ERR_NOERROR;
    }


    /* Get the CSeq */
    if ((p = strstr(rtsp->in_buffer, HDR_CSEQ)) == NULL) 
    {
        send_reply(400, 0, rtsp);       /* Bad Request */
        return ERR_NOERROR;
    }
    else
    {
        if (sscanf(p, "%*s %d", &(rtsp->rtsp_cseq)) != 1) 
        {
            send_reply(400, 0, rtsp);/* Bad Request */
            return ERR_NOERROR;
        }
    }
    
        /*if ((p = strstr(rtsp->in_buffer, "ssrc")) != NULL) {
        p = strchr(p, '=');
        sscanf(p + 1, "%lu", &ssrc);
        } else {*/
        
   // ssrc = random32(0);
		ssrc = 0x11223344;

    /*just for debug,yanf*/  
    /*DEBUG_PRINTF("--------------------------------ssrc got!\n");*/
        //}

    /* Start parsing the Transport header*/
    if ((p = strstr(rtsp->in_buffer, HDR_TRANSPORT)) == NULL) 
    {
        send_reply(406, "Require: Transport settings" /* of rtp/udp;port=nnnn. "*/, rtsp);     /* Not Acceptable */
        return ERR_NOERROR;
    }

    /*�ˬd�ǿ�h�l��O�_���T*/
    if (sscanf(p, "%*10s%255s", transport_str) != 1) 
    {
        DEBUG_RTSP("SETUP request malformed: Transport string is empty\n");
        send_reply(400, 0, rtsp);       /* Bad Request */
        return ERR_NOERROR;
    }

    /*just for debug,yanf*/
    /*DEBUG_PRINTF("+++++++++++++++++++++++++pasering transport list!+++++++++++++++++++++++++++\n");*/


    /*�����r�����Ϊ��ǿ�]�m�C��*/
    if ( !(transport_tkn=strtok_r(transport_str, ",", &saved_ptr)) ) 
    {
       DEBUG_RTSP("Malformed Transport string from client\n");
        send_reply(400, 0, rtsp);/* Bad Request */
        return ERR_NOERROR;
    }

    //transport.type = RTP_no_transport;
   
    do 
    { 
        if ( (p = strstr(transport_tkn, RTSP_RTP_AVP)) ) 
        { 
            /*Transport: RTP/AVP*/ 
            p += strlen(RTSP_RTP_AVP);
            if ( !*p || (*p == ';') || (*p == ' ')) 
            {
#if 0
                // if ((p = strstr(rtsp->in_buffer, "client_port")) == NULL && strstr(rtsp->in_buffer, "multicast") == NULL) {
                if ((p = strstr(transport_tkn, "client_port")) == NULL && strstr(transport_tkn, "multicast") == NULL) {
                send_reply(406, "Require: Transport settings of rtp/udp;port=nnnn. ", rtsp);	/* Not Acceptable */
                return ERR_NOERROR;
                }
#endif 
                /*�漽*/
                if (strstr(transport_tkn, "unicast")) 
                {
                    /*�p�G���w�F�Ȥ�ݺݤf���A��R��������Ӻݤf��*/

                    if( (p = strstr(transport_tkn, "client_port")) ) 
                    {
                        // 2011/07/ 11 JUST DO IT
                    	if(track==1)   //Video
						{
                        	p = strstr(p, "=");
                        	sscanf(p + 1, "%d", &(RTP_session_for_video.transport.client_ports.RTP));
                            p = strstr(p, "-");
                        	sscanf(p + 1, "%d", &(RTP_session_for_video.transport.client_ports.RTCP));
                            
                            RTP_session_for_video.transport.server_ports.RTP  = RTP_DEFAULT_PORT;
            				RTP_session_for_video.transport.server_ports.RTCP = RTP_DEFAULT_PORT+1;							
                   		}
						else if(track==2) //Audio
						{
                        	p = strstr(p, "=");
                        	sscanf(p + 1, "%d", &(RTP_session_for_audio.transport.client_ports.RTP));
                            p = strstr(p, "-");
                        	sscanf(p + 1, "%d", &(RTP_session_for_audio.transport.client_ports.RTCP));
                            
                            RTP_session_for_audio.transport.server_ports.RTP  = RTP_DEFAULT_PORT+2;
            				RTP_session_for_audio.transport.server_ports.RTCP = RTP_DEFAULT_PORT+3;	
                        }
							
                    }


                    /*just for debug,yanf*/
                    /*DEBUG_PRINTF("+++++++++++++++++++++++++begin to send packet!+++++++++++++++++++++++++++\n");*/

                    /*�o�XRTP�ƾڥ]��UDP�s��*/
                   // udp_connect(transport.udp.cli_ports.RTP, &transport.udp.rtp_peer, (*((struct sockaddr_in *) (&rtsp_peer))).sin_addr.s_addr,&transport.rtp_fd);
                    /*�o�XRTCP�ƾڥ]��UDP�s��*/
                    //udp_connect(transport.udp.cli_ports.RTCP, &transport.udp.rtcp_out_peer,(*((struct sockaddr_in *) (&rtsp_peer))).sin_addr.s_addr, &transport.rtcp_fd_out);
                    //udp_open(transport.udp.ser_ports.RTCP, &transport.udp.rtcp_in_peer, &transport.rtcp_fd_in);	//bind

                    //transport.udp.is_multicast = 0;
                } 
               
                //transport.type = RTP_rtp_avp;
                break; 
            } 
           
                
                
        }
    } while ((transport_tkn=strtok_r(NULL, ",", &saved_ptr)));
    
    //DEBUG_GPIU("rtp transport: %d\n", transport.type);    

    /*��������ǿ�����*/
    //if (transport.type == RTP_no_transport) 
    //{
    //    // fnc_log(FNC_LOG_ERR,"Unsupported Transport\n");
    //    send_reply(461, "Unsupported Transport", rtsp);/* Bad Request */
    //    return ERR_NOERROR;
    //}

    /*�p�G���|���Y�A�N���F�@�ӱ���X */ 
    if ((p = strstr(rtsp->in_buffer, HDR_SESSION)) != NULL) 
    {
        if (sscanf(p, "%*s %d", &SessionID) != 1) 
        {
            send_reply(454, 0, rtsp); /* Session Not Found */
            return ERR_NOERROR;
        }
    }
    else
    {
        /*���ͤ@�ӫD0���H�����|�ܧǸ�*/ 
       // gettimeofday(&now_tmp, 0);
      //  srand((now_tmp.tv_sec * 1000) + (now_tmp.tv_usec / 1000));

        SessionID = 1 + (int) (10.0 * rand() / (100000 + 1.0));

        if (SessionID == 0) 
        {
            SessionID++;
        }
    }

    /*�p�G�ݭn�W�[�@�ӷ|��*/ 
    if ( !rtsp->session_list ) 
    {
        rtsp->session_list = (RTSP_session *) calloc(1, sizeof(RTSP_session));
    }
    rtsp_s = rtsp->session_list;

    



    start_seq = 1 + (unsigned int) (rand()%(0xFFFF));
    start_rtptime = 1 + (unsigned int) (rand()%(0xFFFFFFFF));


    /*��g�|�ܤ����ܶq*/
    if (start_seq == 0) 
    {
        start_seq++;
    }
    if (start_rtptime == 0)
    {
        start_rtptime++;
    }
#if 0	
    rtp_s->pause = 1;

    /*�]�m�H���Ʋ��;����ؤl*/
    gettimeofday(&now_tmp, 0);
    srand((now_tmp.tv_sec * 1000) + (now_tmp.tv_usec / 1000));
    
    rtp_s->start_rtptime = start_rtptime;
    rtp_s->start_seq = start_seq;
    memcpy(&rtp_s->transport, &transport, sizeof(transport));
    rtp_s->is_multicast_dad = is_multicast_dad;
    rtp_s->sd_descr=matching_descr;
    rtp_s->sched_id = schedule_add(rtp_s);
    rtp_s->ssrc = ssrc;
	
#endif    

    rtsp_s->session_id = SessionID;
    rtp_s->transport= transport;
    *new_session = rtsp_s;    

    //send_setup_reply(rtsp, rtsp_s , rtp_s);
    if(track==1)   //Video
        send_setup_reply(rtsp, rtsp_s , &RTP_session_for_video);
    else if(track==2) //Audio
        send_setup_reply(rtsp, rtsp_s , &RTP_session_for_audio);
    return ERR_NOERROR;
}


int send_options_reply(RTSP_buffer * rtsp, long cseq)
{
    char p[1024];
    char *r;
    u16 body_len;
	
	r = p;
    sprintf(r, "%s %d %s"RTSP_EL"CSeq: %ld"RTSP_EL, RTSP_VER, 200, get_stat(200), rtsp->rtsp_cseq);
    strcat(r, "Public: OPTIONS,DESCRIBE,SETUP,PLAY,TEARDOWN"RTSP_EL);
    strcat(r, RTSP_EL);
    
    body_len=strlen(r);

    netconn_write(rtsp->pstConn, (u8 *)p, body_len,NETCONN_COPY);		
   // rtsp_send((u8 *)p, 20+body_len ,rtsp->nr);
   // rtsp_conxn[rtsp->nr].my_sequence += body_len;
    return ERR_NOERROR;
}

/*
****************************************************************
*OPTIONS��k���B�z
****************************************************************
*/
int RTSP_options(RTSP_buffer * rtsp)
{
    /*�����ܶq*/
    char *p;
    char trash[255];
    char url[255];
    char method[255];
    char ver[255];
    unsigned int cseq;


    /*�ǦC��*/

    cseq = rtsp->rtsp_cseq;

    sscanf(rtsp->in_buffer, " %31s %255s %31s ", method, url, ver);
    
    send_options_reply(rtsp, cseq);    /*�o�e����*/

    
    return ERR_NOERROR;
}

int send_pause_reply(RTSP_buffer * rtsp, RTSP_session * rtsp_session)
{
    char p[1024];
    char *r;
    u16 body_len;
    char temp[30];	
	
	r = p;

    
    /* �c�ئ^�_����*/
    sprintf(r, "%s %d %s"RTSP_EL"CSeq: %d"RTSP_EL"Server: %s/%s"RTSP_EL, RTSP_VER, 200, get_stat(200), rtsp->rtsp_cseq, PACKAGE,R_VERSION);
   // add_time_stamp(r, 0);
    
    strcat(r, "Session: ");
    sprintf(temp, "%d", rtsp_session->session_id);
    strcat(r, temp);

    strcat(r, RTSP_EL RTSP_EL);
   // bwrite(r, (unsigned short) strlen(r), rtsp);     /*�g�J��w�İϤ�*/

    body_len=strlen(r);
   netconn_write(rtsp->pstConn, (u8 *)p, body_len,NETCONN_COPY);	
   
    //rtsp_send((u8 *)p, 20+body_len ,rtsp->nr);

    return ERR_NOERROR;
}

/*
****************************************************************
*PAUSE��k�B�z
****************************************************************
*/

int RTSP_pause(RTSP_buffer * rtsp)
{
    long int session_id;
    char *p;
    RTSP_session *s;
    RTP_session *r;
    char object[255], server[255], trash[255];
    unsigned short port;
    char url[255];

    send_pause_reply(rtsp, rtsp->session_list);   /*�o�e�����T��*/
  
    return ERR_NOERROR;


    /*�ǦC��*/ 
    if ((p = strstr(rtsp->in_buffer, HDR_CSEQ)) == NULL) 
    {
        send_reply(400, 0, rtsp);   /* Bad Request */
        return ERR_NOERROR;
    } 
    else
    {
        if (sscanf(p, "%254s %d", trash, &(rtsp->rtsp_cseq)) != 2) 
        {
            send_reply(400, 0, rtsp);/* Bad Request */
            return ERR_NOERROR;
        }
    }

    /*���zURL*/
    if (!sscanf(rtsp->in_buffer, " %*s %254s ", url)) 
    {
        send_reply(400, 0, rtsp);/* bad request */
        return ERR_NOERROR;
    }

    /*����URL�����ĩ�*/
    switch (parse_url(url, server, sizeof(server), &port, object, sizeof(object))) 
    {
        case 1:       /* bad request*/
            send_reply(400, 0, rtsp);
            return ERR_NOERROR;
            break;
        
        case -1:    /*internal server error*/ 
              DEBUG_RTSP("500_pause!\n");
            send_reply(500, 0, rtsp);
            return ERR_NOERROR;
            break;
        
        default:
            break;
    }

    /*�T��۹���|*/
//    if (strcmp(server, prefs_get_hostname()) != 0) 
 //   {    
 //   }
    
    if (strstr(object, "../"))
    {
        send_reply(403, 0, rtsp);/* Forbidden */
        return ERR_NOERROR;
    }
    if (strstr(object, "./")) 
    {

        send_reply(403, 0, rtsp); /* Forbidden */
        return ERR_NOERROR;
    }


    /*�|�ܳB�z*/
    if ((p = strstr(rtsp->in_buffer, HDR_SESSION)) != NULL) 
    {
        if (sscanf(p, "%254s %ld", trash, &session_id) != 2)
        {
            send_reply(454, 0, rtsp);  /* Session Not Found */
            return ERR_NOERROR;
        }
    } 
    else 
    {
        session_id = -1;
    }
    s = rtsp->session_list;

    
    if (s == NULL) 
    {
        send_reply(415, 0, rtsp);/* Internal server error*/
        return ERR_GENERIC;
    }
    
    if (s->session_id != session_id)
    {
        send_reply(454, 0, rtsp);   /* Session Not Found */
        return ERR_NOERROR;
    }
    

    
    send_pause_reply(rtsp, s);   /*�o�e�����T��*/

   
    return ERR_NOERROR;
}


