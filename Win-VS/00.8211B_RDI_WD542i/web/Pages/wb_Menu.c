#include "AsExtern.h"
#include "AsEngine.h"
#include "wb_Menu.h"
#include "web_util.h"


char *sWeb_Menu_Language_Get(void *theServerDataPtr, char *theNamePtr,
		Signed16Ptr theIndexValuesPtr)
{
	int pageString = (int)atoi(theNamePtr);
	int language = fWeb_Language_Get();
	char szTemp[256];

	szTemp[0]=0;

	switch (pageString)
	{
		case 1:
			if (language==0)
			    strcpy(szTemp, "<FRAME src=\"./menu.htm?0\" name=\"m\">");
			else if (language==1)
			    strcpy(szTemp, "<FRAME src=\"./menu.htm?1\" name=\"m\">");
			break;
		case 2:
			if (language==0)
				strcpy(szTemp, "<A href=\"menu.htm?1\">Traditional</A>");
			else if (language==1)
				strcpy(szTemp, "<A href=\"menu.htm?0\">English</A>");
			break;
		case 3:
			if (language==0)
				strcpy(szTemp, "Menu");
			else if (language==1)
				strcpy(szTemp, "���");
			break;
		case 4:
			if (language==0)
				strcpy(szTemp, "Preview");
			else if (language==1)
				strcpy(szTemp, "�w��");
			break;
		case 5:
			if (language==0)
				strcpy(szTemp, "Quick Setting");
			else if (language==1)
				strcpy(szTemp, "�ֳt�]�w");
			break;
		case 6:
			if (language==0)
				strcpy(szTemp, "Channel Control");
			else if (language==1)
				strcpy(szTemp, "�W�D����");
			break;
		case 7:
			if (language==0)
				strcpy(szTemp, "Volume Control");
			else if (language==1)
				strcpy(szTemp, "���q����");
			break;
		case 8:
			if (language==0)
				strcpy(szTemp, "Image Adjustment");
			else if (language==1)
				strcpy(szTemp, "�v���վ�");
			break;
		case 20:
			if (language==0)
				strcpy(szTemp, "General Setting");
			else if (language==1)
				strcpy(szTemp, "�@��]�w");
			break;
		case 21:
			if (language==0)
				strcpy(szTemp, "REC. Setting");
			else if (language==1)
				strcpy(szTemp, "���v�]�w");
			break;
		case 22:
			if (language==0)
				strcpy(szTemp, "Display Setting");
			else if (language==1)
				strcpy(szTemp, "��ܳ]�w");
			break;
		case 23:
			if (language==0)
				strcpy(szTemp, "Image Setting");
			else if (language==1)
				strcpy(szTemp, "�v���]�w");
			break;
		case 24:
			if (language==0)
				strcpy(szTemp, "System Setting");
			else if (language==1)
				strcpy(szTemp, "�t�γ]�w");
			break;
		case 25:
			if (language==0)
				strcpy(szTemp, "Network Setting");
			else if (language==1)
				strcpy(szTemp, "�����]�w");
			break;
		case 26:
			if (language==0)
				strcpy(szTemp, "Schedule Time");
			else if (language==1)
				strcpy(szTemp, "�w���ɶ�");
			break;
		case 27:
			if (language==0)
				strcpy(szTemp, "System Info.");
			else if (language==1)
				strcpy(szTemp, "�t�θ�T");
			break;
		case 28:
			if (language==0)
				strcpy(szTemp, "Event");
			else if (language==1)
				strcpy(szTemp, "�ƥ�");
			break;
		case 29:
			if (language==0)
				strcpy(szTemp, "Playback");
			else if (language==1)
				strcpy(szTemp, "�^��");
			break;
		case 30:
			if (language==0)
				strcpy(szTemp, "Playback List");
			else if (language==1)
				strcpy(szTemp, "�^��M��");
			break;
        case 69:
			if (language==0)
				strcpy(szTemp, "charset=iso-8859-1");
			else if (language==1)
				strcpy(szTemp, "charset=Traditional");
			break;
	}

	HPF(NULL);
	return HPF("%s", szTemp);
}


char *sWeb_Menu_Language_Set(void *theServerDataPtr, char *theNamePtr,
		Signed16Ptr theIndexValuesPtr)
{
	int language=(int)(*(theIndexValuesPtr+0)+1);
	int current_language=fWeb_Language_Get();

	if (current_language!=language)
		fWeb_Language_Set(language);
	return NULL;
}

