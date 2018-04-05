#include "drv_console.h"
#include <QMessageBox>

drv_console::drv_console(QWidget *parent) :
    QWidget(parent)
{
    textBrowser = new QTextBrowser(this);
}

drv_console::~drv_console()
{
    delete textBrowser;
}

void drv_console::init()
{
    textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
    textBrowser->setGeometry(0,0,this->width(),this->height());
    textBrowser->show();
}

void drv_console::msg(QByteArray s)
{
    QByteArray t,p,p2,out,tmp;
    //t.append("1:HEXSTR1234HEXEND:2:HEXSTR5678HEXEND:3:HEXSTR90HEXEND:4");

    int str,end;
    p.append(s);
    str = p.indexOf("HEXSTR");
    end = p.indexOf("HEXEND");

    while((str>=0) && (end>=0))
    {
        out.append(p,str);
        int num = end - str - 6;
        for(int i=0;i<num;i++)
        {
            tmp.setNum(p.data()[i+str+6],16);
            out.append("0x"+tmp.right(2)+" ");
        }
        p2.clear();
        p2.append(p);
        p.clear();
        p.append(p2.right(p2.length()-end-6));
        str = p.indexOf("HEXSTR");
        end = p.indexOf("HEXEND");
    }
    out.append(p);

    textBrowser->insertPlainText(out);
    textBrowser->moveCursor(QTextCursor::End);
}
