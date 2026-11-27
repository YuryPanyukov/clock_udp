#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QByteArray>
#include <QString>
#include <QTime>

namespace protocol {

constexpr quint16 kDefaultServerPort = 45454;
constexpr int     kTimeoutMs         = 3000;
constexpr int     kTickMs            = 1000;

inline QByteArray presencePacket() {
    return QByteArrayLiteral("PRESENCE\n");
}

inline QByteArray makeTimePacket(const QTime &t) {
    return "TIME " + t.toString("HH:mm").toUtf8() + "\n";
}

inline bool parseTimePacket(const QByteArray &line, QTime &out) {
    const QByteArray trimmed = line.trimmed();
    if (!trimmed.startsWith("TIME "))
        return false;
    out = QTime::fromString(QString::fromLatin1(trimmed.mid(5)), "HH:mm");
    return out.isValid();
}

} // namespace protocol

#endif // PROTOCOL_H