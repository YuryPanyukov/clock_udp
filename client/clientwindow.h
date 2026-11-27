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
    Ui::ClientWindow *ui;
    QUdpSocket *m_socket = nullptr;
    QTimer *m_timer = nullptr;
    QElapsedTimer m_lastServerPacket;

    QString m_serverHost = QStringLiteral("127.0.0.1");
    quint16 m_serverPort = 45454;
    int m_presenceInterval = 1000;
    int m_serverTimeoutMs = 3000;

};
#endif // CLIENTWINDOW_H
