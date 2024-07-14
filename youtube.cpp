#include "youtube.h"
#include <QProcess>
#include <QDebug>

Youtube::Youtube(QObject *parent)
    : QObject{parent}
{}

void Youtube::download(const QUrl& url, const QString& outFolder) {
    if (!url.host().contains("youtube")) {
        qDebug() << "Invalid URL: not a YouTube link.";
        emit complete();
        return;
    }

    prc = new QProcess(this);
    QStringList arguments;
    arguments << "-x" << "--audio-format" << "mp3" << url.toString();
    arguments << "-o" << outFolder + "/%(title)s.%(ext)s";
    connect(prc, &QProcess::finished, this, &Youtube::onProcessFinished);
    connect(prc, &QProcess::errorOccurred, this, &Youtube::onProcessErrorOccurred);

    prc->start(path, arguments);
}

void Youtube::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        emit complete();
    } else {
        qDebug() << "Process finished with exit code:" << exitCode;
    }

    prc->deleteLater();
}

void Youtube::onProcessErrorOccurred(QProcess::ProcessError error) {
    qDebug() << "Error:" << prc->errorString();
    prc->deleteLater();
}
