#ifndef DRV_RTDB_MEM_H
#define DRV_RTDB_MEM_H

#include "drv_rtdb_base.h"

#define     DRV_RTDB_MEM_ADDR      0

class drv_rtdb_mem:public drv_rtdb_base
{        
public:
    drv_rtdb_mem();

    //Standard
    int32 init(void);
    int32 send(uint32 *para, uint8 *buf, int32 len);
    int32 rece(uint32 *para, uint8 *buf, int32 len);
};

#endif // DRV_PROT_MEM_H
