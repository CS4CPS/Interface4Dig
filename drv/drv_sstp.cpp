#include "drv_sstp.h"

#include "stdio.h"

drv_sstp::drv_sstp()
{
}

int32 drv_sstp::init(int32 setmod, int32 setaddr)
{
    status = 0;
    head = 0;
    tail = 0;
    cnt = 0;
    tlen = 0;
    tailcut = 0;
    flag = 0;
    master = setmod;
    redeal = 0;

    return 0;
}

int drv_sstp::fill(uint8 *buf,drv_sstp_head *head, uint8 *data)
{
	int i,j;
	uint8 sum;

	buf[0] = SSTP_HEAD_ID;
	buf[1] = head->sor;
	buf[2] = head->des;
	buf[3] = head->type;
    if(master == DRV_SSTP_MASTER)
        buf[4] = flag++;
    else
        buf[4] = head->flag;
    buf[5] = head->len;
	for(i=0;i<head->len;i++)
        buf[6+i] = data[i];
	
	sum = 0;
    for(j=0;j<(6+head->len);j++)
		sum += buf[j];
    buf[6+head->len] = sum;
    buf[6+head->len+1] = SSTP_TAIL_ID;

    return (8+head->len);
}

int drv_sstp::resolve(uint8 *buf, int32 len)
{
	int res,i,tmp,lent,count,idt;
	uint8 *bufp,sum;

	if(redeal == 1)
		redeal = 0;

    tmp = SSTP_NMAX_BUF - cnt;
	if(len<tmp)
		lent = len;
	else
	{
		lent = tmp;
        tailcut = 1;
	}
		
    tmp = tail+lent-SSTP_NMAX_BUF;
	if(tmp<0)
	{
		for(i=0;i<lent;i++)
            inbuf[tail+i] = buf[i];
        tail += lent;
	}else
	{
		for(i=0;i<(lent-tmp);i++)
            inbuf[tail+i] = buf[i];
		for(;i<lent;i++)
            inbuf[i-lent+tmp] = buf[i];
        tail = tmp;
	}
    cnt += lent;

    count = cnt;
	res = 0;
	while(count)
	{
        switch(status)
		{
		case 0:
            bufp = &inbuf[head];
			if(*bufp==SSTP_HEAD_ID)
                status = 1;
			else
			{
                head = drv_sstp_move(head,1);
				count--;
			}
			break;
		case 1:
            if(count>=8)
                status = 2;
			else
			{
                if(tailcut==1)
				{
                    tailcut=0;
                    status = 0;
                    head = drv_sstp_move(head,1);
					count--;					
				}else
					res = -1;	
			}
			break;
		case 2:
            idt = drv_sstp_move(head,5);
            bufp = &inbuf[idt];
            tlen = *bufp + 8;
            if(tlen < SSTP_NMAX)
                status = 3;
			else
			{
                status = 0;
                head = drv_sstp_move(head,1);
				count--;
			}
			break;
		case 3:
            if(count>=tlen)
                status = 4;
			else
                if(tailcut==1)
				{
                    tailcut=0;
                    status = 0;
                    head = drv_sstp_move(head,1);
					count--;					
				}else
					res = -1;	
			break;
		case 4:
            idt = drv_sstp_move(head,tlen-1);
            bufp = &inbuf[idt];
			if(*bufp==SSTP_TAIL_ID)
                status = 5;
			else
			{
                status = 0;
                head = drv_sstp_move(head,1);
				count--;
			}
			break;
		case 5:
			sum = SSTP_HEAD_ID;

            bufp = &inbuf[drv_sstp_move(head,1)];
            headbuf.sor = *bufp;
			sum += *bufp;
            bufp = &inbuf[drv_sstp_move(head,2)];
            headbuf.des = *bufp;
			sum += *bufp;
            bufp = &inbuf[drv_sstp_move(head,3)];
            headbuf.type = *bufp;
			sum += *bufp;
            bufp = &inbuf[drv_sstp_move(head,4)];
            headbuf.flag = *bufp;
            sum += *bufp;
            bufp = &inbuf[drv_sstp_move(head,5)];
            headbuf.len = *bufp;
			sum += *bufp;

            for(i=0;i<(tlen-8);i++)
			{
                idt = drv_sstp_move(head,i+6);
                outbuf[i] = inbuf[idt];
                sum += inbuf[idt];
			}

            idt = drv_sstp_move(head,tlen-2);
            bufp = &inbuf[idt];
			if(*bufp==sum)
			{
                status = 0;
                head = drv_sstp_move(head,tlen);
                count-=tlen;
                if(count>0)
                	redeal = 1;
				res = 1;
			}else
			{
                status = 0;
                head = drv_sstp_move(head,1);
				count--;
			}
			break;
		}
		
		if(res)
			break;	
	}
    cnt = count;

	return res;
}

uint16 drv_sstp::drv_sstp_move(short i,short pave)
{
	int res;

	res = i+pave;
	if(res>=SSTP_NMAX_BUF)
		res -= SSTP_NMAX_BUF;
    
	return res;
}
