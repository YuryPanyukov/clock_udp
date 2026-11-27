#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>

class QUdpSocket;
class QTimer;

QT_BEGIN_NAMESPACE
namespace Ui {
class ClientWindow;
}
QT_END_NAMESPACE

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow() override;

    void setActual(bool actual);

private slots:
    void onTick();
    void onReadyRead();

private:
    void tryConnected();
    void sheduleReconnected();

    Ui::ClientWindow *ui;
    QUdpSocket *m_socket = nullptr;
    QTimer *m_timer = nullptr;
    QElapsedTimer m_lastServerPacket;
};
#endif // CLIENTWINDOW_H
