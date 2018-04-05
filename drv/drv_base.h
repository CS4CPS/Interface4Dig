#ifndef DRV_BASE_H
#define DRV_BASE_H

#include "sysconfig.h"

class drv_base
{
public:
    drv_base();
    ~drv_base();

    virtual int32 init(void);
    virtual int32 send(uint32 addr, uint8 *buf, uint32 len);
    virtual int32 rece(uint32 addr, uint8 *buf, uint32 len);

};

#endif // DRV_BASE_H
