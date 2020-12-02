#include "home_workwid.h"
#include "ui_home_workwid.h"

Home_WorkWid::Home_WorkWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home_WorkWid)
{
    ui->setupUi(this);

    QTimer::singleShot(150,this,SLOT(initFunSlot()));
}

Home_WorkWid::~Home_WorkWid()
{
    delete ui;
}

void Home_WorkWid::initLayout()
{
    QPalette pl = ui->textEdit->palette();
    pl.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));
    ui->textEdit->setPalette(pl);
    ui->userEdit->setPalette(pl);
    ui->textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff ); //垂直滚动条隐藏
    ui->textEdit->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );//水平滚动条隐藏

    QGridLayout *gridLayout = new QGridLayout(this->parentWidget());
    gridLayout->setContentsMargins(0, 7, 0, 0);
    gridLayout->addWidget(this);
}

void Home_WorkWid::initFunSlot()
{
    mCnt = 0;
    mSetOpDlg = new Home_SetOpDlg(this);
    mCoreThread = new Test_CoreThread(this);

    mPacket = sDataPacket::bulid();
    mPro = mPacket->getPro();
    mItem = Cfg::bulid()->item;
    mPro->step = Test_End;

    initLayout();
    initTypeComboBox();
    timer = new QTimer(this);
    timer->start(500);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
}

void Home_WorkWid::setTextColor()
{
    QColor color("black");
    bool pass = mPro->pass.first();
    if(!pass) color = QColor("red");
    ui->textEdit->moveCursor(QTextCursor::Start);

    QTextCharFormat fmt;//文本字符格式
    fmt.setForeground(color);// 前景色(即字体色)设为color色
    QTextCursor cursor = ui->textEdit->textCursor();//获取文本光标
    cursor.mergeCharFormat(fmt);//光标后的文字就用该格式显示
    ui->textEdit->mergeCurrentCharFormat(fmt);//textEdit使用当前的字符格式
}

void Home_WorkWid::insertText()
{
    if(mPro->status.size()) {
        setTextColor();
        QString str = QString::number(mId++) + "、"+ mPro->status.first() + "\n";
        ui->textEdit->insertPlainText(str);
        mPro->status.removeFirst();
        mPro->pass.removeFirst();
    }
}

void Home_WorkWid::updateCnt()
{
    sCount *cnt = &(mItem->cnt);
    ui->okLcd->display(cnt->ok);
    ui->allLcd->display(cnt->all);
    ui->errLcd->display(cnt->err);

    QString str = "0";
    if(cnt->all) {
        double value = cnt->ok*100.0 / cnt->all;
        str = QString::number(value,'f',0) +"%";
    }
    ui->passLcd->display(str);
}

QString Home_WorkWid::getTime()
{
    QTime t(0,0,0,0);
    t = t.addSecs(mPro->startTime.secsTo(QTime::currentTime()));
    return  tr("%1").arg(t.toString("mm:ss"));
}

void Home_WorkWid::updateTime()
{
    QString str = getTime();
    QString style = "background-color:yellow; color:rgb(0, 0, 0);";
    style += "font:100 34pt \"微软雅黑\";";

    ui->timeLab->setText(str);
    ui->timeLab->setStyleSheet(style);
    ui->startBtn->setText(tr("终止设置"));
}

void Home_WorkWid::updateResult()
{
    QString style;
    QString str = tr("---");
    if (Test_Fail == mPro->result) {
        str = tr("失败");
        style = "background-color:red; color:rgb(255, 255, 255);";
    } else {
        str = tr("成功");
        style = "background-color:green; color:rgb(255, 255, 255);";
    }
    style += "font:100 34pt \"微软雅黑\";";

    mPro->step = Test_End;
    ui->timeLab->setText(str);
    ui->timeLab->setStyleSheet(style);
    ui->groupBox_4->setEnabled(true);
    ui->startBtn->setText(tr("开始设置"));
}

void Home_WorkWid::updateWid()
{
    QString str = mItem->sn;
    if(str.isEmpty()) str = "--- ---";
    ui->snLab->setText(str);

    str = mItem->dev_type;
    if(str.isEmpty()) str = "--- ---";
    ui->devLab->setText(str);

    if(mPro->step < Test_Over) {
        updateTime();
    } else if(mPro->step < Test_End){
        updateResult();
    }
}

void Home_WorkWid::timeoutDone()
{
    if(mPro->step < Test_End) {
        insertText();
        updateWid();
        updateCnt();
    }
}

bool Home_WorkWid::initSerial()
{
    bool ret = mItem->com->isOpened();
    if(ret){
        mId = 1;
        mItem->sn.clear();
        mItem->dev_type.clear();
        mPro->step = Test_Start;
    } else {
        MsgBox::critical(this, tr("请先打开串口")); return ret;
    }

    return ret;
}

bool Home_WorkWid::initWid()
{
    bool ret = initSerial();
    if(ret) {
        mPacket->init();
        emit startSig();
        ui->textEdit->clear();
        ui->groupBox_4->setEnabled(false);
    }

    return ret;
}

void Home_WorkWid::on_startBtn_clicked()
{
    if(mPro->step == Test_End) {
        if(initWid()) mCoreThread->start();
    } else {
        bool ret = MsgBox::question(this, tr("确定需要提前结束？"));
        if(ret) {
            mPro->result = Test_Fail;
            updateResult();
        }
    }
}

void Home_WorkWid::on_setBtn_clicked()
{
    QString str = tr("修改");
    bool en = ++mCnt % 2;
    if(en) {
        str = tr("保存");
    } else {
        mItem->user = ui->userEdit->text();
        Cfg::bulid()->writeCfgDev();
    }

    emit enabledSig(en);
    ui->setBtn->setText(str);
    ui->userEdit->setEnabled(en);
    ui->startBtn->setDisabled(en);
    ui->typeComboBox->setDisabled(en);
}

void Home_WorkWid::saveErrSlot()
{
    mCnt = 1;
    emit enabledSig(true);
    ui->setBtn->setText(tr("保存"));
}

void Home_WorkWid::on_outputBtn_clicked()
{
    mSetOpDlg->exec();
}

void Home_WorkWid::on_typeComboBox_currentIndexChanged(int index)
{
    mItem->modeId = index;
    initTypeComboBox();
    Cfg::bulid()->writeCfgDev();
}

void Home_WorkWid::initTypeComboBox()
{
    int index = mItem->modeId;
    bool en = false;
    if(index < 2) en = true;
    ui->outputBtn->setHidden(en);

    mSetOpDlg->updateIndex(index);
    ui->typeComboBox->setCurrentIndex(index);
    emit typeSig(index);
}

