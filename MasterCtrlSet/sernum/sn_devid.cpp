/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "sn_devid.h"

Sn_DevId::Sn_DevId(QObject *parent) : Dev_Object(parent)
{
    mTypeDef = Sn_DevType::bulid();
}

Sn_DevId *Sn_DevId::bulid(QObject *parent)
{
    static Sn_DevId* sington = nullptr;
    if(sington == nullptr)
        sington = new Sn_DevId(parent);
    return sington;
}

void Sn_DevId::initReadType(sRtuItem &it)
{
    it.addr = mDev->id;
    it.fn = 0x03;
    it.reg = 0xA001;
    it.num = 2;
}

bool Sn_DevId::analysDevType(uchar *buf, int len)
{
    bool ret = false;
    QString str = tr("读取设备类型成功");
    if(len != 4) {
        str = tr("读取设备类型失败：返回长度为%1").arg(len);
        return mPacket->updatePro(str, ret);
    }

    uint id = 0;
    for(int i=0; i<len; ++i) {
        id *= 256;  id += buf[i];
    }

    ret = mTypeDef->analysDevType(id);
    if(!ret){
        str = tr("不支持此设备类型 ID：%1").arg(id);
    }

    return mPacket->updatePro(str, ret);
}

bool Sn_DevId::readDevId()
{
    sRtuItem it;
    initReadType(it);

    uchar recv[8] = {0};
    int len = mModbus->read(it, recv);
    if(!len){ mPacket->delay(1); len = mModbus->read(it, recv); }
    if(0 == len){
        bool ret = mModbus->changeBaudRate(); // 自动转变波特泫
        if(!ret) len = mModbus->read(it, recv);
        if(!len) mModbus->changeBaudRate();
    }

    return analysDevType(recv, len);
}

bool Sn_DevId::readDevType()
{
    mDev->devType.dev_type[0] = 0;
    QString str = tr("开始识别设备类型！");
    bool ret = mPacket->updatePro(str, true, 1);
    if(ret) {
        ret = readDevId();
    }

    return ret;
}
