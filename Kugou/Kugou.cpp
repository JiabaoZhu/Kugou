#include <QIcon>
#include <QAction>
#include <QMenu>
#include <QTimer>
#include <QDesktopWidget>
#include <QDir>
#include <QPalette>
#include <QPainter>
#include <QBitmap>
#include <QDesktopWidget>
#include <QtGlobal>

#include "Kugou.h"
#include "ui_Kugou.h"

// #ifdef Q_OS_MAC
// #ifdef Q_OS_LINUX
// #ifdef Q_OS_WIN32

#define DISTANCE        8

#define  GBK2UTF8(gbk) QString::fromLocal8Bit(gbk).toUtf8().data()

#ifdef QT_NO_DEBUG
#define DebugText(format, ...)  ;
#else
#define DebugText(format, ...)  qDebug(format, ##__VA_ARGS__);
// #define DebugText(format, ...)  ;
#endif

Kugou::Kugou(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Kugou)
{
    ui->setupUi(this);

    QDir dir;
    DebugText("current path: %s", dir.currentPath().toStdString().c_str());


    initChangeSize();

    initLeftTabList();

    {
        m_imageName = ":/Background/Resources/Background/3.jpg";
        m_image.load(m_imageName);
        if (m_image.isNull())
        {
            m_imageName = ":/Background/Resources/Background/1.jpg";
            m_image.load(m_imageName);
        }
        m_imageWidth = m_image.width();
        m_imageHeight = m_image.height();
    }

    {
        setWindowFlags(Qt::FramelessWindowHint);
        setAttribute(Qt::WA_TranslucentBackground);
        setWindowIcon(QIcon("://Resources/Logo.png"));

        // 追踪鼠标
        setMouseTracking(true);
    }

    // 为所有的QWidget部件设置过滤器, 用来转发鼠标移动消息
    {
        QObjectList list = foreachWidget(this);
        foreach (QObject *obj, list)
        {
            QWidget *widget = qobject_cast<QWidget*>(obj);

            // 设置过滤器
            widget->installEventFilter(this);

            // 追踪鼠标
            widget->setMouseTracking(true);
        }
    }

    {
        ui->widget_main->show();
        ui->widget_main->setAutoFillBackground(true);   // 用来显示图片
    }

    // 托盘
    {
        // 判断是否支持系统托盘
        if (QSystemTrayIcon::isSystemTrayAvailable())
        {
            QSystemTrayIcon *trayIcon;
            trayIcon = new QSystemTrayIcon(this);
            trayIcon->setToolTip(tr("Kugou Music"));
            trayIcon->setIcon(QIcon(":/Tray/Resources/Tray/tray.png"));
            // trayIcon->setToolTip(tr("Hello"));
            trayIcon->show();
            // trayIcon->showMessage(QString("jiabao"),QString::fromLocal8Bit("我热恋的故乡"),QSystemTrayIcon::Information,5000);

            connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayiconActivated(QSystemTrayIcon::ActivationReason)));

            //创建监听行为
            QAction *restoreAction = new QAction(QString::fromLocal8Bit("打开主面板"), this);
            QAction *quitAction = new QAction(QString::fromLocal8Bit("退出"), this);
            // connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
            // connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

            //创建右键弹出菜单
            QMenu *trayIconMenu = new QMenu(this);
            trayIconMenu->addAction(restoreAction);
            trayIconMenu->addSeparator();
            trayIconMenu->addAction(quitAction);
            trayIcon->setContextMenu(trayIconMenu);
        }
    }

    ui->slider_time->setValue(0);

    {
        QSize desktopSize = QApplication::desktop()->size();
        move((desktopSize.width() - width())/2, (desktopSize.height() - height())/2);
    }
}

Kugou::~Kugou()
{
    delete ui;
}

void Kugou::trayiconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    //单击托盘图标
    case QSystemTrayIcon::Trigger:

    //双击托盘图标
    case QSystemTrayIcon::DoubleClick:
        show();
        this->raise();
        break;
    default:
        break;
    }
}

bool Kugou::eventFilter(QObject *object, QEvent *event)
{
    if (QEvent::MouseMove == event->type())
    {
        QMouseEvent *mouseEvent = (QMouseEvent*) event;

        // 构造鼠标移动消息
        // QMouseEvent mouseEvent(QEvent::MouseMove, QPoint(1, 12), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
        QApplication::sendEvent(this, mouseEvent);
    }
    return false;
}

void Kugou::resizeEvent(QResizeEvent *event)
{
    ui->widget_main->resize(event->size());

    // 缩放背景图片
    {
        QPalette pal(palette());

        double x = event->size().width() / (double) m_imageWidth;
        double y = event->size().height() / (double) m_imageHeight;
        if (x < y)
        {
            x = y;
        }

        pal.setBrush(QPalette::Window, QBrush(m_image.scaled(QSize(m_imageWidth * x, m_imageHeight * x))));
        ui->widget_main->setPalette(pal);
    }
}

void Kugou::mouseDoubleClickEvent(QMouseEvent *event)
{
    if( event->buttons() == Qt::LeftButton)
    {
        if (m_isFullScreen)
        {
            showNormal();
        }
        else
        {
            showFullScreen();
        }
    }
}

