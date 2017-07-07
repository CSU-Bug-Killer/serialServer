#include "deviceinfo.h"

QHash<QString,QString> DeviceInfo::carHashset;
QHash<QString,QString> DeviceInfo::lightHashset;

DeviceInfo::DeviceInfo(QObject *parent) : QObject(parent)
{

}

DeviceInfo::DeviceInfo(int deviceType,QString addr)
{
    this->deviceType = deviceType;
    this->addr = addr;


}


