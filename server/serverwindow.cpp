#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "protocol.h"
#include "settings.h"

#include <QLabel>
#include <QJsonObject>
#include <QTimeEdit>
#include <QTimer>
#include <QUdpSocket>

namespace {

const QJsonObject kServerDefaults = {
    { "listenPort",      protocol::kDefaultServerPort },
    { "sendIntervalMs",  protocol::kTickMs },
    { "clientTimeoutMs", protocol::kTimeoutMs },
    { "initialTime",     "12:00" },
    };

}

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ServerWindow)
{
    ui->setupUi(this);

    // Вначале загружаем профиль
    const QJsonObject profile = settings::loadProfile("server.json", kServerDefaults);
    const quint16 listenPort = static_cast<quint16>(profile.value("listenPort").toInt(protocol::kDefaultServerPort));
    const int sendIntervalMs = profile.value("sendIntervalMs").toInt(protocol::kTickMs);
    m_clientTimeoutMs = profile.value("clientTimeoutMs").toInt(protocol::kTimeoutMs);
    const QString initialTimeStr = profile.value("initialTime").toString("12:00");

    if (sendIntervalMs <= 0) return;
    if (m_clientTimeoutMs <= 0) m_clientTimeoutMs = protocol::kTimeoutMs;

    m_time = QTime::fromString(initialTimeStr, "HH:mm");
    if (!m_time.isValid()) m_time = QTime(12, 0);

    // Клиент видит время
    setClientSeen(false);

    // Время
    ui->timeEdit->setDisplayFormat("HH:mm");
    updateTimeEditProgrammatic();
    connect(ui->timeEdit, &QTimeEdit::timeChanged,
            this, &ServerWindow::onTimeEdited);

    // UDP сокет
    m_socket = new QUdpSocket(this);
    if (!m_socket->bind(QHostAddress::AnyIPv4, listenPort)) {
        qWarning("Не удалось забиндить UDP-порт %u: %s",
                 static_cast<unsigned>(listenPort),
                 qPrintable(m_socket->errorString()));
    }
    connect(m_socket, &QUdpSocket::readyRead, this, &ServerWindow::onReadyRead);

    // Время
    m_timer = new QTimer(this);
    m_timer->setInterval(sendIntervalMs);
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer, &QTimer::timeout, this, &ServerWindow::onTick);
    m_timer->start();
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::updateTimeEditProgrammatic()
{
    m_programmaticUpdate = true;
    ui->timeEdit->setTime(m_time);
    m_programmaticUpdate = false;
}

void ServerWindow::onTimeEdited(const QTime &time)
{
    if (m_programmaticUpdate)
        return;

    m_time = time;
    sendTimeToClient();
}

void ServerWindow::onTick()
{
    if (!ui->timeEdit->hasFocus()) {
        m_time = m_time.addSecs(60);
        updateTimeEditProgrammatic();
    }

    sendTimeToClient();

    const bool seen = m_hasClient
                      && m_lastPresence.isValid()
                      && m_lastPresence.elapsed() <= m_clientTimeoutMs;
    setClientSeen(seen);
}

void ServerWindow::onReadyRead()
{
    while (m_socket->hasPendingDatagrams()) {
        QByteArray data;
        data.resize(static_cast<int>(m_socket->pendingDatagramSize()));

        QHostAddress sender;
        quint16 senderPort = 0;
        m_socket->readDatagram(data.data(), data.size(), &sender, &senderPort);

        if (data.trimmed() == "PRESENCE") {
            m_clientAddress = sender;
            m_clientPort = senderPort;
            m_hasClient = true;
            m_lastPresence.restart();
        }
    }
}

void ServerWindow::sendTimeToClient()
{
    if (!m_hasClient)
        return;

    m_socket->writeDatagram(protocol::makeTimePacket(m_time),m_clientAddress, m_clientPort);
}

void ServerWindow::setClientSeen(bool seen)
{
    const QString text  = seen ? QStringLiteral("Да") : QStringLiteral("нет");
    const QString color = seen ? QStringLiteral("#4CAF50") : QStringLiteral("#9e9e9e");

    if (ui->statusLabel->text() == text)
        return;

    ui->statusLabel->setText(text);
    ui->statusLabel->setStyleSheet(QStringLiteral("color: %1;").arg(color));

}