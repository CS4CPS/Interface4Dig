#include "diginterface.h"

extern "C" {
typedef api::Api* (__cdecl *CREATEAPI)(const char* username, const char* password);
typedef void (__cdecl *DESTROYAPI)(api::Api*&);
}

#undef GetClassName

DigInterface::DigInterface()
{
    dllHandle = NULL;
    apiInstance = NULL;
}

DigInterface::~DigInterface()
{
    interfaceDel();
}

void DigInterface::interfaceInit()
{
    //create an api instance
    dllHandle = LoadLibraryExA("C:\\DIgSILENT\\pf150\\digapi.dll", NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

    if(!dllHandle){
        emit senddata("handling dll is failed.\n");
        return;
    }
    emit senddata("handling dll is successful.\n");

    CREATEAPI createApi = (CREATEAPI) GetProcAddress ((struct HINSTANCE__*)dllHandle, "CreateApiInstance");
    emit senddata("Creating API instance...\n");
    apiInstance = createApi(NULL, NULL);
    if(apiInstance)
        emit senddata("Creating API instance is successful.\n");
    else
        emit senddata("Creating API instance is failed.\n");
}

void DigInterface::interfaceDel()
{
    //note: destroying the api instance will immediately terminate the whole process as this is the way
    //      how PowerFactory is closed
    if(relevantObj)
        apiInstance->ReleaseValue(relevantObj);

    if(do_CmdInc)
        apiInstance->ReleaseObject(do_CmdInc);

    if(do_CmdSim)
        apiInstance->ReleaseObject(do_CmdSim);

    if(apiInstance)
    {
        emit senddata("Releasing API instance...\n");
        DESTROYAPI destroyApi = (DESTROYAPI) GetProcAddress ((struct HINSTANCE__*)dllHandle, "DestroyApiInstance");
        destroyApi(apiInstance);
        apiInstance = NULL;
        emit senddata("Releasing API instance is successful.\n");
    }
    if(dllHandle)
    {
        emit senddata("Releasing DLL...\n");
        FreeLibrary(dllHandle);
        dllHandle = NULL;
        emit senddata("Releasing DLL is successful.\n");
    }

}

int DigInterface::Dig_init(void)
{
    QByteArray qbatmp;
    int error = 0;
    int res;

    //application
    app = apiInstance->GetApplication();

    //version
    const Value *tmp_version = app->GetVersion();
    qbatmp.clear();
    qbatmp.append(tmp_version->GetString());
    emit senddata("PowerFactory version:"+qbatmp+".\n");
    apiInstance->ReleaseValue(tmp_version);

    //path
    //std::cout << app->GetTempDirectory()->GetString() << std::endl;
    //std::cout << app->GetWorkingDirectory()->GetString() << std::endl;

    //outwindows
    //api::OutputWindow* outWindow = app->GetOutputWindow();
    //outWindow->Print(api::OutputWindow::M_INFO,"Running PowerFactory from the API");
    //outWindow->Clear();

    //user
    DataObject *user = app->GetCurrentUser();

    const Value *tmp_locname = user->GetAttributeString("loc_name",error);
    qbatmp.clear();
    qbatmp.append(tmp_locname->GetString());
    emit senddata("user:"+qbatmp+".\n");
    apiInstance->ReleaseValue(tmp_locname);

    //Active project
    const Value *firstlevel = user->GetChildren(false);
    unsigned int nbChilds = firstlevel->VecGetSize();
    res = 0;
    for(unsigned int i = 0; i < nbChilds; i++)
    {
        DataObject *elm = firstlevel->VecGetDataObject(i);
        const Value *tmp_clsname = elm->GetClassName();
        //std::cout << elm->GetClassName()->GetString()
        //    << "::" << elm->GetName()->GetString()
        //    << "::" << elm->IsDeleted() << std::endl;
        if (strcmp(tmp_clsname->GetString(),"IntPrj")==0)
        {
            if(!elm->IsDeleted())
            {
                Value tmp_prj(elm);
                app->Execute("Activate", &tmp_prj, &error);

                if(error)
                {
                    emit senddata("Activating Project is failed.\n");
                    res = 1;
                }else
                {
                    emit senddata("Activating Project is sucessful.\n");
                    res = 2;
                }
            }
        }
        apiInstance->ReleaseObject(elm);
        apiInstance->ReleaseValue(tmp_clsname);

        if(res)
            break;
    }
    apiInstance->ReleaseObject(user);
    if(res < 2)
    {
        if(res == 0)
            emit senddata("No Project.\n");
        return 1;
    }

    //show proj
    Value cmd("rcom/show");
    app->Execute("ExecuteCmd", &cmd, &error);

    Value tmp_cmd1("ComInc");
    const Value *ComInc = app->Execute("GetCaseObject", &tmp_cmd1, &error);
    do_CmdInc = ComInc->GetDataObject();

    Value tmp_cmd2("ComSim");
    const Value *ComSim = app->Execute("GetCaseObject", &tmp_cmd2, &error);
    do_CmdSim = ComSim->GetDataObject();

    Value tmp_cmd("IntEvt");
    const Value *IntEvt = app->Execute("GetCaseObject", &tmp_cmd, &error);
    do_IntEvts = IntEvt->GetDataObject();

    relevantObj = app->GetCalcRelevantObjects();
    if(relevantObj)
        Dig_parinit();

    const Value *tmp_children = do_IntEvts->GetChildren(false);
    for (size_t i=0, count = tmp_children->VecGetSize(); i < count; ++i)
    {
        DataObject* child = tmp_children->VecGetDataObject(i);
        if(strcmp(child->GetClassName()->GetString(),"EvtSwitch") == 0)
            child->DeleteObject(&error);
        //apiInstance->ReleaseObject(child);
    }

    return 0;
}

int DigInterface::Dig_reset(void)
{
    int error = 0;

    //Value tmp_cmd("ComLdf");
    //const Value *comLdf = app->Execute("GetCaseObject", &tmp_cmd, &error);
    //comLdf->GetDataObject()->Execute("Execute", NULL);
    //apiInstance->ReleaseValue(comLdf);
    int intctrl;

    for(int i=0;i<modelcfg.getCount();i++)
    {
        if(modelcfg.getMC(i) == MODELCONFIG_CTRL)
        {
            if(modelcfg.getType(i) == 0)
            {
                intctrl = modelcfg.getValue(i).toInt();
                modelcfg.getObj(i)->SetAttributeInt("on_off",intctrl,&error);
                //modelcfg.getObj(i)->SetAttributeInt("on_off",intctrl,&error);
            }
        }
    }

    LocalTime = 0;
    RealTime = 0;
    stepsize = 0.05;
    do_CmdInc->Execute("Execute", NULL);

    return 0;
}

float DigInterface::Dig_next(void)
{
    return (LocalTime+stepsize);
}

void DigInterface::setLocalTime(float s)
{
    LocalTime = s;
}

int DigInterface::Dig_step(float s)
{
    int error = 0;

    do_CmdSim->SetAttributeDouble("tstop",s,&error);
    do_CmdSim->Execute("Execute", NULL);

    LocalTime = s;
    //apiInstance->ReleaseObject(tmp_obj);
    //apiInstance->ReleaseValue(ComSim);

    return 0;
}

int DigInterface::Dig_getdata(void)
{
    QByteArray qbatmp,valuetmp,datatmp;
    QByteArray name;
    int error = 0;
    float dbdata;
    int intdata,display;
    unsigned int *savrtdbdata;

    emit senddata("\nNo.\tname\tpar\tvalue\n");
    emit senddata("-----------------------------------------------------\n");

    display = 0;
    name = modelcfg.getpfName(0);
    for(int j=0;j<modelcfg.getCount();j++)
    {
        switch(modelcfg.getType(j))
        {
        case 0:
        case 15:
            intdata = modelcfg.getObj(j)->GetAttributeInt(modelcfg.getpfAddr(j).data());
            datatmp.setNum(intdata);
            savrtdbdata = (unsigned int*)&intdata;
            savToRTDB(j*4, *savrtdbdata);
            display = 1;
            //for test
            //modelcfg.getObj(j)->SetAttributeInt("on_off",0,&error);

            break;
        case 2:
        case 17:
            dbdata = modelcfg.getObj(j)->GetAttributeDouble(modelcfg.getpfAddr(j).data());
            datatmp.setNum(dbdata);
            savrtdbdata = (unsigned int*)&dbdata;
            savToRTDB(j*4, *savrtdbdata);
            display = 1;
            break;
        default:
            display = 0;
            break;
        }

        if(display == 1)
        {
            qbatmp.clear();
            qbatmp.append(valuetmp.setNum(j));
            qbatmp.append("\t");
            qbatmp.append(modelcfg.getpfName(j));
            qbatmp.append("\t");
            qbatmp.append(modelcfg.getpfAddr(j));
            qbatmp.append("\t");
            qbatmp.append(datatmp);
            qbatmp.append("\n");
            emit senddata(qbatmp);
        }
    }

    return 0;
}

void DigInterface::savToRTDB(int cnt, unsigned int savedata)
{
    uint32 par = cnt;
    send(&par, (uint8 *)&savedata, 4);
}

unsigned int DigInterface::getOneFromRTDB(int cnt)
{
    uint32 par = cnt;
    unsigned int recedata;
    rece(&par, (uint8 *)&recedata, 4);
    return recedata;
}

int DigInterface::Dig_parinit(void)
{
    QByteArray qbatmp,valuetmp;
    int error = 0;

    emit senddata("\nNo.\tname\ttypename\n");
    emit senddata("-----------------------------------------------------\n");

    modelcfg.config();
    for(int j=0;j<modelcfg.getCount();j++)
    {
        for(int i=0,count=relevantObj->VecGetSize(&error);i<count;i++)
        {
            DataObject *child = relevantObj->VecGetDataObject(i);
            if(strcmp(child->GetName()->GetString(), modelcfg.getpfName(j).data()) == 0)
            {

                qbatmp.clear();
                qbatmp.append(valuetmp.setNum(j));
                qbatmp.append("\t");
                qbatmp.append(child->GetName()->GetString());
                qbatmp.append("\t");
                qbatmp.append(child->GetClassName()->GetString());
                //qbatmp.append("\t");
                //double volt = child->GetAttributeDouble(modelcfg.getPar(j).data());
                //qbatmp.append(valuetmp.setNum(volt));
                qbatmp.append("\n");
                emit senddata(qbatmp);
                modelcfg.setObj(j,child);
            }
        }
    }

/*
    const Value *tmp_locname = ElmRes->GetDataObject()->GetAttributeString("loc_name",error);
    qbatmp.clear();
    qbatmp.append(tmp_locname->GetString());
    emit senddata("user:"+qbatmp+".\n");
    apiInstance->ReleaseValue(tmp_locname);

    const Value *tmp_children = ElmRes->GetDataObject()->GetChildren(false);
    for (size_t i=0, count = tmp_children->VecGetSize(); i < count; ++i)
    {
        DataObject* child = tmp_children->VecGetDataObject(i);
        const Value *tmp_cname = child->GetClassName();
        const Value *tmp_name = child->GetName();
        qbatmp.clear();
        qbatmp.append("cname:");
        qbatmp.append(tmp_cname->GetString());
        qbatmp.append(",name:");
        qbatmp.append(tmp_name->GetString());
        if (strcmp(tmp_name->GetString(), "ES") == 0)
        {
            qbatmp.append(",value:");
            const Value *tmp_var = child->Execute("GetVar",&Value(0),&error);
            //Value *volt = ;
            qbatmp.append(tmp_var->GetString());
        }
        qbatmp.append(".\n");
        emit senddata(qbatmp);
        apiInstance->ReleaseValue(tmp_cname);
        apiInstance->ReleaseValue(tmp_name);
        apiInstance->ReleaseObject(child);
    }*/
    /*
    //Display bus voltages
    vector<api::DataObject*> elmTerms = GetChildren(projects, "ElmTerm");
    std::cout << "Load flow results / Voltage at buses:" << std::endl;
    for(vector<DataObject*>::const_iterator iter = elmTerms.begin(), end = elmTerms.end(); iter != end; ++iter)
    {
        DataObject* folder = *iter;
        double volt = folder->GetAttributeDouble("m:u");
        string termName = folder->GetAttributeString("loc_name")->GetString();
        std::cout << termName << ": " << volt << " pu" << std::endl;
    }
    */
    //apiInstance->ReleaseValue(tmp_children);
    //apiInstance->ReleaseValue(ElmRes);

    return 0;
}

int DigInterface::Dig_setevent(void)
{
    QByteArray qbatmp,valuetmp;
    int error = 0;

    //const Value *tmp_locname = do_IntEvts->GetAttributeString("loc_name",error);
    //qbatmp.clear();
    //qbatmp.append(tmp_locname->GetString());
    //qbatmp.append("\t");
    //qbatmp.append(do_IntEvts->GetName()->GetString());
    //qbatmp.append(".\n");
    //emit senddata(qbatmp);

    //const Value *tmp_children = tmp_evts->GetChildren(false);
    //for (size_t i=0, count = tmp_children->VecGetSize(); i < count; ++i)
    //{
    //    DataObject* child = tmp_children->VecGetDataObject(i);
    //    qbatmp.clear();
    //    qbatmp.append("cname:");
    //    qbatmp.append(child->GetClassName()->GetString());
    //    qbatmp.append(",name:");
    //    qbatmp.append(child->GetName()->GetString());
    //    qbatmp.append(".\n");
    //    emit senddata(qbatmp);
    //    apiInstance->ReleaseObject(child);
    //}
    int intdata,intctrl;

    for(int i=0;i<modelcfg.getCount();i++)
    { 
        if(modelcfg.getpfName(i).contains("Switch"))
        {
            intdata = modelcfg.getObj(i)->GetAttributeInt(modelcfg.getpfAddr(i).data());
            intctrl = 0;//getOneFromRTDB(modelcfg.getCtrlAddr(i));
            if(intdata != intctrl)
            {
                DataObject *tmp_newevt = do_IntEvts->CreateObject("EvtSwitch", "add_evt");
                tmp_newevt->SetAttributeDouble("time",LocalTime,&error);
                //tmp_newevt->SetAttributeInt("i_allph",1,&error);
                tmp_newevt->SetAttributeObject("p_target",modelcfg.getObj(i),&error);
            }
        }
    }



    //DataObject::AttributeType tmp_type1 = tmp_newevt->GetAttributeType("tottime");
    //qbatmp.setNum(tmp_type1);
    //emit senddata("tottime:"+qbatmp+"\n");

    //DataObject *tmp_evtobj = tmp_newevt->GetAttributeObject("p_target",&error);
    //qbatmp.setNum(tmp_totime);
    //emit senddata("tottime1:"+qbatmp+"\n");

    //tmp_totime = tmp_newevt->GetAttributeInt("i_allph",&error);
    //qbatmp.setNum(tmp_totime);
    //emit senddata("tottime2:"+qbatmp+"\n");
    //tmp_totime = tmp_newevt->GetAttributeInt("i_a",&error);
    //qbatmp.setNum(tmp_totime);
    //emit senddata("tottime3:"+qbatmp+"\n");
    //tmp_totime = tmp_newevt->GetAttributeInt("i_b",&error);
    //qbatmp.setNum(tmp_totime);
    //emit senddata("tottime4:"+qbatmp+"\n");
    //tmp_totime = tmp_newevt->GetAttributeInt("i_c",&error);
    //qbatmp.setNum(tmp_totime);
    //emit senddata("tottime5:"+qbatmp+"\n");

    return 0;
}

int DigInterface::Dig_SwitchCtrl(void)
{
    int error = 0;
    int intctrl;

    for(int i=0;i<modelcfg.getCount();i++)
    {
        if(modelcfg.getMC(i) == MODELCONFIG_CTRL)
        {
            intctrl = getOneFromRTDB(i*4);
            modelcfg.getObj(i)->SetAttributeInt("on_off",intctrl,&error);
        }
    }

    return 0;
}

int DigInterface::Dig_del(void)
{
    int error = 0;

    //show proj
    Value cmd("rcom/hide");
    app->Execute("ExecuteCmd", &cmd, &error);

    return 0;
}

int DigInterface::getCBase()
{
    return modelcfg.getCBase();
}

