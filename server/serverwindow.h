#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include <QElapsedTimer>
#include <QTime>

class QUdpSocket;
class QTimer;

QT_BEGIN_NAMESPACE
namespace Ui { class ServerWindow; }
QT_END_NAMESPACE

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow() override;

private slots:
    void onTick();                    // 1 Гц
    void onReadyRead();               // приём PRESENCE
    void onTimeEdited(const QTime &); // пользователь изменил поле

private:
    void updateTimeEditProgrammatic();
    void sendTimeToClient();
    void setClientSeen(bool seen);

    Ui::ServerWindow *ui;
    QUdpSocket  *m_socket    = nullptr;
    QTimer      *m_timer     = nullptr;

    QTime        m_time{12, 0};
    QHostAddress m_clientAddress;
    quint16      m_clientPort = 0;
    bool         m_hasClient  = false;
    bool         m_programmaticUpdate = false;   // guard для QTimeEdit

    QElapsedTimer m_lastPresence;
};

#endif // SERVERWINDOW_H