#ifndef YOUTUBE_H
#define YOUTUBE_H

#include <QObject>
#include <QUrl>
#include <QProcess>
#include <QStandardPaths>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QString>
#include <QMediaPlayer>
#include <QAudioOutput>

#include "qmediaplayerwidget.h"

class Youtube : public QObject
{
    Q_OBJECT
public:
    explicit Youtube(QObject *parent = nullptr);

    void download(const QUrl& url, const QString& outFolder, const bool& openInITunes = false, const bool& openInDefault = false);
private slots:
    void startDownload(const QUrl& url, const QString& outFolder, const bool& openInItunes, const bool& openInDefault);
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessErrorOccurred(QProcess::ProcessError err);

signals:
    void complete();
    void info(const QString& msg, const float& timeout = 3.0);
    void error(const QString& msg, const float& timeout = 3.0);

private:
    QString getFileName(const QUrl& url, const QString& outFolder);

    QMediaPlayer* player;
    QAudioOutput* audio;
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/yt-dlp";
    QString outPath;
    bool iTunes;
    bool openSong;
    QProcess* prc;
};

#endif // YOUTUBE_H
