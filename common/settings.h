#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

namespace settings {

inline QString profilePath(const QString &fileName) {
    const QString dir =
        QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir().mkpath(dir);
    return dir + QLatin1Char('/') + fileName;
}

inline QJsonObject loadProfile(const QString &fileName,
                               const QJsonObject &defaults = {}) {
    QFile f(profilePath(fileName));
    if (!f.open(QIODevice::ReadOnly))
        return defaults;
    const auto doc = QJsonDocument::fromJson(f.readAll());
    return doc.isObject() ? doc.object() : defaults;
}

inline bool saveProfile(const QString &fileName, const QJsonObject &obj) {
    QFile f(profilePath(fileName));
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;
    f.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
    return true;
}

} // namespace settings

#endif // SETTINGS_H