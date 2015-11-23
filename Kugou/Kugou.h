#ifndef KUGOU_H
#define KUGOU_H

#include <QDialog>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QImage>
#include <QListWidget>
#include <QSystemTrayIcon>
#include <QRadioButton>

namespace Ui {
class Kugou;
}

class Kugou : public QDialog
{
    Q_OBJECT

    enum Direction
    {
        Direction_No = 0,
        Direction_Left = 1,
        Direction_Top = 2,
        Direction_Right = 4,
        Direction_Bottom = 8,
        Direction_LeftTop = Direction_Left + Direction_Top,
        Direction_LeftBottom = Direction_Left + Direction_Bottom,
        Direction_RightTop = Direction_Right + Direction_Top,
        Direction_RightBottom = Direction_Right + Direction_Bottom
    };

    enum MousePressType
    {
        MT_No = 0,
        MT_Move,
        MT_Change
    };

    enum RepeatType
    {
        Repeat_SinglePlay = 1,
        Repeat_SingleLoop,
        Repeat_OrderPlay,
        Repeat_ListLoop,
        Repeat_ShufflePlay
    };

public:
    explicit Kugou(QWidget *parent = 0);
    ~Kugou();

protected:
    bool eventFilter(QObject *, QEvent *);
    void resizeEvent(QResizeEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

private slots:
    void trayiconActivated(QSystemTrayIcon::ActivationReason);

    void on_btn_left_show_clicked();
    void on_btn_left_min_clicked();
    void on_btn_left_close_clicked();

    void on_btn_right_hide_clicked();
    void on_btn_right_min_clicked();
    void on_btn_right_close_clicked();

private:
    void initChangeSize();
    void showMinimized();
    void showNormal();
    void showFullScreen();

    void setRepeat(int);
    void close(bool exit = true);
    QObjectList foreachWidget(QObject *);

    QRadioButton* addRadioButton(QString styleSheet);
    void initLeftTabList();

    // 大小改变的鼠标
    void sizeCursor(int);

    // 获得大小改变的方向
    int sizeDirection(int, int);

private:
    // 背景图片
    QImage m_image;
    QString m_imageName;
    int m_imageWidth;
    int m_imageHeight;

    // 是否全屏
    bool m_isFullScreen;

    // 移动窗口
    QPoint m_oldMousePos;
    QRect m_oldDlgRect;
    bool m_mousePress;
    int m_direction;

private:
    Ui::Kugou *ui;
};

#endif // KUGOU_H
