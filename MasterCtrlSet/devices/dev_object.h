#ifndef DEV_OBJECT_H
#define DEV_OBJECT_H

#include "rtu_modbus.h"

class Dev_Object : public QThread
{
    Q_OBJECT
public:
    explicit Dev_Object(QObject *parent = nullptr);
    sDevData *getDev() {return mDev;}

    virtual void save()=0;
    void setMacAddr();

signals:
    void setMacSig();

protected:
    virtual QString cfgGroup()=0;
    void initUnit(const QString& prefix, sUnit &unit, int f=0);
    void writeUnit(const QString& prefix, sUnit &unit, int f=0);

    void write(const QString &key, const QVariant& v);
    QVariant read(const QString &key, const QVariant &v = QVariant());

protected slots:
    virtual void initFunSlot();

protected:
    sDevType *mDt;
    sDevData *mDev;
    RtuRw *mModbus;
    sCfgItem *mItem;
    sProgress *mPro;
    sDataPacket *mPacket;
};

#endif // DEV_OBJECT_H
