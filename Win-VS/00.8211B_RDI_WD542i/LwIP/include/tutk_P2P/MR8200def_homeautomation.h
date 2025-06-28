//
//  MR8200def.h
//  Mars MR8200
//
//  Created by RD1-Gavin on 2014/8/7.
//  Copyright (c) 2014�~ com.tutk. All rights reserved.
//

#ifndef Mars_MR8200_MR8200def_h
#define Mars_MR8200_MR8200def_h
#include "AVIOCTRLDEFs.h"
//----------------------------------
#define room_test        1
#define sensor_test      1
//----------------------------------

#define SENSORNAME_LEN        16

typedef enum {
    mrst_Door           = 0,
    mrst_RemoteCtrl     = 1,
    mrst_PIR            = 2,
    mrst_Vibrate        = 3,
    mrst_Leak           = 4,
    mrst_Smoke          = 5,
    mrst_Gas            = 6,
    mrst_Siren          = 7,
    mrst_Temperature    = 8,
    mrst_HearBeat       = 9,
    mrst_BooldPressure  = 10,
    mrst_PowerPlug		= 11,
    mrst_Panic		    = 12,
    mrst_IAQ			= 13,
    mrst_ADE			= 14,
    mrst_FDS			= 15,
    mrst_SWITCH			= 16,
    mrst_TEMP_HYG		= 17
} emSensorType;

typedef enum {
    mrss_Unknown         = 0,
    mrss_Normal          = 1,
    mrss_Tirggered	     = 2,	// ���Sensor�B��Ĳ�o���A
    mrss_alarm	         = 3,	// ���Siren���A��
	mrss_disalarm	     = 4,	// ���Siren���A��
	mrss_sos	         = 5,	// ���Siren���A��
    mrss_LowPower        = 10,
    mrss_Unavailable     = 20   // ���Sensor�L�k���`�s�u
} emSensorStatus;
// ---------------- �H�UEventID ----------------
#define EventID_Unlink			    0x000
#define EventID_LowBattery			0x001
#define EventID_MotionDetected      0x100
#define EventID_DoorWindowOpen      0x200
#define EventID_DoorWindowClose     0x201
#define EventID_PIRDetected         0x300
#define EventID_TemperatureHigh     0x400
#define EventID_TemperatureLow      0x401
#define EventID_SirenSwitchOn		0x500


// ---------------- �H�U�������P�� ----------------
typedef struct {
    unsigned int nIsOpen;    // 0: Close ; 1: Open
} SDoor;

// ---------------- �H�U��PIR�P�� ----------------
typedef struct {
    unsigned int nReserved;	// �L�N�q,�n��oTrigger���A���emSensorStatus
} SPIR;

// ---------------- �H�U���ū׷P�� ----------------
typedef struct {
	int nTemperature;	// ����
	int nHigh;	        // ����ĵ���]�w��
	int nLow;	        // �C��ĵ���]�w��
			            //�u�Ϋe���byte�A�Ĥ@��byte(Float)�A�ĤG��
			            //byte(Int)�A���byte�ĤK��bit�ҥΨӪ�ܥ��t��(0:����, 1: �t��)
    int nAlarmSwitch;   // bit0: all off; bit 1:����ĵ��; bit 2:�C��ĵ��
} STemperature;

typedef struct {
	u8	Thermometer_H;
	u8	Thermometer_L;
	u8	Hygrometer_H;
	u8	Hygrometer_L;
} STEMP_HYG;


// ---------------- �H�U��Plug�P�� ----------------
typedef struct {
	unsigned int nIsPowerOn;	// Plug�����A 0: Off ; 1: On
	unsigned int nWattage;	// ���plug�˼�
	unsigned int nVoltage;	// ���plug�q��
	unsigned int nCurrent;	// ���plug�q�y
	unsigned char byteHasGalvanometer;	// �O�_�䴩�q�y�p0: false, 1: true
	unsigned char reserved[3];	// �ꢳ������

} SPowerPlug;


// ---------------- �H�U��Siren�P�� ----------------
typedef struct {
	unsigned int nIsRinging;	// Siren�����A 0: Mute ; 1: Ringing 
} SSiren;


