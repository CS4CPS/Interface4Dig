#include "modelconfig.h"

#include <QDebug>

ModelConfig::ModelConfig()
{

}

ModelConfig::~ModelConfig()
{
    if(swCount!=NULL)
        delete swCount;
}

void ModelConfig::preOrder(QDomNode root, QString name)
{

    //if(root!=NULL)
    //{
        //cout<<root->data<<" ";
        //preOrder(root->lchild);
        //preOrder(root->rchild);
    //}
}

int ModelConfig::config()
{
    QDomNode IEDNode,swNode,Nodetmp;
    inst_xml.init("C:/ModelConfig.xml");
    IEDNode = inst_xml.findNode(inst_xml.getRootNode(), "GENERAL", "");
    inst_MT.iedCount = inst_xml.GetValue(IEDNode, "IEDCount", "").toInt();
    swCount = new int[inst_MT.iedCount];

    QString numtmp;
    int i,j,k,iedcnt,swcnt;

    int mcnt = 0;
    int ccnt = 0;
    for(i=0;i<inst_MT.iedCount;i++)
    {
        IEDNode = inst_xml.findNode(inst_xml.getRootNode(), "CIED", numtmp.setNum(i));

        iedcnt = inst_xml.GetCount(IEDNode, "Measure");
        for(j=0;j<iedcnt;j++)
        {
            inst_MT.Mbuf[mcnt].Name = inst_xml.GetValue(IEDNode, "Name", "").toLatin1();
            inst_MT.Mbuf[mcnt].DevId = inst_xml.GetValue(IEDNode, "DevId", "").toInt();
            inst_MT.Mbuf[mcnt].pfName = inst_xml.GetValue(IEDNode, "pfName", "").toLatin1();
            Nodetmp = inst_xml.findNode(IEDNode, "Measure", numtmp.setNum(j));
            inst_MT.Mbuf[mcnt].pfAddr = inst_xml.GetAttribute(Nodetmp, "pfAddr").toLatin1();
            inst_MT.Mbuf[mcnt].Type = inst_xml.GetAttribute(Nodetmp, "Type").toInt();
            inst_MT.Mbuf[mcnt].Value = inst_xml.GetValue(Nodetmp).toLatin1();
            mcnt++;
        }

        iedcnt = inst_xml.GetCount(IEDNode, "Ctrl");
        for(j=0;j<iedcnt;j++)
        {
            inst_MT.Cbuf[ccnt].Name = inst_xml.GetValue(IEDNode, "Name", "").toLatin1();
            inst_MT.Cbuf[ccnt].DevId = inst_xml.GetValue(IEDNode, "DevId", "").toInt();
            inst_MT.Cbuf[ccnt].pfName = inst_xml.GetValue(IEDNode, "pfName", "").toLatin1();
            Nodetmp = inst_xml.findNode(IEDNode, "Ctrl", numtmp.setNum(j));
            inst_MT.Cbuf[ccnt].pfAddr = inst_xml.GetAttribute(Nodetmp, "pfAddr").toLatin1();
            inst_MT.Cbuf[ccnt].Type = inst_xml.GetAttribute(Nodetmp, "Type").toInt();
            inst_MT.Cbuf[ccnt].Value = inst_xml.GetValue(Nodetmp).toLatin1();
            ccnt++;
        }

        iedcnt = inst_xml.GetCount(IEDNode, "Switch");
        for(j=0;j<iedcnt;j++)
        {
            swNode = inst_xml.findNode(IEDNode, "Switch", numtmp.setNum(j));

            swcnt = inst_xml.GetCount(swNode, "Measure");
            for(k=0;k<swcnt;k++)
            {
                inst_MT.Mbuf[mcnt].Name = inst_xml.GetValue(IEDNode, "Name", "").toLatin1();
                inst_MT.Mbuf[mcnt].DevId = inst_xml.GetValue(IEDNode, "DevId", "").toInt();
                inst_MT.Mbuf[mcnt].pfName = inst_xml.GetValue(swNode, "pfName", "").toLatin1();
                Nodetmp = inst_xml.findNode(swNode, "Measure", numtmp.setNum(k));
                inst_MT.Mbuf[mcnt].pfAddr = inst_xml.GetAttribute(Nodetmp, "pfAddr").toLatin1();
                inst_MT.Mbuf[mcnt].Type = inst_xml.GetAttribute(Nodetmp, "Type").toInt();
                inst_MT.Mbuf[mcnt].Value = inst_xml.GetValue(Nodetmp).toLatin1();
                mcnt++;
            }
            swcnt = inst_xml.GetCount(swNode, "Ctrl");
            for(k=0;k<swcnt;k++)
            {
                inst_MT.Cbuf[ccnt].Name = inst_xml.GetValue(IEDNode, "Name", "").toLatin1();
                inst_MT.Cbuf[ccnt].DevId = inst_xml.GetValue(IEDNode, "DevId", "").toInt();
                inst_MT.Cbuf[ccnt].pfName = inst_xml.GetValue(swNode, "pfName", "").toLatin1();
                Nodetmp = inst_xml.findNode(swNode, "Ctrl", numtmp.setNum(k));
                inst_MT.Cbuf[ccnt].pfAddr = inst_xml.GetAttribute(Nodetmp, "pfAddr").toLatin1();
                inst_MT.Cbuf[ccnt].Type = inst_xml.GetAttribute(Nodetmp, "Type").toInt();
                inst_MT.Cbuf[ccnt].Value = inst_xml.GetValue(Nodetmp).toLatin1();
                ccnt++;
            }
        }
    }

    for(i=0;i<mcnt;i++)
    {
        qDebug() << i << inst_MT.Mbuf[i].Name << inst_MT.Mbuf[i].DevId << inst_MT.Mbuf[i].pfName
                 << inst_MT.Mbuf[i].pfAddr << inst_MT.Mbuf[i].Type << inst_MT.Mbuf[i].Value;
    }
    for(i=0;i<ccnt;i++)
    {
        qDebug() << i << inst_MT.Cbuf[i].Name << inst_MT.Cbuf[i].DevId << inst_MT.Cbuf[i].pfName
                 << inst_MT.Cbuf[i].pfAddr << inst_MT.Cbuf[i].Type << inst_MT.Cbuf[i].Value;
    }

    inst_MT.Tmslen = mcnt;
    inst_MT.Tctlen = ccnt;
    inst_MT.Tlen = mcnt + ccnt;

    return 0;
}

