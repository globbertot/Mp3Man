#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>

class Updater : public QObject
{
    Q_OBJECT
public:
    explicit Updater(QObject *parent = nullptr);
    void checkForUpdate();
signals:
    void updateFound();
    void updateFinished();
private:
    QNetworkAccessManager* man;
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/youtube-dl";
    QString getCurrentVersion();
    void install(const QString& versionToInstall);
    bool shouldInstall(const QString& latest, const QString& current);
};

#endif // UPDATER_H
