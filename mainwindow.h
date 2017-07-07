#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>
#include <QMutex>
#include <QFile>
#include <QVBoxLayout>

#include "framewidget.h"
#include "serverwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void sendMegSignal(QByteArray);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
//!--1
    //刷新可用端口
    void    refreshPort(void);
    //读取串口
    void    readSlot(void);
    //显示数据
    void    displaySlot(void);
//!--1
//! --6
protected:
    void    closeEvent(QCloseEvent *);
//! --6
private slots:
    void logDevice(QByteArray data);
    void setLogDevice();
    void server_send_to_serial_port(QString data);
//    void test_time(QByteArray data);
//!--2
    void on_openOrCloseBtn_clicked();

    void on_refreshPortAct_triggered();

    void on_sendBtn_clicked();

    void on_clearBtn_clicked();

    void on_autoSendCheckBox_clicked(bool checked);

    void on_hexSend_clicked(bool checked);

    void on_hexRecv_clicked(bool checked);

    void on_baudComBox_currentIndexChanged(const QString &arg1);

    void on_dataComBox_currentIndexChanged(const QString &arg1);

    void on_portComBox_currentIndexChanged(const QString &arg1);

    void on_sendLdt_textChanged(const QString &arg1);

    void on_addCheckBox_clicked();

    void on_addCheckBox_clicked(bool checked);

    void on_toolComBox_currentIndexChanged(const QString &arg1);

    void on_action_3_triggered();
//!--2

private:

    QByteArray car;
    bool isLogDevice = false;
//!--3
    //从界面上获取配置信息
    QSerialPort::BaudRate   getBaud(void);
    QSerialPort::StopBits   getStopBit(void);
    QSerialPort::DataBits   getDataBit(void);
    QSerialPort::Parity     getParity(void);
    //字符串转化成16进制
    int     stringToHex(QString&, QByteArray&);
    char    charToHex(char);
//!--3
private:
    Ui::MainWindow *ui;
    QSerialPort*    serialPort;

    bool            isPortOpened;
    bool            isExDisplay;

    QTimer          displayTimer;
    QTimer          autoSendTimer;

    QMutex          readMutex;
    QMutex          displayMutex;
    QByteArray      readData;
    QByteArray      rfidRead;

    QByteArray      displayBuffer;
    QByteArray      displayHexBuffer;

    QByteArray      hexSendBuffer;
    QByteArray      normalSendBuffer;

    unsigned int    rxCount;
    unsigned int    txCount;

    //!--4 扩展帧模块
    bool                isDelete;
    QFile               frameFile;
    QList<FrameWidget*> frameList;

    //!--4

    //! --5

    ServerWidget *singleServer;

public slots:
    void    saveFrame();
    void    loadFrame();
    void    deleteFrame();
    void    addFrameSlot();
    void    sendFrameSlot(QString &frame, FrameWidget::SendMode &mode);
    //! --5
};

#endif // MAINWINDOW_H
