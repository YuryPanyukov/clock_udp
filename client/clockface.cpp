#include "clockface.h"
#include <QPainter>
#include <QtMath>

ClockFace::ClockFace(QWidget *parent): QWidget(parent){
    setMinimumSize(300, 300);
}

void ClockFace::setTime(const QTime &time){
    m_time = time;
    update();
}

void ClockFace::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const QPointF center(width() / 2, height() / 2);
    const double side = qMin(width(), height());
    const double radius = side / 2 - 10;

    // Циферблат
    painter.setPen(QPen(Qt::black, 3));
    painter.setBrush(Qt::white);
    painter.drawEllipse(center, radius, radius);

    // Метки
    painter.setPen(QPen(Qt::black, 2));
    for ( int i = 1 ; i <= 12 ; i++ ){
        const double angle = i * 30.0 * M_PI / 180.0 - M_PI / 2.0;
        const QPointF start = center + QPointF(qCos(angle), qSin(angle)) * (radius - 15);
        const QPointF end = center + QPointF(qCos(angle), qSin(angle)) * (radius - 5);
        painter.drawLine(start, end);
    }

    // Часовая стрелка
    const double hourDeg = (m_time.hour() % 12 + m_time.minute() / 60) * 30.0;
    painter.save();
    painter.translate(center);
    painter.rotate(hourDeg);
    painter.setPen(QPen(Qt::black, 6, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(QPointF(0, 0), QPointF(0, -radius * 0.5));
    painter.restore();

    // Минутная стрелка
    const double minuteDeg = m_time.minute() * 6.0;
    painter.save();
    painter.translate(center);
    painter.rotate(minuteDeg);
    painter.setPen(QPen(Qt::black, 4, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(QPointF(0, 0), QPointF(0, -radius * 0.75));
    painter.restore();
}