typedef struct
{
	unsigned int nSensorID;	        //���ȥ�Device�ݰt�o,�z��������
	unsigned char szName[SENSORNAME_LEN];
	unsigned char byteType;	        // refer to emSensorType
	unsigned char byteStatus;	    // refer to emSensorStatus
	unsigned char bytePushAlarm;	// ����Sensorĵ�ܪ��`�}��,�Y�N�������h�Ҧ�client�N�L�k�����즹Sensor��ĵ��
	unsigned char byteBattery;	    // -1: ���䴩�q�q��ܡA�q�q�ƭȽd��� 0~100
	unsigned char byteSirenAlarm;	// ��ܸ�Sensor�O�_�|Ĳ�oSiren
	unsigned char byteIsHealthType;	// 0: false, 1: true
	unsigned char byteIsSceneAffect;// 0: false, 1: true
	unsigned char byteSameOldID;	//Sean: 20170612 Add.

	union {
		SDoor door;
		SPIR pir;
		STemperature tp;
		SSiren siren;
		SPowerPlug plug;
		STEMP_HYG temp;
		
	} data;

} SSensorStore;


// �H�U��IO command�w�q
//

// ----------------------
// Ū��Sensor�M��
// ----------------------
#define IOTYPE_USER_GETSENSORLST_REQ        0x600
#define IOTYPE_USER_GETSENSORLST_RESP       0x601

typedef struct
{
    unsigned int channel;    // AvServer Index
} SMsgAVIoctrlGetSensorLstReq;

#define MAXSENSOR_NUM_ONCE        20
typedef struct
{
    unsigned int channel;    // AvServer Index
    unsigned int nTotalCount;    // Sensor�`��
    unsigned int nStartIdx;    // �_�lindex
                            // Ex. (1)    channel:0    nTotalCount:15    startIdx:0 ...
                            //     (2)    channel:0    nTotalCount:15    startIdx:10 ...
                            //
    unsigned int nCount;    // sRooms���Ī���
    SSensorStore sSensors[MAXSENSOR_NUM_ONCE];
} SMsgAVIoctrlGetSensorLstResp;

// ----------------------
// �s�WSensor
// ----------------------
#define IOTYPE_USER_SETADDSENSOR_REQ        0x602
#define IOTYPE_USER_SETADDSENSOR_RESP       0x603

typedef struct
{
    unsigned int channel;    // AvServer Index
} SMsgAVIoctrlSetAddSensorReq;

typedef struct
{
    unsigned int channel;    // AvServer Index
    int result;    // �^�ǭ�    0: success; otherwise: failed.
    SSensorStore sSensor;
} SMsgAVIoctrlSetAddSensorResp;

// ----------------------
// �ק�/�s��Sensor
// ----------------------
#define IOTYPE_USER_SETEDITSENSOR_REQ        0x604
#define IOTYPE_USER_SETEDITSENSOR_RESP       0x605

typedef struct
{
    unsigned int channel;    // AvServer Index
    SSensorStore sSensor;
} SMsgAVIoctrlSetEditSensorReq;

typedef struct
{
    int result;    // �^�ǭ�    0: success; otherwise: failed.
    unsigned char reserved[4];
    
} SMsgAVIoctrlSetEditSensorResp;

// ----------------------
// �R��Sensor
// ----------------------
#define IOTYPE_USER_SETDELSENSOR_REQ        0x606
#define IOTYPE_USER_SETDELSENSOR_RESP       0x607

typedef struct
{
    unsigned int channel;    // AvServer Index
    unsigned int nSensorID;
} SMsgAVIoctrlSetDelSensorReq;

typedef struct
{
    int result;    // �^�ǭ�    0: success; otherwise: failed.
    unsigned char reserved[4];
    
} SMsgAVIoctrlSetDelSensorResp;

// ----------------------
// Room�򥻸�Ƶ��c
// ----------------------
#define MAXSENSOR_NUM        32
#define ROOMNAME_LEN         64

typedef struct
{
    int nRoomID;
    unsigned char szName[ROOMNAME_LEN];
    
    unsigned int nSensorCount;
    unsigned int nSensors[MAXSENSOR_NUM];
} SRoomStore;

// ----------------------
// Ū����ĵ�D��Room�M��
// ----------------------
#define IOTYPE_USER_GETROOMLST_REQ        0x60C
#define IOTYPE_USER_GETROOMLST_RESP       0x60D

