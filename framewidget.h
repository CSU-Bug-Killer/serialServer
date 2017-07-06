#ifndef FRAMEWIDGET_H
#define FRAMEWIDGET_H

#include <QFrame>

namespace Ui {
class FrameWidget;
}

class FrameWidget : public QFrame
{
    Q_OBJECT

public:
    explicit FrameWidget(QWidget *parent = 0);
    ~FrameWidget();

    enum    SendMode{Normal, Hex};
    QString getFrameName();
    QString getFrame();
    void    setFrameName(QString name);
    void    setFrame(QString frame);
    bool    isDel();
    void    setDelCheckEnable(bool);
public slots:
    //设置帧名
    void setFrameName();
    //修改帧
    void modeifyFrame();
    //界面控件设置
    void setWidgetEnable(bool);
    //普通发送
    void    sendSlot();
    //hex发送
    void    hexSendSlot();
signals:
    void    frameNameChanged(QString name);
    void    sendFrame(QString &frame, FrameWidget::SendMode &mode);
private:
    bool    isModify;
    bool    noFrame;

private:
    Ui::FrameWidget *ui;
};

#endif // FRAMEWIDGET_H