void Kugou::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_mousePress = true;
        m_direction = sizeDirection(event->pos().x(), event->pos().y());
        m_oldMousePos = event->globalPos();

        // 是否全屏
        if (!m_isFullScreen)
        {
            m_oldDlgRect = geometry();
        }
    }
}

void Kugou::mouseMoveEvent(QMouseEvent *event)
{
    // 鼠标按下
    if (m_mousePress)
    {
        showNormal();
        move(0,0);
    }

    // 当全屏时不能使用拉伸箭头
    else if (!m_isFullScreen)
    {
        m_direction = sizeDirection(event->pos().x(), event->pos().y());
        sizeCursor(m_direction);
    }
}

void Kugou::mouseReleaseEvent(QMouseEvent *)
{
    m_mousePress = false;
    setCursor(QCursor(Qt::ArrowCursor));
}

void Kugou::sizeCursor(int direction)
{
    if (Direction_No == direction)
    {
        setCursor(QCursor(Qt::ArrowCursor));
    }
    else if ((Direction_Left == direction) || (Direction_Right == direction))
    {
        setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if ((Direction_Top == direction) || (Direction_Bottom == direction))
    {
        setCursor(QCursor(Qt::SizeVerCursor));
    }
    else if ((Direction_LeftTop == direction) || (Direction_RightBottom == direction))
    {
        setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else if ((Direction_LeftBottom == direction) || (Direction_RightTop == direction))
    {
        setCursor(QCursor(Qt::SizeBDiagCursor));
    }
}

int Kugou::sizeDirection(int x, int y)
{
    int width = this->width();
    int height = this->height();

    int direction = Direction_No;

    if (x <= DISTANCE)
    {
        direction += Direction_Left;
    }
    else if (width-DISTANCE <= x)
    {
        direction += Direction_Right;
    }

    if (y <= DISTANCE)
    {
        direction += Direction_Top;
    }
    else if (height-DISTANCE <= y)
    {
        direction += Direction_Bottom;
    }

    return direction;
}

// 遍历所有的QWidget控件
QObjectList Kugou::foreachWidget(QObject *object)
{
    QObjectList result;
    QObjectList list = object->children();
    foreach (QObject *obj, list)
    {
        if ("QWidget" == QString(obj->metaObject()->className()))
        {
            DebugText("child: widget %s", obj->objectName().toStdString().c_str());
            result.append(obj);
        }
        result += foreachWidget(obj);
    }
    return result;
}

QRadioButton* Kugou::addRadioButton(QString styleSheet)
{
    QRadioButton *btn = new QRadioButton(this);
    btn->setStyleSheet(styleSheet);
    btn->setFocusPolicy(Qt::NoFocus);
    return btn;
}

void Kugou::on_btn_left_show_clicked()
{
}

void Kugou::on_btn_left_min_clicked()
{
    showMinimized();
}

void Kugou::on_btn_left_close_clicked()
{
    close();
}

void Kugou::on_btn_right_hide_clicked()
{
}

void Kugou::on_btn_right_min_clicked()
{
    showMinimized();
}

void Kugou::on_btn_right_close_clicked()
{
    close();
}

void Kugou::initChangeSize()
{
    m_isFullScreen = false;
    m_mousePress = false;
    m_direction = Direction_No;
}

void Kugou::showMinimized()
{
    hide();
}

void Kugou::showNormal()
{
    if (m_isFullScreen)
    {
        m_isFullScreen = false;
        setFixedSize(m_oldDlgRect.width(), m_oldDlgRect.height());
        setGeometry(m_oldDlgRect);
    }
}

void Kugou::showFullScreen()
{
    if (!m_isFullScreen)
    {
        m_isFullScreen = true;
        m_oldDlgRect = geometry();
        QRect availableGeometry = QApplication::desktop()->availableGeometry();
        setFixedSize(availableGeometry.size());
        setGeometry(availableGeometry);
    }
}

void Kugou::setRepeat(int repeatType)
{
    if (Repeat_SinglePlay == repeatType)
    {
    }
}

void Kugou::close(bool exit)
{
    if (exit)
    {
        qApp->quit();
    }
}

void Kugou::initLeftTabList()
{
    // 初始化LeftTab
    {
        {
            QListWidgetItem *item = new QListWidgetItem(ui->listWidget_tab);
            item->setToolTip(GBK2UTF8("本地列表"));
            ui->listWidget_tab->insertItem(0, item);
            ui->listWidget_tab->setItemWidget(item, addRadioButton("QRadioButton::indicator { width: 0px; height: 0px; }"
                                                                       "QRadioButton {"
                                                                       "border:none;"
                                                                       "background-image: url(:/LeftTabList/Resources/LeftTabList/item_music_normal.png);}"
                                                                       "QRadioButton:hover {"
                                                                       "background-image: url(:/LeftTabList/Resources/LeftTabList/item_music_hover.png);}"
                                                                       "QRadioButton:checked {"
                                                                       "background-image: url(:/LeftTabList/Resources/LeftTabList/item_music_clicked.png);}"));
        }
        {
            QListWidgetItem *item = new QListWidgetItem(ui->listWidget_tab);
            item->setToolTip(GBK2UTF8("网络收藏"));
            ui->listWidget_tab->insertItem(0, item);
            ui->listWidget_tab->setItemWidget(item, addRadioButton("QRadioButton::indicator { width: 0px; height: 0px; }"
                                                                       "QRadioButton {"
                                                                       "border:none;"
                                                                       "background-image: url(:/LeftTabList/Resources/LeftTabList/item_favorite_normal.png);}"
                                                                       "QRadioButton:hover {"
                                                                       "background-image: url(:/LeftTabList/Resources/LeftTabList/item_favorite_hover.png);}"
                                                                       "QRadioButton:checked {"
                                                                       "background-image: url(:/LeftTabList/Resources/LeftTabList/item_favorite_clicked.png);}"));
        }
        {
            QListWidgetItem *item = new QListWidgetItem(ui->listWidget_tab);
            item->setToolTip(GBK2UTF8("音乐电台"));
            ui->listWidget_tab->insertItem(0, item);
            ui->listWidget_tab->setItemWidget(item, addRadioButton("QRadioButton::indicator { width: 0px; height: 0px; }"
                                                                       "QRadioButton {"
                                                                       "border:none;"
                                                                       "background-image: url(:/LeftTabList/Resources/LeftTabList/item_radio_normal.png);}"
                                                                       "QRadioButton:hover {"
                                                                       "background-image: url(:/LeftTabList/Resources/LeftTabList/item_radio_hover.png);}"
                                                                       "QRadioButton:checked {"
                                                                       "background-image: url(:/LeftTabList/Resources/LeftTabList/item_radio_clicked.png);}"));
        }
        {
            QListWidgetItem *item = new QListWidgetItem(ui->listWidget_tab);
            item->setToolTip(GBK2UTF8("附近"));
            ui->listWidget_tab->insertItem(0, item);
            ui->listWidget_tab->setItemWidget(item, addRadioButton("QRadioButton::indicator { width: 0px; height: 0px; }"
                                                                       "QRadioButton {"
                                                                       "border:none;"
                                                                       "background-image: url(:/LeftTabList/Resources/LeftTabList/item_location_normal.png);}"
                                                                       "QRadioButton:hover {"
                                                                       "background-image: url(:/LeftTabList/Resources/LeftTabList/item_location_hover.png);}"
                                                                       "QRadioButton:checked {"
                                                                       "background-image: url(:/LeftTabList/Resources/LeftTabList/item_location_clicked.png);}"));
        }
        {
            QListWidgetItem *item = new QListWidgetItem(ui->listWidget_tab);
            item->setToolTip(GBK2UTF8("临时播"));
            ui->listWidget_tab->insertItem(0, item);
            ui->listWidget_tab->setItemWidget(item, addRadioButton("QRadioButton::indicator { width: 0px; height: 0px; }"
                                                                       "QRadioButton {"
                                                                       "border:none;"
                                                                       "background-image: url(:/LeftTabList/Resources/LeftTabList/item_temp_normal.png);}"
                                                                       "QRadioButton:hover {"
                                                                       "background-image: url(:/LeftTabList/Resources/LeftTabList/item_temp_hover.png);}"
                                                                       "QRadioButton:checked {"
                                                                       "background-image: url(:/LeftTabList/Resources/LeftTabList/item_temp_clicked.png);}"));
        }
        {
            QListWidgetItem *item = new QListWidgetItem(ui->listWidget_tab);
            item->setToolTip(GBK2UTF8("我的下载"));
            ui->listWidget_tab->insertItem(0, item);
            ui->listWidget_tab->setItemWidget(item, addRadioButton("QRadioButton::indicator { width: 0px; height: 0px; }"
                                                                       "QRadioButton {"
                                                                       "border:none;"
                                                                       "background-image: url(:/LeftTabList/Resources/LeftTabList/item_download_normal.png);}"
                                                                       "QRadioButton:hover {"
                                                                       "background-image: url(:/LeftTabList/Resources/LeftTabList/item_download_hover.png);}"
                                                                       "QRadioButton:checked {"
                                                                       "background-image: url(:/LeftTabList/Resources/LeftTabList/item_download_clicked.png);}"));
        }
        {
            QListWidgetItem *item = new QListWidgetItem(ui->listWidget_tab);
            item->setToolTip(GBK2UTF8("更多"));
            ui->listWidget_tab->insertItem(0, item);
            ui->listWidget_tab->setItemWidget(item, addRadioButton("QRadioButton::indicator { width: 0px; height: 0px; }"
                                                                       "QRadioButton {"
                                                                       "border:none;"
                                                                       "background-image: url(:/LeftTabList/Resources/LeftTabList/item_more_normal.png);}"
                                                                       "QRadioButton:hover {"
                                                                       "background-image: url(:/LeftTabList/Resources/LeftTabList/item_more_hover.png);}"
                                                                       "QRadioButton:checked {"
                                                                       "background-image: url(:/LeftTabList/Resources/LeftTabList/item_more_clicked.png);}"));
        }
    }
}
