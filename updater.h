#include "updater.h"

Updater::Updater(QObject *parent, const QString& ver)
    : QObject{parent}, man(new QNetworkAccessManager(this))
{
#if defined(Q_WS_WIN) || defined(Q_OS_WIN)
    ytPath += ".exe";
    ffmpegPath += ".exe";
#endif
    mp3ManVersion = ver;
}

void Updater::updateMp3Man() {
    connect(man, &QNetworkAccessManager::finished, this, [this](QNetworkReply* r) {
        if (r->error() == QNetworkReply::NoError) {
            const QJsonDocument json = QJsonDocument::fromJson(r->readAll());
            const QJsonObject obj = json.object();
            const QString latest = obj["tag_name"].toString();

            const QString current = mp3ManVersion;
            qDebug() << latest << current;

            if (shouldInstall(latest, current)) {
                qDebug() << "Update found | Mp3Man";
                emit majorUpdateFound();
            } else {
                qDebug() << "No update, latest version running. || Mp3Man";
                emit updateFinished();
            }
        } else {
            qDebug() << "Network error: " << r->errorString();
        }
        r->deleteLater();
    });
    man->get(QNetworkRequest(QUrl("https://api.github.com/repos/globbertot/Mp3Man/releases/latest")));
}

QString Updater::getPackageManager() {
    if (QFile::exists("/usr/bin/apt")) {
        return "sudo apt install ffmpeg";
    } else if (QFile::exists("/usr/bin/dnf")) {
        return "sudo dnf install ffmpeg";
    } else if (QFile::exists("/usr/bin/yum")) {
        return "sudo yum install ffmpeg";
    } else if (QFile::exists("/usr/bin/pacman")) {
        return "sudo pacman -S ffmpeg";
    } else if (QFile::exists("/usr/bin/zypper")) {
        return "sudo zypper install ffmpeg";
    } else if (QFile::exists("/usr/bin/emerge")) {
        return "sudo emerge media-video/ffmpeg";
    } else if (QFile::exists("/usr/bin/snap")) {
        return "sudo snap install ffmpeg";
    } else if (QFile::exists("/usr/bin/flatpak")) {
        return "flatpak install flathub org.ffmpeg.ffmpeg";
    } else {
        return "NOT FOUND";
    }
}

void Updater::ffmepgCheck() {
    #ifdef Q_OS_UNIX
        QProcess prc;
        prc.start("ffmpeg", QStringList() << "-version");
        prc.waitForFinished();

        bool success = prc.exitStatus() == QProcess::NormalExit && prc.exitCode() == 0;
        if (!success) {
            emit noFfmpegWarning(getPackageManager());
        } else { qDebug() << "All good fffmepg exists! "; }

        return;
    #endif

    QFileInfo f(ffmpegPath);
    if (f.exists()) { qDebug() << "All good fffmepg exists! ";return; }

    QFile old(QApplication::applicationDirPath() + "/ffmpeg.exe");
    if (old.exists()) {
        old.rename(ffmpegPath);
        qDebug() << "Moved!";
        return;
    }

    emit noFfmpegWarning();
}

void Updater::checkForUpdate() {
    QDir dirPath(pathBase);
    if (!dirPath.exists()) {
        dirPath.mkpath(pathBase);
    }
    updateMp3Man();
    ffmepgCheck();

    QFileInfo exe(ytPath);
    bool shouldCheck = true;

    if (!exe.exists() || !exe.isExecutable()) {
        qDebug() << "Youtube-dl not found or not an executable";
        shouldCheck = false;
    }

    disconnect(man, &QNetworkAccessManager::finished, nullptr, nullptr);
    connect(man, &QNetworkAccessManager::finished, this, [this, shouldCheck](QNetworkReply* r) {
        if (r->error() == QNetworkReply::NoError) {
            const QJsonDocument json = QJsonDocument::fromJson(r->readAll());
            const QJsonObject obj = json.object();
            const QString latest = obj["tag_name"].toString();

            if (!shouldCheck) {
                install(latest);
                return;
            }

            const QString current = getCurrentVersion();
            qDebug() << latest << current;

            if (shouldInstall(latest, current)) {
                qDebug() << "Time again, " << latest << current << shouldCheck;
                install(latest);
            } else {
                qDebug() << "No update, latest version running.";
                emit updateFinished();
            }
        } else {
            qDebug() << "Network error: " << r->errorString();
        }
        r->deleteLater();
    });
    man->get(QNetworkRequest(QUrl("https://api.github.com/repos/yt-dlp/yt-dlp/releases/latest")));
}

QString Updater::getCurrentVersion() {
    QProcess* prc = new QProcess(this);
    prc->start(ytPath, QStringList("--version"));

    if (prc->waitForFinished()) {
        QByteArray out = prc->readAllStandardOutput();
        return QString::fromUtf8(out).trimmed();
    } else {
        qDebug() << "Failed to get version";
    }
    prc->close();
    prc->deleteLater();
    return QString();
}

bool Updater::shouldInstall(const QString& latest, const QString& current) {
    return latest != current;
}

void Updater::install(const QString& versionToInstall) {
    disconnect(man, &QNetworkAccessManager::finished, nullptr, nullptr);
    connect(man, &QNetworkAccessManager::finished, this, [this](QNetworkReply* r) {
        if (r->error() == QNetworkReply::NoError) {
            QFile f(ytPath);
            if (f.open(QIODevice::WriteOnly)) {
                f.write(r->readAll());
                f.close();
                #ifdef Q_OS_UNIX
                    QFile::setPermissions(ytPath,
                    QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner |
                    QFile::ReadGroup | QFile::ExeGroup |
                    QFile::ReadOther | QFile::ExeOther);
                #endif
            }
        } else { qDebug() << "Download error: " << r->errorString(); }
        emit updateFinished();
        r->deleteLater();
    });
    QString url = "https://github.com/yt-dlp/yt-dlp/releases/download/"+versionToInstall+"/yt-dlp";
#if defined(Q_WS_WIN) || defined(Q_OS_WIN)
    url += ".exe";
#endif
    man->get(QNetworkRequest(QUrl(url)));
    emit updateFound();
}
