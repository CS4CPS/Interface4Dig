#ifndef DRV_LOOPBUF_H
#define DRV_LOOPBUF_H

#include "drv/sysconfig.h"

#include <QMutex>

#define 	DRV_LOOPBUF_NMAX					1024

class drv_LoopBuf
{
public:
    drv_LoopBuf();

    int drv_read(char *buf, int len);
    int drv_write(char *buf, int len);
    int drv_add(char *buf, int len);

    int addring(int i);
    int get(char *z);
    int put(char z);

private:
    char buffer[DRV_LOOPBUF_NMAX];
    int iput,iget,prot_n;

    QMutex mutex;
};

#endif // DRV_LOOP_BUF_H
