#ifndef DIGINTERFACE_H
#define DIGINTERFACE_H

#include <QWidget>

#include "api/Api.hpp"
#include "modelconfig.h"
#include "drv/sysconfig.h"
#include "drv/drv_rtdb_mem.h"

#include <vector>
#include <string>
#include <iostream>

#include <windows.h>
#include <QtDebug>

// Api
using api::DataObject;
using api::Application;
using api::Api;
using api::Value;

using std::vector;
using std::string;

class DigInterface : public QWidget, public drv_rtdb_mem
{
    Q_OBJECT

public:
    DigInterface();
    ~DigInterface();

    void interfaceInit();
    void interfaceDel();

    int Dig_init(void);
    int Dig_reset(void);
    int Dig_parinit(void);
    float Dig_next(void);
    int Dig_step(float s);
    int Dig_getdata(void);
    int Dig_setevent(void);
    int Dig_del(void);
    void savToRTDB(int cnt, unsigned int savedata);
    unsigned int getOneFromRTDB(int cnt);
    void setLocalTime(float s);
    int Dig_SwitchCtrl(void);
    int getCBase();
    //api::Api* GetInstance(){return apiInstance;}

private:
    DigInterface(const DigInterface&){}

    HINSTANCE dllHandle;
    Api *apiInstance;
    Application *app;
    ModelConfig modelcfg;

    //for cmd
    DataObject *do_CmdInc, *do_CmdSim, *do_IntEvts;

    const Value *relevantObj;

    float LocalTime;
    float RealTime;
    float stepsize;

signals:
    int senddata(QByteArray s);
};

#endif // DIGINTERFACE_H
