#include "drv_LoopBuf.h"

drv_LoopBuf::drv_LoopBuf()
{    
    iput = 0;
    iget = 0;
    prot_n = 0;
}

int drv_LoopBuf::drv_read(char *buf, int len)
{
    int cnt;

    cnt = 0;
    for(int i=0;i<len;i++)
    {
        if(get(&buf[i]) == -1)
            break;
        cnt++;
    }

    return cnt;
}

int drv_LoopBuf::drv_add(char *buf, int len)
{
    int cnt;

    cnt = 0;
    for(int i=0;i<len;i++)
    {
        if(put(buf[i]) == -1)
            break;
        cnt++;
    }
    return cnt;
}

int drv_LoopBuf::addring(int i)
{
    return (i+1) == DRV_LOOPBUF_NMAX ? 0 : i+1;
}

int drv_LoopBuf::get(char *z)
{
    int pos,res;

    res = 1;

    mutex.lock();
    if(prot_n>0)
    {
        pos = iget;
        iget = addring(iget);
        prot_n--;
        *z = buffer[pos];
    }else
    {
        res = -1;
    }
    mutex.unlock();

    return res;
}

int drv_LoopBuf::put(char z)
{
    int res;

    res = 0;

    mutex.lock();
    if(prot_n<DRV_LOOPBUF_NMAX)
    {
        buffer[iput]=z;
        iput = addring(iput);
        prot_n++;        
    }else
        res = -1;

    mutex.unlock();

    return res;
}
