#ifndef DRV_CONSOLE_H
#define DRV_CONSOLE_H

#include <QWidget>
#include <QTextBrowser>

class drv_console : public QWidget
{
    Q_OBJECT

public:
    explicit drv_console(QWidget *parent = 0);
    ~drv_console();

    void init();
    QTextBrowser *textBrowser;

public slots:
    void msg(QByteArray s);
};

#endif // DRV_CONSOLE_H
