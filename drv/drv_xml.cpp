#include "drv_xml.h"

#include <QDebug>

drv_xml::drv_xml()
{    
}

drv_xml::~drv_xml()
{
}

void drv_xml::init(QByteArray fileName)
{
    QFile *file = new QFile(fileName);//"e:/test.xml"
    if (file->open(QIODevice::ReadOnly))
    {
        dom.setContent(file);
    }
    file->close();

    QDomNodeList elms = dom.elementsByTagName("ModelConfig");
    ModelConfig = elms.at(0);

    QDomNode blk,dat,nod;
    QDomNodeList blks = ModelConfig.childNodes();
    for(int i=0;i<blks.count();i++)
    {
        blk = blks.at(i);
        QByteArray tmp = blk.toElement().nodeName().toLatin1();
        //qDebug() << tmp;
    }
}

QDomNode drv_xml::getRootNode()
{
    return ModelConfig;
}

QDomNode drv_xml::findNode(QDomNode root, QString Name, QString cnt)
{
    QDomNodeList children = root.toElement().childNodes();
    QString nametmp;
    QDomNode Noderes;
    int chcnt = children.count();
    for (int i = 0; i < chcnt; i++)
    {
        nametmp = children.item(i).toElement().nodeName();
        if(nametmp.compare(Name) == 0)
        {
            if(cnt.compare("")!=0)
            {
                nametmp = children.item(i).toElement().attribute("id");
                if(nametmp.compare(cnt, Qt::CaseSensitive) == 0)
                {
                    Noderes = children.item(i);
                    break;
                }
            }else
            {
                Noderes = children.item(i);
                break;
            }
        }
    }
    return Noderes;
}

int drv_xml::GetCount(QDomNode root, QString Name)
{
    QDomNodeList children = root.toElement().childNodes();
    QString nametmp;
    int cnt = 0;
    for (int i = 0; i < children.count(); i++)
    {
        nametmp = children.item(i).toElement().nodeName();
        if(nametmp.compare(Name) == 0)
        {
            cnt = cnt + 1;
        }
    }
    return cnt;
}

QString drv_xml::GetValue(QDomNode root, QString Name, QString cnt)
{
    QDomNode Noderes = findNode(root, Name, cnt);
    return Noderes.toElement().text();
}

QString drv_xml::GetValue(QDomNode root)
{
    return root.toElement().text();
}

QString drv_xml::GetAttribute(QDomNode root, QString ab)
{
    return root.toElement().attribute(ab);
}
