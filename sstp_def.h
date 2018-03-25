#ifndef SSTP_DEF_H
#define SSTP_DEF_H

#define     ID_COSIMMANAGER         0
#define     ID_COMPOWERFFACTORY     1
#define     ID_COMOMNETPP           2

#define     ID_POWERFACTORY         11
#define     ID_OMNETPP              12

#define     ID_COMIED               20

//CMD for both
#define		SSTP_DEF_CMD_STOP		0x10        //common
#define		SSTP_DEF_CMD_RUN		0x11        //common
#define		SSTP_DEF_CMD_INIT		0x12        //common
#define		SSTP_DEF_CMD_RST		0x13

#define     SSTP_DEF_PAR_NEXT       0x31
#define     SSTP_DEF_PAR_STEP       0x32
#define     SSTP_DEF_PAR_PROCESS    0x33

//CMD for ARM
//#define		SSTP_DEF_CMD_CAL		0x20
#define		SSTP_DEF_CMD_UPDATE		0x21		//P1-data zone, P2P3-time;
//#define		SSTP_DEF_CMD_SAVPAR     0x22
#define		SSTP_DEF_CMD_DEFPAR     0x23		//load default par
//#define		SSTP_DEF_CMD_ATTADJ     0x24		//att adj mod
//#define		SSTP_DEF_CMD_ATTCAL     0x25		//att normal mod
//#define		SSTP_DEF_CMD_ALMCLR     0x26		//clr alarm reg

#define		SSTP_DEF_CMD_SWITCHPRE  0x30		//P1-switch ID(1~4), p2-act(0-off,1-on)
#define		SSTP_DEF_CMD_SWITCH     0x31		//P1-switch ID(1~4), p2-act(0-off,1-on)
//#define		SSTP_DEF_CMD_DIGIO		0x31		//P1-switch ID(1~4), p2-act(0-off,1-on)
//#define		SSTP_DEF_CMD_BREAKER	0x32		//P1-switch ID(1~4), p2-act(0-off,1-on),
													//p3(15:0)-alarm time, p3(31:16)-off time
//#define		SSTP_DEF_CMD_PPOWER	0x33            //p2-act(0-off,1-on), p3-protect time

#endif
