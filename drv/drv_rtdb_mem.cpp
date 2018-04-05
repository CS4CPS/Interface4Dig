#include "drv_rtdb_mem.h"

#include "stdio.h"

drv_rtdb_mem::drv_rtdb_mem()
{
}

int drv_rtdb_mem::init(void)
{
    return 0;
}

int32 drv_rtdb_mem::send(uint32 *para, uint8 *buf, int32 len)
{
    int32 i,res;
    uint32 addr;

    addr = para[DRV_RTDB_MEM_ADDR];

    res = 0;
    for(i=0;i<len;i++)
    {
        if(addr+i<maxlen)
        {
            datap[addr+i] = buf[i];
            res++;
        }else
            break;
    }

    return res;
}

int32 drv_rtdb_mem::rece(uint32 *para, uint8 *buf, int32 len)
{
    int32 i;
    uint32 addr;
    addr = para[DRV_RTDB_MEM_ADDR];

    for(i=0;i<len;i++)
    {
        if(addr+i<maxlen)
            buf[i] = datap[addr+i];
        else
            break;
    }

    return i;
}
