#ifndef DRV_SSTP_H
#define DRV_SSTP_H

#include "sysconfig.h"

#define 	SSTP_NMAX			228
//#define 	SSTP_NMAX_HEAD		7
//#define 	SSTP_NMAX_DATA		200
#define 	SSTP_NMAX_BUF		512

#define		SSTP_HEAD_ID		0x68
#define		SSTP_TAIL_ID		0x16

//TYPE
#define		DRV_SSTP_CMD_CGET		0x11
#define		DRV_SSTP_CMD_RDATA		0x14
#define		DRV_SSTP_CMD_CSET		0x31
#define		DRV_SSTP_CMD_WDATA		0x34

#define		DRV_SSTP_IDXDS          4
#define		DRV_SSTP_IDXDL          16

//cset
#define		SQUEUE_DEF_CMD_POS		0

//rw data
#define		DRV_SSTP_DATA_BLK		0
#define		DRV_SSTP_DATA_ADDR		1
#define		DRV_SSTP_DATA_LEN		2
#define		DRV_SSTP_DATA_PAR		3

//rw evt
#define		DRV_SSTP_EVT_CLASS		0
#define		DRV_SSTP_EVT_ADDR		1
#define		DRV_SSTP_EVT_LEN		2

//response
#define		DRV_SSTP_RSP_POS		0
#define		DRV_SSTP_RSP_OK 		1
#define		DRV_SSTP_RSP_ERR		10

#define     DRV_SSTP_MASTER           1
#define     DRV_SSTP_SLAYER           2



typedef struct{
	uint8 sor;
	uint8 des;
	uint8 type;
    uint8 flag;
	uint8 len;
}drv_sstp_head;

class drv_sstp
{
public:
    drv_sstp();

    int32 init(int32 setmod, int32 setaddr);

    //int32 fill(uint8 *buf,modbus_rd_send head);
    //int32 fill(uint8 *buf,modbus_rd_rece head,uint16 data[]);
    //int32 fill(uint8 *buf,modbus_wr_send head,uint16 data[]);

    int32 resolve(uint8 *buf,int32 len);
    uint16 crc16(uint8 *puchMsg, int32 usDataLen);

    int32 fill(uint8 *buf,drv_sstp_head *head, uint8 *data);
    uint16 drv_sstp_move(short i, short pave);

    drv_sstp_head headbuf;

    uint8 data[SSTP_NMAX_BUF];
    uint8 inbuf[SSTP_NMAX_BUF];
    uint8 outbuf[SSTP_NMAX_BUF];

    short status;
    short head;
    short tail;
    short cnt;
    short tlen;
    short tailcut;

    uint8 flag;
    uint32 master;

    uint32 redeal;
};

#endif