typedef struct
{
    unsigned int channel;    // AvServer Index
} SMsgAVIoctrlGetRoomLstReq;

#define MAXROOM_NUM_ONCE        5 //�ۦ�q�N
typedef struct
{
    unsigned int channel;    // AvServer Index
    unsigned int nTotalCount;    // Room�`��
    unsigned int nStartIdx;    // �_�lindex
    unsigned int nCount;    // sRooms���Ī���
    SRoomStore sRooms[MAXROOM_NUM_ONCE];
} SMsgAVIoctrlGetRoomLstResp;
//P.S.�YRoom�`�ƶW�LMAXROOM_NUM_ONCE�ƭ�,�h�@����REQ�|�o�ͦh����RESP

// ----------------------
// �s�WRoom
// ----------------------
#define IOTYPE_USER_SETADDROOM_REQ        0x60E
#define IOTYPE_USER_SETADDROOM_RESP       0x60F

typedef struct
{
    unsigned int channel;    // AvServer Index
    SRoomStore sRoom;    // �o�䪺SRoomStore. nRoomID�]��-1
} SMsgAVIoctrlSetAddRoomReq;
typedef struct
{
    unsigned int channel;    // AvServer Index
    SRoomStore sRoom;    // �o�䪺SRoomStore. nRoomID�|�^�Ǧ��Ī���(>=0)
} SMsgAVIoctrlSetAddRoomResp;

// ----------------------
// �ק�/�s��Room
// ----------------------
#define IOTYPE_USER_SETEDITROOM_REQ     0x610
#define IOTYPE_USER_SETEDITROOM_RESP    0x611

typedef struct
{
    unsigned int channel;    // AvServer Index
    SRoomStore sRoom;
} SMsgAVIoctrlSetEditRoomReq;

typedef struct
{
    int result;    // �^�ǭ�    0: success; otherwise: failed.
    unsigned char reserved[4];
    
} SMsgAVIoctrlSetEditRoomResp;

// ----------------------
// �R��Room
// ----------------------
#define IOTYPE_USER_SETDELROOM_REQ        0x612
#define IOTYPE_USER_SETDELROOM_RESP       0x613

typedef struct
{
    unsigned int channel;    // AvServer Index
    int nRoomID;
} SMsgAVIoctrlSetDelRoomReq;

typedef struct
{
    int result;    // �^�ǭ�    0: success; otherwise: failed.
    unsigned char reserved[4];
    
} SMsgAVIoctrlSetDelRoomResp;

// ----------------------
// �O�_�䴩IP Camera
// ----------------------
#define IOTYPE_USER_GETIPCAMSUPPORT_REQ     0x614
#define IOTYPE_USER_GETIPCAMSUPPORT_RESP    0x615

typedef struct
{
    unsigned int channel;    // AvServer Index
} SMsgAVIoctrlGetIPCamSupportReq;

#define MAXROOM_NUM_ONCE        5
typedef struct
{
    unsigned int channel;    // AvServer Index
    unsigned int nIsSupport;    // 0: Not support ; 1: Support
    
} SMsgAVIoctrlGetIPCamSupportResp;

// ----------------------
// Ū���S�wSensor����T
// ----------------------
#define IOTYPE_USER_GETSENSOR_REQ       0x616
#define IOTYPE_USER_GETSENSOR_RESP      0x617

typedef struct
{
    unsigned int channel;    // AvServer Index
    unsigned int nSensorID;
} SMsgAVIoctrlGetSensorReq;

typedef struct
{
    int result;    // �^�ǭ�    0: success; otherwise: failed.
    SSensorStore sSensor;
} SMsgAVIoctrlGetSensorResp;

// ----------------------
// Scene�򥻸�Ƶ��c
// ----------------------
#define SCENENAME_LEN        64

typedef struct
{
    unsigned int nSceneID;
    unsigned char szName[SCENENAME_LEN];
} SSceneHead;

// ----------------------
// Ū����ĵ�D��Scene�M��
// ----------------------
#define IOTYPE_USER_GETSCENCELST_REQ        0x618
#define IOTYPE_USER_GETSCENCELST_RESP       0x619

typedef struct
{
    unsigned int channel;    // AvServer Index
} SMsgAVIoctrlGetSceneLstReq;

