#ifndef YOUTUBE_H
#define YOUTUBE_H

#include <QObject>
#include <QUrl>
#include <QProcess>
#include <QStandardPaths>
#include <QDebug>
#include <QFile>
#include <QStandardPaths>
#include <QDesktopServices>

class Youtube : public QObject
{
    Q_OBJECT
public:
    explicit Youtube(QObject *parent = nullptr);

    void download(const QUrl& url, const QString& outFolder);
private slots:
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessErrorOccurred(QProcess::ProcessError error);

signals:
    void complete();

private:
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/youtube-dl";
    QProcess* prc;

};

#endif // YOUTUBE_H
