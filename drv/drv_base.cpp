#include "drv_base.h"

drv_base::drv_base()
{
}

drv_base::~drv_base()
{
}

int32 drv_base::init(void)
{
	return 0;
}

int32 drv_base::send(uint32 addr, uint8 *buf, uint32 len)
{
    return 0;
}

int32 drv_base::rece(uint32 addr, uint8 *buf, uint32 len)
{
    return 0;
}
