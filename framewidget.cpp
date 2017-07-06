#include "framewidget.h"
#include "ui_framewidget.h"

#include <QInputDialog>
#include <QMessageBox>

FrameWidget::FrameWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameWidget)
{
    ui->setupUi(this);

    this->isModify = false;
    this->noFrame = true;
    //设置删除选项为不可选
    this->setDelCheckEnable(false);
    //修改帧名字按钮与信号连接
    connect(ui->nameBtn, SIGNAL(clicked()), this, SLOT(setFrameName()));
    //连接发送按钮与槽函数
    connect(ui->sendBtn, SIGNAL(clicked()), this, SLOT(sendSlot()));
    connect(ui->hexSendBtn, SIGNAL(clicked()), this, SLOT(hexSendSlot()));

}

FrameWidget::~FrameWidget()
{
    delete ui;
}

void FrameWidget::setFrameName(QString name)
{
    ui->nameBtn->setText(tr("%1").arg(name));
}

void FrameWidget::setFrame(QString frame)
{
    ui->frameLdt->setText(tr("%1").arg(frame));
}

QString FrameWidget::getFrameName()
{
    return ui->nameBtn->text();
}

QString FrameWidget::getFrame()
{
    return ui->frameLdt->text();
}

bool FrameWidget::isDel()
{
    return ui->delCheck->isChecked();
}

void FrameWidget::setDelCheckEnable(bool ok)
{
    ui->delCheck->setEnabled(ok);
}

void FrameWidget::setFrameName()
{
    bool    ok;
    QString name = QInputDialog::\
            getText(this, tr("Input a Name"),\
                    tr("Name: "),\
                    QLineEdit::Normal,\
                    tr("Test Frame:"), &ok);
    if(ok){
        ui->nameBtn->setText(name+tr(": "));
        emit this->frameNameChanged(name);
    }
}

void FrameWidget::modeifyFrame()
{
    if(isModify){//停止修改
        if(ui->frameLdt->text().isEmpty()){
            this->noFrame = true;
            this->setWidgetEnable(this->isModify);
            QMessageBox::warning(this, tr("Modify Or Add Frame"),\
                                 tr("No Data, Please Input Data."),\
                                 QMessageBox::Ok);
        } else {
            isModify = false;
            this->setWidgetEnable(this->isModify);
            this->noFrame = false;
            //ui->modifyBtn->setText(tr("Modify"));
        }
    } else {//修改帧
        this->isModify = true;
        this->setWidgetEnable(this->isModify);
        //ui->modifyBtn->setText(tr("OK"));
    }
}

void FrameWidget::setWidgetEnable(bool flag)
{
    ui->nameBtn->setEnabled(flag);
    ui->frameLdt->setReadOnly(!flag);
    ui->sendBtn->setEnabled(!flag);
    ui->hexSendBtn->setEnabled(!flag);
}

void FrameWidget::sendSlot()
{
    QString frame = ui->frameLdt->text();
    SendMode mode = FrameWidget::Normal;
    emit this->sendFrame(frame, mode);
}

void FrameWidget::hexSendSlot()
{
    QString frame = ui->frameLdt->text();
    SendMode mode = FrameWidget::Hex;
    emit this->sendFrame(frame, mode);
}

