#include "dev_mpdu.h"

Dev_Mpdu::Dev_Mpdu(QObject *parent) : Dev_Object(parent)
{
    mDev = mPacket->getMpdu();
    mDt = &(mDev->dt);
    initCfg();
}


Dev_Mpdu *Dev_Mpdu::bulid(QObject *parent)
{
    static Dev_Mpdu* sington = nullptr;
    if(sington == nullptr)
        sington = new Dev_Mpdu(parent);
    return sington;
}


void Dev_Mpdu::initType()
{
    sDevType *ptr = mDt;
    ptr->lines = read("lines", 1).toInt();
    ptr->loops = read("loops", 3).toInt();
    ptr->series = read("series", 4).toInt();
    ptr->breaker = read("breaker", 1).toInt();
    ptr->standar = read("standar", 0).toInt();
    ptr->outputs = read("outputs", 24).toInt();
    ptr->language = read("language", 1).toInt();
    ptr->modbus = read("modbus", 0).toInt();
    ptr->mac = read("mac", "").toString();
}

void Dev_Mpdu::writeType()
{
    sDevType *ptr = mDt;
    write("lines", ptr->lines);
    write("loops", ptr->loops);
    write("series", ptr->series);
    write("breaker", ptr->breaker);
    write("standar", ptr->standar);
    write("outputs", ptr->outputs);
    write("language", ptr->language);
    write("modbus", ptr->modbus);
    write("mac", ptr->mac);
}

void Dev_Mpdu::initData()
{
    sObjData *ptr = &(mDev->data);
    initUnit("vol", ptr->vol);
    initUnit("cur", ptr->cur);

    initUnit("tem", ptr->tem);
    initUnit("hum", ptr->hum);
    initUnit("output", ptr->output);

    for(int i=0; i<OpSize; ++i) {
        QString str = "op_" + QString::number(i+1);
        initUnit(str, ptr->opCur[i]);
    }
}

void Dev_Mpdu::writeData()
{
    sObjData *ptr = &(mDev->data);
    writeUnit("vol", ptr->vol);
    writeUnit("cur", ptr->cur);

    writeUnit("tem", ptr->tem);
    writeUnit("hum", ptr->hum);
    writeUnit("output", ptr->output);

    for(int i=0; i<OpSize; ++i) {
        QString str = "op_" + QString::number(i+1);
        writeUnit(str, ptr->opCur[i]);
    }
}

void Dev_Mpdu::initCfg()
{
    initType();
    initData();
}

void Dev_Mpdu::save()
{
    writeType();
    writeData();
}
