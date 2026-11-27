#include "clientwindow.h"
#include "ui_clientwindow.h"
#include "clockface.h"

#include <QButtonGroup>
#include <QPushButton>

#include <QHostAddress>
#include <QUdpSocket>

#include <QTime>
#include <QTimer>

namespace{
constexpr quint16 kServerPort = 45454;
constexpr int kTimeoutMs = 3000;
constexpr int kTickMs = 1000;
const char *const kServerHost = "127.0.0.1";
const QByteArray kPresencePacket = "PRESENCS\n";
}


ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ClientWindow)
{
    ui->setupUi(this);

    auto *group = new QButtonGroup(this);
    group->setExclusive(true);
    group->addButton(ui->btnYes);
    group->addButton(ui->btnNo);

    ui->btnYes->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->btnNo->setAttribute(Qt::WA_TransparentForMouseEvents);

    ui->btnYes->setStyleSheet("QPushButton:checked { background-color: #4CAF50; color: white;}");
    ui->btnNo->setStyleSheet("QPushButton:checked { background-color: #f44336; color: white;}");

    setActual(false);

    // Подключение по UDP
    m_socket = new QUdpSocket(this);
    if (!m_socket->bind(QHostAddress::AnyIPv4, 0)){
        qWarning("Не удалось забиндить сокет: %s", qPrintable(m_socket->errorString()));
    }

    connect(m_socket, &QUdpSocket::readyRead, this, &ClientWindow::onReadyRead);

    // Таймер
    m_timer = new QTimer(this);
    m_timer->setInterval(kTickMs);
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer, &QTimer::timeout, this, &ClientWindow::onTick);
    m_timer->start();

    onTick();
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::onTick()
{
    // Уведомляем сервер о присутствии
    m_socket->writeDatagram(kPresencePacket, QHostAddress(QString::fromLatin1(kServerHost)), kServerPort);

    // Обновляем поле актуальности
    bool actual = m_lastServerPacket.isValid() && m_lastServerPacket.elapsed() <= kTimeoutMs;

    setActual(actual);
}

void ClientWindow::onReadyRead()
{
    while (!m_socket->hasPendingDatagrams()){
        QByteArray data;
        data.resize(static_cast<int>(m_socket->pendingDatagramSize()));
        m_socket->readDatagram(data.data(), data.size());

        // Ожадаем время
        const QByteArray line = data.trimmed();
        if (!line.startsWith("TIME ")){
            continue;
        }

        const QTime time = QTime::fromString(QString::fromLatin1(line.mid(5)), "HH:mm");
        if (!time.isValid()){
            continue;
        }

        ui->clockWidget->setTime(time);
        m_lastServerPacket.restart();

        setActual(true);
    }
}

void ClientWindow::setActual(bool actual)
{
    ui->btnYes->setChecked(actual);
    ui->btnNo->setChecked(!actual);
}


