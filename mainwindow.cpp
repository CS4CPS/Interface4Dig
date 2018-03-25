#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //console
    fm_console = new drv_console(this);
    fm_console->setGeometry(QRect(10, 190, 480, 200));
    fm_console->init();
    fm_console->show();
    fm_console->msg("DigCom Start.\n");

    //sstp
    inst_sstp.init(DRV_SSTP_SLAYER, ID_POWERFACTORY);

    //fcuntion
    pb_net_state = 0;
    on_pb_net_clicked();

    //for test
    //fixture->interfaceInit();
    //fixture->Dig_init();
    //fixture->Dig_reset();

    //fixture->Dig_next();
    //fixture->Dig_step(0.1);
    //fixture->Dig_getdata();
    //fixture->Dig_step(0.5);
    //fixture->Dig_getdata();

    //fixture->Dig_process();
    //fixture->Dig_run(0.1);
    //fixture->Dig_setevent();
}

void MainWindow::xmlpf_init(void)
{
    //xml
    devModelConfig.config();

    //powerfactory
    fixture = new DigInterface();
    fixture->init();
    fixture->maxlen = 20480;
    fixture->datap = inst_rtdb_mem_buf;
    QObject::connect(fixture, SIGNAL(senddata(QByteArray)),
                     fm_console, SLOT(msg(QByteArray)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete fm_console;
    delete fixture;
}

void MainWindow::on_pb_net_clicked()
{
    switch(pb_net_state)
    {
    case 0:
        ui->pb_net->setText("Start Server");
        //ui->le_netpar->setText("49.123.120.232;6666");
        ui->le_netpar->setText("10.0.2.15;6666");
        //ui->le_netpar->setText("192.168.43.87;6666");
        ui->le_netpar->setEnabled(true);
        pb_net_state = 1;
        break;
    case 1:
        server_init();

        ui->pb_net->setText("Stop Server");
        ui->le_netpar->setEnabled(false);
        pb_net_state = 2;
        break;
    case 2:
        server_final();

        ui->pb_net->setText("Start Server");
        ui->le_netpar->setEnabled(true);
        pb_net_state = 1;
        break;
    }
}

void MainWindow::server_init(void)
{
    QString partmp,iptmp;
    QStringList splittmp;
    uint16 porttmp;

    partmp = ui->le_netpar->text().toLatin1();
    splittmp = partmp.split(";",QString::SkipEmptyParts);

    iptmp.append(splittmp.at(0));
    porttmp = splittmp.at(1).toUShort();

    tcpserver.init(tcpserver.getIP(iptmp), porttmp);

    QObject::connect(&tcpserver, SIGNAL(ReceReady()),
                     this, SLOT(server_rece()));
}

void MainWindow::server_final(void)
{
    QObject::disconnect(&tcpserver, SIGNAL(ReceReady()),
                     this, SLOT(server_rece()));
}

void MainWindow::server_rece(void)
{
    int32 resrece,resprot,resdata,ressend;
    uint8 buf[512],sbuf[512];
    drv_sstp_head shead;
    QByteArray qbatmp;
    int i;

    resrece = tcpserver.rece(0,buf,512);
    if(resrece > 0)
    {
        //fm_console->msg("ETH::PRIM::");
        //for(i=0;i<resrece;i++){
        //    qbatmp.setNum(buf[i],16);
        //    fm_console->msg("0x"+qbatmp+" ");
        //}
        //fm_console->msg("\n");

        //printf("ETH::response::");for(int i=0;i<resrece;i++)printf("0x%x ",buf[i]);printf("\n");
        resprot = inst_sstp.resolve(buf,resrece);
        if(resprot > 0)
        {
            //if(inst_sstp.headbuf.des == ID_POWERFACTORY)
                resdata = response(inst_sstp.headbuf, inst_sstp.outbuf, buf);
            //else
                //resdata = 0;

            if(resdata > 0)
            {
                shead.des = inst_sstp.headbuf.sor;
                shead.sor = inst_sstp.headbuf.des;
                shead.type = inst_sstp.headbuf.type | 0x80;
                shead.flag = inst_sstp.headbuf.flag;
                shead.len = resdata;

                ressend = inst_sstp.fill(sbuf, &shead, buf);
                if(ressend > 0)
                {
                    tcpserver.send(0,sbuf,ressend);
                }
            }
        }
    }
}

int32 MainWindow::response(drv_sstp_head shead, uint8 *inbuf, uint8 *outbuf)
{
    uint8 *data;
    uint32 datapl[4];
    int32 res,i;
    uint32 blk,para[4],len;
    QByteArray qbatmp;
    float runtime;

    data = &inbuf[DRV_SSTP_IDXDL];
    for(i=0;i<4;i++)
        datapl[i] = (inbuf[i*4+3]<<24) | (inbuf[i*4+2]<<16) | (inbuf[i*4+1]<<8) | (inbuf[i*4+0]);
    res = -1;

    uint32 *cmdp = (uint32*)outbuf;
    //printf("ETH::response::");for(i=0;i<16;i++)printf("0x%x ",inbuf[i]);printf("\n");

    switch(shead.type)
    {
        case DRV_SSTP_CMD_CSET:
            fm_console->msg("CMD_CSET::cmd = "+qbatmp.setNum(datapl[SQUEUE_DEF_CMD_POS])+".\n");
            switch(datapl[SQUEUE_DEF_CMD_POS])
            {
            case SSTP_DEF_CMD_INIT:
                fm_console->msg("MultiAgentSystem have get the control.\n");
                fixture->interfaceInit();
                fixture->Dig_init();
                cmdp[SQUEUE_DEF_CMD_POS] = datapl[SQUEUE_DEF_CMD_POS];
                cmdp[1] = 0;
                cmdp[2] = 0;
                cmdp[DRV_SSTP_RSP_POS] = DRV_SSTP_RSP_OK;
                res = DRV_SSTP_IDXDL;
                break;
            case SSTP_DEF_CMD_RST:
                fm_console->msg("Initial Conditions.\n");
                fixture->Dig_reset();
                cmdp[SQUEUE_DEF_CMD_POS] = datapl[SQUEUE_DEF_CMD_POS];
                cmdp[1] = 0;
                cmdp[2] = 0;
                cmdp[DRV_SSTP_RSP_POS] = DRV_SSTP_RSP_OK;
                res = DRV_SSTP_IDXDL;
                break;
            case SSTP_DEF_PAR_NEXT:
                fm_console->msg("Get next step.\n");
                res = fixture->Dig_next();
                break;
            case SSTP_DEF_PAR_STEP:
                runtime = datapl[1];
                runtime = runtime/10000;
                fm_console->msg("Run the Simulation to "+qbatmp.setNum(runtime*1000)+" ms.\n");
                res = fixture->Dig_step(runtime);
                fixture->Dig_getdata();
                cmdp[SQUEUE_DEF_CMD_POS] = datapl[SQUEUE_DEF_CMD_POS];
                cmdp[1] = 0;
                cmdp[2] = 0;
                cmdp[DRV_SSTP_RSP_POS] = DRV_SSTP_RSP_OK;
                res = DRV_SSTP_IDXDL;
                break;
            default:
                cmdp[SQUEUE_DEF_CMD_POS] = datapl[SQUEUE_DEF_CMD_POS];
                cmdp[1] = 0;
                cmdp[2] = 0;
                cmdp[DRV_SSTP_RSP_POS] = DRV_SSTP_RSP_ERR;
                res = DRV_SSTP_IDXDL;
            }
            break;
        case DRV_SSTP_CMD_RDATA:
            blk = datapl[DRV_SSTP_DATA_BLK];
            para[0] = datapl[DRV_SSTP_DATA_ADDR];
            para[1] = datapl[DRV_SSTP_DATA_PAR];
            para[2] = blk;
            len = datapl[DRV_SSTP_DATA_LEN];
            res = fixture->rece(para,outbuf,len);

            //fm_console->msg("ETH::RDATA::");
            //for(i=0;i<len;i++){
            //    qbatmp.setNum(outbuf[i],16);
            //    fm_console->msg("0x"+qbatmp+" ");
            //}
            //printf("\n");

            break;
        case DRV_SSTP_CMD_WDATA:
            blk = datapl[DRV_SSTP_DATA_BLK];
            para[0] = datapl[DRV_SSTP_DATA_ADDR]+fixture->getCBase()*4;
            para[1] = datapl[DRV_SSTP_DATA_PAR];
            para[2] = blk;
            len = datapl[DRV_SSTP_DATA_LEN];

            //fm_console->msg("ETH::WDDATA::");
            //for(i=0;i<len;i++){
            //    qbatmp.setNum(data[i],16);
            //    fm_console->msg("0x"+qbatmp+" ");
            //}
            //fm_console->msg("\n");
            res = fixture->send(para,data,len);

            //fm_console->msg("ETH::MEM::");
            //for(i=0;i<len;i++){
            //    qbatmp.setNum(fixture->datap[i],16);
            //    fm_console->msg("0x"+qbatmp+" ");
            //}
            //fm_console->msg("\n");

            //fixture->Dig_setevent();
            fixture->Dig_SwitchCtrl();
            cmdp[SQUEUE_DEF_CMD_POS] = datapl[SQUEUE_DEF_CMD_POS];
            cmdp[1] = 0;
            cmdp[2] = 0;
            if(res == len)
                cmdp[DRV_SSTP_RSP_POS] = DRV_SSTP_RSP_OK;
            else
                cmdp[DRV_SSTP_RSP_POS] = DRV_SSTP_RSP_ERR;
            res = DRV_SSTP_IDXDL;
            break;
    }
    return res;
}
