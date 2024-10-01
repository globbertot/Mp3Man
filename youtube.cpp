#include "youtube.h"
#include <QProcess>
#include <QDebug>

Youtube::Youtube(QObject *parent)
    : QObject{parent}, player(new QMediaPlayer(this)), audio(new QAudioOutput(this))
{}

void Youtube::download(const QUrl& url, const QString& outFolder, const bool& openInItunes, const bool& openInDefault) {
    if (!url.host().contains("youtube")) {
        emit error("Invalid URL: Only Youtube links are accepted.");
        emit complete();
        return;
    }

    QStringList arguments;
    arguments << "--print" << "filename" << "-o" << "%(title)s.mp3" << "--restrict-filenames" << url.toString();

    QProcess* filenameProcess = new QProcess(this);
    connect(filenameProcess, &QProcess::finished, this, [=](int exitCode, QProcess::ExitStatus exitStatus) {
        if (exitStatus == QProcess::NormalExit && exitCode == 0) {
            QByteArray data = filenameProcess->readAllStandardOutput();
            if (!data.isNull() && !data.isEmpty()) {
                outPath = QString::fromUtf8(data.trimmed());
                startDownload(url, outFolder, openInItunes, openInDefault);
            } else {
                emit error("Error: Could not retrive the file name");
                emit complete();
            }
        } else {
            emit error("Process returned code: " + QString::number(exitCode));
            emit complete();
        }
        filenameProcess->deleteLater();
    });
    filenameProcess->start(path, arguments);
}

void Youtube::startDownload(const QUrl& url, const QString& outFolder, const bool& openInItunes, const bool& openInDefault) {
    prc = new QProcess(this);
    QStringList arguments;
    arguments << "-x" << "--audio-format" << "mp3" << url.toString();
    arguments << "-o" << outPath;

    connect(prc, &QProcess::finished, this, &Youtube::onProcessFinished);
    connect(prc, &QProcess::errorOccurred, this, &Youtube::onProcessErrorOccurred);

    iTunes = openInItunes;
    openSong = openInDefault;
    prc->start(path, arguments);
}

void Youtube::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        if (iTunes || openSong) {
            QFile song(outPath);
            if (song.exists()) {
                if (iTunes) {
                    emit info("Path found: Attempting to open ITunes..", 2.5);
                    bool success = QProcess::startDetached("itunes.exe", QStringList() << outPath);

                    if (success) {
                        emit info("ITunes: Success!");
                    } else {
                        // A small update: Actually provide more feedback here
                        emit error("ITunes: Unknown error", 2.5);
                    }
                } else {
                    emit info("Path found: Attempting to open the song", 2.5);

                    QMediaPlayerWidget* player = new QMediaPlayerWidget(nullptr, QUrl::fromLocalFile(outPath));
                    player->show();
                }
            } else {
                qDebug() << outPath;
                emit error("Path not found, song will not be opened.", 5.2);
            }
        } else { emit info("Download complete!", 5); }
    } else {
        emit error("Process (youtube-dlp) return code: "+QString::number(exitCode), 10); // TODO: Copy the yt-dlp log to clipboard
    }

    emit complete();
    prc->deleteLater();
}

void Youtube::onProcessErrorOccurred(QProcess::ProcessError err) {
    emit error("Fatal error: "+prc->errorString(), 10.5);
    prc->deleteLater();
}
