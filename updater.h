#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QApplication>
#include <QDesktopServices>

#include "QNotifier.h"

class Updater : public QObject
{
    Q_OBJECT
public:
    explicit Updater(QObject *parent = nullptr, const QString& ver = QString());
    void checkForUpdate();
    void updateMp3Man();
signals:
    void majorUpdateFound();
    void updateFound();
    void updateFinished();
    void noFfmpegWarning(const QString& packageManager = QString());
private:
    QNetworkAccessManager* man;
    QString pathBase = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QString ytPath = pathBase + "/yt-dlp";
    QString ffmpegPath = pathBase + "/ffmpeg";
    QString mp3ManVersion = "";
    QString getCurrentVersion();
    QString getPackageManager();

    void ffmepgCheck();
    void install(const QString& versionToInstall);
    void reInstallMp3Man(const QString& versionToInstall);
    bool shouldInstall(const QString& latest, const QString& current);
};

#endif // UPDATER_H
