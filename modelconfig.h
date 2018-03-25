#ifndef MODELCONFIG_H
#define MODELCONFIG_H

#include "api/Api.hpp"

#include <vector>
#include <string>
#include <iostream>

#include <QByteArray>
#include "hal_qt/drv_xml.h"

// Api
using api::DataObject;
using api::Application;
using api::Api;
using api::Value;

typedef struct rect
{
    QByteArray  Name;
    int         DevId;
    QByteArray  pfName;
    QByteArray  pfAddr;
    int         Type;
    QByteArray  Value;
    DataObject  *Objp;

}cfgInfo;

typedef struct{
    int iedBase;
    int iedCnt;
    int iedCount;
    int swTotalCount;
    int swControlBase;
    int swControlCnt;
    int parcount;

    int Tmslen, Tctlen, Tlen;

    cfgInfo Mbuf[20480];
    cfgInfo Cbuf[2048];
}ModelInfo;

#define MODELCONFIG_MEASURE     1
#define MODELCONFIG_PARA        2
#define MODELCONFIG_CTRL        3

class ModelConfig
{
public:
    ModelConfig();
    ~ModelConfig();

    int getaddress(const Value *relevantObj);

    void preOrder(QDomNode root, QString name);
    int config();

    QByteArray getpfName(int cnt);
    QByteArray getpfAddr(int cnt);
    int getType(int cnt);
    QByteArray getValue(int cnt);
    void setObj(int cnt, DataObject *Objp);
    DataObject *getObj(int cnt);
    int getCount();
    int getCBase();
    int getMC(int cnt);

    //cfgInfo cfgInfobuf[512];
    //int parcount;
    //int iedBase,iedCnt,iedCount,swTotalCount,swControlBase,swControlCnt;
    //int *swCount;
    ModelInfo inst_MT;
    drv_xml inst_xml;
    int *swCount;
};

#endif // MODELCONFIG_H
