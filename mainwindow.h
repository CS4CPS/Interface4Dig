#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <string>
#include <QtDebug>
#include "hal/drv_sstp.h"

#include "hal_qt/drv_console.h"
#include "hal_qt/drv_tcp_server.h"

#include "rtdb/drv_rtdb_mem.h"

#include "diginterface.h"

#include "sstp_def.h"

#include "modelconfig.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //console
    drv_console *fm_console;

    //xml
    ModelConfig devModelConfig;
    QString app_dir;

    //Powerfactory
    DigInterface *fixture;
    uint8 inst_rtdb_mem_buf[20480];
    void xmlpf_init(void);

    //sstp
    drv_sstp inst_sstp;
    int32 response(drv_sstp_head shead, uint8 *inbuf, uint8 *outbuf);

    //tcp
    drv_tcp_server tcpserver;
    void server_init(void);
    void server_final(void);

private slots:
    void on_pb_net_clicked();

    void server_rece(void);

private:
    Ui::MainWindow *ui;
    int pb_net_state;
};

#endif // MAINWINDOW_H
