#include "QSliderClicked.h"

QSliderClicked::QSliderClicked(QWidget *parent)
    : QSlider(parent)
{
    m_mousePress = false;
    m_value = 0;
}

void QSliderClicked::mousePressEvent(QMouseEvent *event)
{
    m_mousePress = true;

    QSlider::mousePressEvent(event);

    double pos = event->pos().x() / (double)width();
    m_value = pos * (maximum() - minimum()) + minimum();
}

void QSliderClicked::mouseMoveEvent(QMouseEvent *event)
{
    QSlider::mouseMoveEvent(event);
    if (m_mousePress)
    {
        int x = event->pos().x();
        if ((x >= 0) && (x <= width()))
        {
            double pos = event->pos().x() / (double)width();
            m_value = pos * (maximum() - minimum()) + minimum();
            setValue(m_value);
        }
        else if (x < 0)
        {
            m_value = minimum();
        }
        else
        {
            m_value = maximum();
        }
    }
}

void QSliderClicked::mouseReleaseEvent(QMouseEvent *event)
{
    QSlider::mouseReleaseEvent(event);
    m_mousePress = false;
}

bool QSliderClicked::event(QEvent *event)
{
    if (m_mousePress)
    {
        setValue(m_value);
    }
    return QSlider::event(event);
}

