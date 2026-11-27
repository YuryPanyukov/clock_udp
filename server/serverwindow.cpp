#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "protocol.h"

#include <QButtonGroup>
#include <QPushButton>
#include <QTimeEdit>
#include <QTimer>
#include <QUdpSocket>

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ServerWindow)
{
    ui->setupUi(this);

    // --- индикатор «Клиент видит время: Да/Нет» ---
    auto *group = new QButtonGroup(this);
    group->setExclusive(true);
    group->addButton(ui->btnYes);
    group->addButton(ui->btnNo);

    ui->btnYes->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->btnNo ->setAttribute(Qt::WA_TransparentForMouseEvents);

    ui->btnYes->setStyleSheet(
        "QPushButton:checked { background-color: #4CAF50; color: white; }");
    ui->btnNo->setStyleSheet(
        "QPushButton:checked { background-color: #f44336; color: white; }");

    setClientSeen(false);

    // --- поле времени ---
    ui->timeEdit->setDisplayFormat("HH:mm");
    updateTimeEditProgrammatic();   // выставить m_time без «пользовательского» сигнала
    connect(ui->timeEdit, &QTimeEdit::timeChanged,
            this, &ServerWindow::onTimeEdited);

    // --- UDP-сокет ---
    m_socket = new QUdpSocket(this);
    if (!m_socket->bind(QHostAddress::AnyIPv4, protocol::kDefaultServerPort)) {
        qWarning("Не удалось забиндить UDP-порт %u: %s",
                 protocol::kDefaultServerPort,
                 qPrintable(m_socket->errorString()));
    }
    connect(m_socket, &QUdpSocket::readyRead,
            this, &ServerWindow::onReadyRead);

    // --- таймер 1 Гц ---
    m_timer = new QTimer(this);
    m_timer->setInterval(protocol::kTickMs);
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

void ServerWindow::onTimeEdited(const QTime &t)
{
    if (m_programmaticUpdate)
        return;                     // это наша же установка, не пользователь

    m_time = t;
    sendTimeToClient();             // 2.3: сразу отправить новое время
}

void ServerWindow::onTick()
{
    // 2.5: если пользователь НЕ редактирует поле — +1 минута
    if (!ui->timeEdit->hasFocus()) {
        m_time = m_time.addSecs(60);
        updateTimeEditProgrammatic();
    }

    // 2.1: раз в секунду отправляем время клиенту
    sendTimeToClient();

    // 2.4: «Да», если PRESENCE был не позже 3 с назад
    const bool seen = m_hasClient
                      && m_lastPresence.isValid()
                      && m_lastPresence.elapsed() <= protocol::kTimeoutMs;
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
            m_clientPort    = senderPort;
            m_hasClient     = true;
            m_lastPresence.restart();
        }
        // прочие датаграммы игнорируем
    }
}

void ServerWindow::sendTimeToClient()
{
    if (!m_hasClient)
        return;
    m_socket->writeDatagram(protocol::makeTimePacket(m_time),
                            m_clientAddress, m_clientPort);
}

void ServerWindow::setClientSeen(bool seen)
{
    ui->btnYes->setChecked(seen);
    ui->btnNo ->setChecked(!seen);
}