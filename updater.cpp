#include "updater.h"

Updater::Updater(QObject *parent)
    : QObject{parent}, man(new QNetworkAccessManager(this))
{
#if defined(Q_WS_WIN) || defined(Q_OS_WIN)
    path += ".exe";
#endif
}

void Updater::checkForUpdate() {
    QFileInfo exe(path);
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
    man->get(QNetworkRequest(QUrl("https://api.github.com/repos/ytdl-org/ytdl-nightly/releases/latest")));
}

QString Updater::getCurrentVersion() {
    QProcess* prc = new QProcess(this);
    prc->start(path, QStringList("--version"));

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
            QFile f(path);
            if (f.open(QIODevice::WriteOnly)) {
                f.write(r->readAll());
                f.close();
                #ifdef Q_OS_UNIX
                    QFile::setPermissions(path,
                    QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner |
                    QFile::ReadGroup | QFile::ExeGroup |
                    QFile::ReadOther | QFile::ExeOther);
                #endif
            }
        } else { qDebug() << "Download error: " << r->errorString(); }
        emit updateFinished();
        r->deleteLater();
    });
    man->get(QNetworkRequest("https://github.com/ytdl-org/ytdl-nightly/releases/download/"+versionToInstall+"/youtube-dl"));
    emit updateFound();
}
