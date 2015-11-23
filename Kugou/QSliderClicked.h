#ifndef QSLIDERCLICKED_H
#define QSLIDERCLICKED_H

#include <QSlider>
#include <QMouseEvent>

class QSliderClicked : public QSlider
{
public:
    explicit QSliderClicked(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    bool event(QEvent *event);

private:
    QPoint m_oldMousePos;
    int m_value;
    bool m_mousePress;
};

#endif // QSLIDERCLICKED_H