#define MAXSCENE_NUM_ONCE        12
typedef struct
{
    unsigned int channel;    // AvServer Index
    unsigned int nTotalCount;    // Scene�`��
    unsigned int nStartIdx;    // �_�lindex
    unsigned int nCount;    // sSceneHead���Ī���
    SSceneHead sSceneHead[MAXSCENE_NUM_ONCE];
} SMsgAVIoctrlGetSceneLstResp;

//P.S.�YScene�`�ƶW�LMAXSCENE_NUM_ONCE�ƭ�,�h�@����REQ�|�o�ͦh����RESP

// ----------------------
// �s�WScene - �s�WSceneHead
// ----------------------
#define IOTYPE_USER_SETADDSCENEHEAD_REQ    0x61A
#define IOTYPE_USER_SETADDSCENEHEAD_RESP   0x61B

typedef struct
{
    unsigned int channel;    // AvServer Index
    SSceneHead sScene;    // �o�䪺SSceneHead. nSceneID�]��-1
} SMsgAVIoctrlSetAddSceneHeadReq;

typedef struct
{
    unsigned int channel;    // AvServer Index
    SSceneHead sScene;    // �o�䪺SSceneHead. nSceneID�|�^�Ǧ��Ī���(>=0)
} SMsgAVIoctrlSetAddSceneHeadResp;

// ----------------------
// Ū��Scene���e (All sensors in Scene)
// ----------------------
#define IOTYPE_USER_GETSCENE_REQ    0x61C
#define IOTYPE_USER_GETSCENE_RESP   0x61D

typedef struct
{
    unsigned int channel;    // AvServer Index
    unsigned int nSceneID;    
} SMsgAVIoctrlGetSceneReq;

//#define MAXSENSOR_NUM_ONCE        10
typedef struct
{
    unsigned int channel;    // AvServer Index
    unsigned int nTotalCount;    // �o��Scene�U���Ҧ�Sensor�`��
    unsigned int nStartIdx;    // �_�lindex
    unsigned int nCount;    // sSensors���Ī���
    SSensorStore sSensors[MAXSENSOR_NUM_ONCE];
} SMsgAVIoctrlGetSceneResp;

//P.S.�YSensor�`�ƶW�LMAXSENSOR_NUM_ONCE�ƭ�,�h�@����REQ�|�o�ͦh����RESP typedef 

// ----------------------
// �ק�/�s��Scene�W��
// ----------------------
#define IOTYPE_USER_SETEDITSCENEHEAD_REQ    0x61E
#define IOTYPE_USER_SETEDITSCENEHEAD_RESP   0x61F

typedef struct
{
    unsigned int channel;    // AvServer Index
    SSceneHead sScene;
} SMsgAVIoctrlSetEditSceneHeadReq;

typedef struct
{
    int result;    // �^�ǭ�    0: success; otherwise: failed.
    unsigned char reserved[4];

} SMsgAVIoctrlSetEditSceneHeadResp;

// ----------------------
// �ק�/�s��Scene (All sensors in Scene)
// ----------------------
#define IOTYPE_USER_SETEDITSCENE_REQ    0x620
#define IOTYPE_USER_SETEDITSCENE_RESP   0x621

//#define MAXSENSOR_NUM_ONCE        10
typedef struct
{
    unsigned int channel;    // AvServer Index
    unsigned int nSceneID;    // ���]�w��SceneID
    unsigned int nTotalCount;    // �o��Scene�U���Ҧ�Sensor�`��
    unsigned int nStartIdx;    // �_�lindex
    unsigned int nCount;    // sSensors���Ī���
    SSensorStore sSensors[MAXSENSOR_NUM_ONCE];
} SMsgAVIoctrlSetEditSceneReq;

typedef struct
{
    int result;    // �^�ǭ�    0: success; otherwise: failed.
    int nStartIdx;    // �P SMsgAVIoctrlSetEditSceneReq �̪� nStartIdx
} SMsgAVIoctrlSetEditSceneResp;

// ----------------------
// �R��Scene
// ----------------------
#define IOTYPE_USER_SETDELSCENE_REQ    0x622
#define IOTYPE_USER_SETDELSCENE_RESP   0x623

typedef struct
{
    unsigned int channel;    // AvServer Index
    unsigned int nSceneID;
} SMsgAVIoctrlSetDelSceneReq;

