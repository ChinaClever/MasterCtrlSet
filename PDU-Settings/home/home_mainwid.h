#ifndef HOME_MAINWID_H
#define HOME_MAINWID_H

#include <QWidget>
#include "home_workwid.h"

namespace Ui {
class Home_MainWid;
}

class Home_MainWid : public QWidget
{
    Q_OBJECT

public:
    explicit Home_MainWid(QWidget *parent = nullptr);
    ~Home_MainWid();

signals:
    void startSig();

protected:
    void initWid();

private:
    Ui::Home_MainWid *ui;
    Home_IpWid *mIpWid;
    Home_SiWid *mSiWid;
    Home_WorkWid *mWorkWid;
    Home_MpduWid *mMpduWid;
    Home_ZpduWid *mZpduWid;
    Home_RpduWid *mRpduWid;
    Home_RpduWid *mXRpduWid;
    Home_AtsWid *mAtsWid;
    Home_IpBusbarwid *mIpBusbarWid;
};

#endif // HOME_MAINWID_H