QByteArray ModelConfig::getpfName(int cnt)
{
    if(cnt < inst_MT.Tmslen)
        return inst_MT.Mbuf[cnt].pfName;
    else
        return inst_MT.Cbuf[cnt-inst_MT.Tmslen].pfName;
}

QByteArray ModelConfig::getpfAddr(int cnt)
{
    if(cnt < inst_MT.Tmslen)
        return inst_MT.Mbuf[cnt].pfAddr;
    else
        return inst_MT.Cbuf[cnt-inst_MT.Tmslen].pfAddr;
}

int ModelConfig::getType(int cnt)
{
    if(cnt < inst_MT.Tmslen)
        return inst_MT.Mbuf[cnt].Type;
    else
        return inst_MT.Cbuf[cnt-inst_MT.Tmslen].Type;
}

QByteArray ModelConfig::getValue(int cnt)
{
    if(cnt < inst_MT.Tmslen)
        return inst_MT.Mbuf[cnt].Value;
    else
        return inst_MT.Cbuf[cnt-inst_MT.Tmslen].Value;
}

void ModelConfig::setObj(int cnt, DataObject  *Objp)
{
    if(cnt < inst_MT.Tmslen)
        inst_MT.Mbuf[cnt].Objp = Objp;
    else
        inst_MT.Cbuf[cnt-inst_MT.Tmslen].Objp = Objp;
}

DataObject *ModelConfig::getObj(int cnt)
{
    if(cnt < inst_MT.Tmslen)
        return inst_MT.Mbuf[cnt].Objp;
    else
        return inst_MT.Cbuf[cnt-inst_MT.Tmslen].Objp;
}

int ModelConfig::getCount()
{
    return inst_MT.Tlen;
}

int ModelConfig::getCBase()
{
    return inst_MT.Tmslen;
}

int ModelConfig::getMC(int cnt)
{
    if(cnt < inst_MT.Tmslen)
        return MODELCONFIG_MEASURE;
    else
        return MODELCONFIG_CTRL;
}

int ModelConfig::getaddress(const api::Value *relevantObj)
{
    QByteArray qbatmp,valuetmp;
    int error = 0;

    //emit senddata("typename\tname\ttype");
    //emit senddata("-----------------------------------------------------\n");
    for(int i=0,count=relevantObj->VecGetSize(&error);i<count;i++)
    {
        DataObject *child = relevantObj->VecGetDataObject(i);
        if(strcmp(child->GetName()->GetString(), "ElmTerm") == 0)
        {

            //qbatmp.clear();
            //qbatmp.append(child->GetClassName()->GetString());
            //qbatmp.append("\t");
            //qbatmp.append(child->GetName()->GetString());
            //qbatmp.append("\t");

            double volt = child->GetAttributeDouble("m:u");
            qbatmp.append(valuetmp.setNum(volt));

            qbatmp.append("\n");
            //emit senddata(qbatmp);
        }
        if(strcmp(child->GetClassName()->GetString(), "ElmLne") == 0)
        {
            qbatmp.clear();
            qbatmp.append(child->GetClassName()->GetString());
            qbatmp.append("\t");
            qbatmp.append(child->GetName()->GetString());
            qbatmp.append("\t");

            double volt = child->GetAttributeDouble("m:u");
            qbatmp.append(valuetmp.setNum(volt));

            qbatmp.append("\n");
            //emit senddata(qbatmp);

            //if(strcmp(child->GetName()->GetString(), "S2") == 0)
            //    S2 = child;
            //else
            //    apiInstance->ReleaseObject(child);
        }
        //apiInstance->ReleaseObject(child);
    }

    return 0;
}


