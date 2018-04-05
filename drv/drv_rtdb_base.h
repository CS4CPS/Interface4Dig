#ifndef DRV_RTDB_BASE_H
#define DRV_RTDB_BASE_H

#include "drv/sysconfig.h"

class drv_rtdb_base
{
public:
    drv_rtdb_base();

    //Standard
    virtual int32 init(void);
    virtual int32 send(uint32 *para, uint8 *buf, int32 len);
    virtual int32 rece(uint32 *para, uint8 *buf, int32 len);

    uint32 maxlen;
    uint8 *datap;


};

#endif // DRV_RTDB_BASE_H
