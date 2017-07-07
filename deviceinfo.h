#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QObject>
//#include <QList>
#include <QHash>

class DeviceInfo : public QObject
{
    Q_OBJECT
public:
//    enum  DeviceType{
//        DEVICE_TYPE_CAR,
//        DEVICE_TYPE_LIGHT
//    };

    static const int DEVICE_TYPE_CAR = 0;
    static const int DEVICE_TYPE_LIGHT = 1;

    explicit DeviceInfo(QObject *parent = 0);

    DeviceInfo(int deviceType,QString addr);

    static QHash<QString,QString> carHashset;
    static QHash<QString,QString> lightHashset;

private:
    int deviceType;
    QString addr;

//signals:


//public slots:


};

#endif // DEVICEINFO_H
