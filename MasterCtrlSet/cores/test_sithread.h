#ifndef TEST_SICTRL_H
#define TEST_SICTRL_H

#include "test_network.h"

class Test_SiThread : public Test_Object
{
    Q_OBJECT
    explicit Test_SiThread(QObject *parent = nullptr);
public:
    static Test_SiThread *bulid(QObject *parent = nullptr);

    bool setDev();

protected:
    bool readDev();
    bool setData();
    bool checkLine();
    bool volAlarmWrite(int i);
    bool curAlarmWrite(int i);
    bool envAlarmWrite();
    bool writeAlarmTh();
    bool setAlarm();
    bool clearEle();

protected slots:
    void initFunSlot();

private:
    Dev_SiRtu *mRtu;
    Dev_SiCtrl *mCtrl;
    Test_Logs *mLogs;
};

#endif // TEST_SICTRL_H
