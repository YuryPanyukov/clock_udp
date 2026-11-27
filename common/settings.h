#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

namespace settings {

inline QString profilePath(const QString &fileName)
{
    const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir().mkpath(dir);

    return dir + QLatin1Char('/') + fileName;
}

inline bool saveProfile(const QString &fileName, const QJsonObject &obj)
{
    QFile f(profilePath(fileName));
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;
    f.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));

    return true;
}

inline QJsonObject loadProfile(const QString &fileName, const QJsonObject &defaults)
{
    QFile f(profilePath(fileName));
    if (!f.open(QIODevice::ReadOnly)) {
        saveProfile(fileName, defaults);
        return defaults;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    if (!doc.isObject()) {
        saveProfile(fileName, defaults);
        return defaults;
    }

    QJsonObject merged = defaults;
    const QJsonObject loaded = doc.object();
    for (auto it = loaded.begin(); it != loaded.end(); ++it)
        merged[it.key()] = it.value();
    return merged;
}

}

#endif // SETTINGS_H