typedef struct
{
    int result;    // �^�ǭ�    0: success; otherwise: failed.
    unsigned char reserved[4];

} SMsgAVIoctrlSetDelSceneResp;

// ----------------------
// ����Scene
// ----------------------
#define IOTYPE_USER_PLAYSCENE_REQ       0x626
#define IOTYPE_USER_PLAYSCENE_RESP      0x627

typedef struct
{
	unsigned int channel;	// AvServer Index
	unsigned int nSceneID;
} SMsgAVIoctrlPlaySceneReq;

typedef struct
{
	int result;	// �^�ǭ�	0: success; otherwise: failed.
	unsigned char reserved[4];
} SMsgAVIoctrlPlaySceneResp;


// ----------------------
// Ū���S�wSensor��log
// ----------------------
#define IOTYPE_USER_GETSENSORLOG_REQ    0x624
#define IOTYPE_USER_GETSENSORLOG_RESP   0x625

typedef struct
{
    unsigned int channel;    // AvServer Index
    unsigned int nSensorID;
    unsigned int nStartIdx;    // ���w�_�lindex
 } SMsgAVIoctrlGetSensorLogReq;

// ----------------------
// ����ĵ��
// ----------------------
#define IOTYPE_USER_STOPALARM_REQ       0x628
#define IOTYPE_USER_STOPALARM_RESP      0x629

typedef struct
{
	unsigned char reserved[4];

} SMsgAVIoctrlStopAlarmReq;

typedef struct
{
	int result;	// �^�ǭ�	0: success; otherwise: failed.
	unsigned char reserved[4];
} SMsgAVIoctrlStopAlarmResp;

/*typedef struct
{
    unsigned short year;    // The number of year.
    unsigned char month;    // The number of months since January, in the range 1 to 12.
    unsigned char day;        // The day of the month, in the range 1 to 31.
    unsigned char wday;        // The number of days since Sunday, in the range 0 to 6. (Sunday = 0, Monday = 1, ...)
    unsigned char hour;     // The number of hours past midnight, in the range 0 to 23.
    unsigned char minute;   // The number of minutes after the hour, in the range 0 to 59.
    unsigned char second;   // The number of seconds after the minute, in the range 0 to 59.
}STimeDay;*/
 // define in AVIOCTRLDEFs.h
typedef struct
{
    STimeDay time;    // Event time
    unsigned int nPreDefEventID;    // EventID define in SensorEnentStringTable.csv
    unsigned char szName[SENSORNAME_LEN];	//Triggered sensor name
} SSensorLogRecord;

#define MAX_LOGRECORD_NUM    20
typedef struct
{
    unsigned int channel;    // AvServer Index
    unsigned int nSensorID;    // ����Sensor��ID
    unsigned int nTotalCount; // Log record �`��
    unsigned int nStartIdx; // �_�lindex
    unsigned int nCount;    // sRecords���Ī���
    SSensorLogRecord sRecords[MAX_LOGRECORD_NUM];
} SMsgAVIoctrlGetSensorLogResp;


typedef struct
{
    STimeDay time;    // Event time
    unsigned int nPreDefEventID;    // EventID define in SensorEnentStringTable.csv
    unsigned int nSensorType;    // ����Sensor��Type    
    unsigned char szName[SENSORNAME_LEN];	//Triggered sensor name
} HOMERF_SensorLogRecord;

#define MAX_UI_LOGRECORD_NUM    7
#define MAX_UI_LOGRECORD_DAY    7

typedef struct
{
    unsigned int channel;    // AvServer Index
    unsigned int nTotalCount; // Log record �`��
    unsigned int nStartIdx; // �_�lindex
    unsigned int nCount;    // sRecords���Ī���
    HOMERF_SensorLogRecord sRecords[MAX_UI_LOGRECORD_NUM];
} HOMERF_SensorLogList;

typedef struct
{
    STimeDay time;    // Event time
} HOMERF_SensorLogDayRecord;

typedef struct
{
    unsigned int nTotalCount; // Log record �`��
    unsigned int nStartIdx; // �_�lindex
    unsigned int nCount;    // sRecords���Ī���
    HOMERF_SensorLogDayRecord sRecords[MAX_UI_LOGRECORD_NUM];
} HOMERF_SensorLogDayList;


#endif
