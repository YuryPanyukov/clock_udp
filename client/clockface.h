#ifndef CLOCKFACE_H
#define CLOCKFACE_H

#pragma once
#include <QWidget>
#include <QTime>

class ClockFace : public QWidget{
    Q_OBJECT
public:
    explicit ClockFace(QWidget* parent = nullptr);
    void setTime(const QTime &time);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QTime m_time{12, 0};
};

#endif // CLOCKFACE_H
