#ifndef SETUP_MAINWID_H
#define SETUP_MAINWID_H

#include <QWidget>
#include "usermainwid.h"

namespace Ui {
class Setup_MainWid;
}

class Setup_MainWid : public QWidget
{
    Q_OBJECT

public:
    explicit Setup_MainWid(QWidget *parent = nullptr);
    ~Setup_MainWid();

protected:
    void initMac();
    void initSerial();
    void updateMac();

    void initLogCount();
    void writeLogCount();
    void initPcNum();
    void writePcNum();    

protected slots:
    void timeoutDone();
    void initFunSlot();
    void checkPcNumSlot();
    void on_pcBtn_clicked();
    void on_verBtn_clicked();

private:
    Ui::Setup_MainWid *ui;

    QTimer *timer;
    sCfgItem *mItem;
    UserMainWid *mUserWid;
    SerialStatusWid *mComWid;
};

#endif // SETUP_MAINWID_H