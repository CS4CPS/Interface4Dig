#ifndef DRV_EXCEL_H
#define DRV_EXCEL_H

#include "drv/drv_base.h"

#include <QDomDocument>
#include <QFile>

class drv_xml : public QObject,public drv_base
{
    Q_OBJECT

public:
    drv_xml();
    ~drv_xml();

    void init(QByteArray fileName);
    QDomNode getRootNode();
    QDomNode findNode(QDomNode root, QString Name, QString cnt);
    int GetCount(QDomNode root, QString Name);
    QString GetValue(QDomNode root, QString Name, QString cnt);
    QString GetValue(QDomNode root);
    QString GetAttribute(QDomNode root, QString ab);

    QDomDocument dom;
    QDomNode ModelConfig;
};

#endif // DRV_EXCEL_H
