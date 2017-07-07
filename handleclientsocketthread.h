#ifndef HANDLECLIENTSOCKETTHREAD_H
#define HANDLECLIENTSOCKETTHREAD_H
#include <QThread>
#include <QTcpSocket>
#include <QSerialPort>


class HandleClientSocketThread : public QThread

{
    Q_OBJECT

public:
    HandleClientSocketThread(QTcpSocket* soc,QSerialPort *sPort);
private:
    QTcpSocket *socket;
    QSerialPort *serialPort;
protected:
    void run();
signals:
    void done();
private slots:
    void handleSlot();


};

#endif // HANDLECLIENTSOCKETTHREAD_H
