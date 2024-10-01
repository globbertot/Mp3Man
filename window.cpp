#include "window.h"

window::window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::window)
    , setts(new QSettings("Mp3Man", "Mp3Man", this))
    , update(new Updater(this))
    , yt(new Youtube(this))
{
    ui->setupUi(this);
    ui->centralwidget->setStyleSheet(stylesheet);

    yt->connect(yt, &Youtube::complete, this, &window::downloadComplete);
    yt->connect(yt, &Youtube::info, this, [this](const QString& msg, const float& timeout) {
        QNotifier::quickNotif(this, "Mp3Man - Info", msg, timeout, true, "orange");
    });
    yt->connect(yt, &Youtube::error, this, [this](const QString& msg, const float& timeout) {
        QNotifier::quickNotif(this, "Mp3Man - Possible Error", msg, timeout, true, "orange");
    });

    updatePage();
    update->connect(update, &Updater::majorUpdateFound, this, [this]() {
        QDesktopServices::openUrl(QUrl("https://github.com/globbertot/Mp3Man/releases/latest"));
        QNotifier::quickNotif(this, "Mp3Man - Update required",
                                    "There is a new update available, "
                              "you'll now be redirected to install it!", 10);
    });
    update->connect(update, &Updater::updateFinished, this, &window::homePage);
    update->connect(update, &Updater::noFfmpegWarning, this, &window::noFFmpeg);
    update->checkForUpdate();
    loadSettings();
}
window::~window() { delete ui; delete update; delete yt; delete setts; deleteLater(); }

void window::loadSettings() {
#if defined(Q_WS_WIN) || defined(Q_OS_WIN)
    windowsUser = true;
#endif
    bItunesIntergration = setts->value("iTunes", false).toBool();
    bOpenSongs = setts->value("openSongs", false).toBool();
    path = setts->value("path", QStandardPaths::writableLocation(QStandardPaths::MusicLocation)).toString();
}

void window::noFFmpeg(const QString& packageManager) {
    const QString ffmpegInitialText = "FFmpeg was not found in your system, this means that this mp3 files will not be "
                                      "created and error might occur. Downloaded songs will be in a .webm format";
    if (!packageManager.isEmpty()) {
        if (packageManager == "NOT FOUND") {
            QNotifier::quickNotif(this, "WARNING", ffmpegInitialText + ". If you wish to install it, please look up how to do so, on your operating system.", 14, true, "orange");
            return;
        }
        QChoiceNotifier* warning = QChoiceNotifier::quickNotif(
            this, "WARNING", ffmpegInitialText + ". To install it, run the command in a terminal",
            "Copy the command to clipboard", "OK, I'll do it on my own.", QIcon(), "orange");

        warning->setupConnection(0, [this, packageManager]() {
            QClipboard* clipboard = QApplication::clipboard();
            clipboard->setText(packageManager);
        });
    } else {
        QChoiceNotifier* warning = QChoiceNotifier::quickNotif(
            this, "WARNING", ffmpegInitialText, "I need help installing ffmpeg", "Alright thanks!", QIcon(), "orange");

        warning->setupConnection(0, [this]() {
            QDesktopServices::openUrl(QUrl("https://www.wikihow.com/Install-FFmpeg-on-Windows"));
        });
    }
}

void window::resetSettings() {
    QChoiceNotifier* notif = QChoiceNotifier::quickNotif(this, "--WARNING Mp3Man WARNING--","All your **SETTINGS** will be "
                                                        "reset to default. This does not include songs.",
                                                        "Yes", "No", QIcon(), "orange");

    notif->setupConnection(0, [this]() {
        setts->setValue("path", QStandardPaths::writableLocation(QStandardPaths::MusicLocation));
        setts->setValue("iTunes", false);
        setts->setValue("openSongs", false);

        qApp->quit();
    });
}

void window::updatePage() {
    clearScreen();

    QLabel* infoLabel = createWidget<QLabel>("Please wait.. Update may be needed\nThis usually does not take a while");
    infoLabel->setAlignment(Qt::AlignCenter);

    ui->centralwidget->layout()->addWidget(infoLabel);
}

void window::settingsPage() {
    clearScreen();

    QLabel* titleLabel = createWidget<QLabel>("---SETTINGS PAGE---");
    QLineEdit* outPathLineEdit = createWidget<QLineEdit>("", "Mp3 output location (Press enter to submit changes)");
    QCheckBox* openSongCheckbox = createWidget<QCheckBox>("Attempt to open a song when downloaded.");
    QCheckBox* iTunesCheckbox = createWidget<QCheckBox>("Itunes intergration (Opens downloaded mp3 file directly in itunes)");
    outPathLineEdit->setText(path);
    openSongCheckbox->setChecked(bOpenSongs);
    iTunesCheckbox->setChecked(bItunesIntergration);
    iTunesCheckbox->setVisible(windowsUser);

    QPushButton* resetSettingsBtn = createWidget<QPushButton>("Reset settings to default (Restart required)");
    QPushButton* goBackBtn = createWidget<QPushButton>("Go back");

    connect(outPathLineEdit, &QLineEdit::returnPressed, this, [this, outPathLineEdit]() {
        if (!QFileInfo::exists(outPathLineEdit->text())) {
            QNotifier::quickNotif(this, "Mp3Man", "Path was not found, please enter an existing location.", 1.5, true, "orange");
            return;
        }
        path = outPathLineEdit->text();
        setts->setValue("path", path);
        QNotifier::quickNotif(this, "Mp3Man", "Mp3 file will now be downloaded at: "+path, 1.5, true, "orange");
    });
    connect(openSongCheckbox, &QCheckBox::clicked, this, [this, openSongCheckbox]() {
        bOpenSongs   = openSongCheckbox->isChecked();
        setts->setValue("openSongs", bOpenSongs);

        QString integrationStatus = (bOpenSongs ? " Enabled!" : " Disabled!");
        QNotifier::quickNotif(this, "Mp3Man", "Opening songs is now" + integrationStatus, 1.5, true,  "orange");
    });
    connect(iTunesCheckbox, &QCheckBox::clicked, this, [this, iTunesCheckbox]() {
        bItunesIntergration = iTunesCheckbox->isChecked();
        setts->setValue("iTunes", bItunesIntergration);

        QString integrationStatus = (bItunesIntergration ? " Enabled!" : " Disabled!");
        QNotifier::quickNotif(this, "Mp3Man", "ITunes intergration is now" + integrationStatus, 1.5, true,  "orange");
    });
    connect(resetSettingsBtn, &QPushButton::clicked, this, &window::resetSettings);
    connect(goBackBtn, &QPushButton::clicked, this, &window::homePage);

    addWidgets(ui->centralwidget->layout(), {titleLabel, outPathLineEdit, openSongCheckbox, iTunesCheckbox ,resetSettingsBtn, goBackBtn});
}

void window::downloadComplete() {
    canDownload = true;
    homePage();
}
 // https://www.youtube.com/watch?v=Loj6efL0M-g
void window::homePage() {
    clearScreen();

    QLabel* titleLabel = createWidget<QLabel>("---HOME PAGE---");
    QLineEdit* downloadLine = createWidget<QLineEdit>("", "Enter url here");
    QPushButton* pasteBtn = createWidget<QPushButton>("Paste");
    QPushButton* clearBtn = createWidget<QPushButton>("Clear");
    QPushButton* downloadBtn = createWidget<QPushButton>("Download!");
    QPushButton* openYtBtn = createWidget<QPushButton>("Open youtube");
    QPushButton* settingsBtn = createWidget<QPushButton>("Settings");
    downloadBtn->setEnabled(canDownload);

    connect(downloadLine, &QLineEdit::returnPressed, this, [this, downloadBtn]() { downloadBtn->click(); });

    connect(pasteBtn, &QPushButton::clicked, this, [this, downloadLine]() {
        QClipboard* clipboard = QGuiApplication::clipboard();
        downloadLine->setText(clipboard->text());
        downloadLine->setFocus();
    });

    connect(clearBtn, &QPushButton::clicked, this, [this, downloadLine]() {
        downloadLine->setText("");
        downloadLine->setFocus();
    });

    connect(downloadBtn, &QPushButton::clicked, this, [this, downloadLine, downloadBtn]() {
        const QUrl& text = QUrl(downloadLine->text());

        if (text.isEmpty()) { downloadLine->setText("Link cannot be empty.");return; }
        if (!text.host().contains("youtube")) { downloadLine->setText("Only youtube links allowed.");return; }

        downloadBtn->setEnabled(false);
        downloadBtn->setText("Downloading please wait..");
        yt->download(QUrl(downloadLine->text()), path, bItunesIntergration, bOpenSongs);
    });

    connect(openYtBtn, &QPushButton::clicked, this, [this]() {
        QChoiceNotifier* notif = QChoiceNotifier::quickNotif(this, "Mp3Man", "Which youtube version would you like to use?", "Youtube Music", "Youtube", QIcon(), "orange");
        notif->setupConnection(0, [this]() { QDesktopServices::openUrl(QUrl("https://music.youtube.com/")); });
        notif->setupConnection(1, [this]() { QDesktopServices::openUrl(QUrl("https://www.youtube.com/")); });
    });
    connect(settingsBtn, &QPushButton::clicked, this, &window::settingsPage);

    addWidgets(ui->centralwidget->layout(), {titleLabel, downloadLine, pasteBtn, clearBtn, downloadBtn, openYtBtn, settingsBtn});
}


template<typename widgetType>
void window::addWidgets(widgetType* widget, const std::vector<QWidget*>& widgets, const std::vector<Qt::Alignment>& alignments) {
    if (alignments.size() > 0 && alignments.size() != widgets.size()) {
        qDebug() << "Alignemnts do not match the widgets to be added";
        return;
    }

    for (size_t i =0; i<widgets.size(); ++i) {
        QWidget* w = widgets[i];
        if (w && widget) {
            Qt::Alignment alg = alignments.empty() ? Qt::Alignment() : alignments[i];
            widget->addWidget(w, 0, alg);
        }
    }
}
// Overload for layouts
void window::addWidgets(QLayout* layout, const std::vector<QWidget*>& widgets, const std::vector<Qt::Alignment>& alignments) {
    if (alignments.size() > 0 && widgets.size() != alignments.size()) {
        qWarning() << "Warning: Number of widgets does not match number of alignments.";
        return;
    }

    for (size_t i = 0; i < widgets.size(); ++i) {
        QWidget* w = widgets[i];
        if (w && layout) {
            layout->addWidget(w);
        }
    }
}

template<typename widgetType>
widgetType* window::createWidget(const QString& text, const QString& placeHolder,
                                 const QString& objName, const QFont& font, const bool& wordWrap) {
    widgetType* w = new widgetType();
    if constexpr (!std::is_same_v<widgetType, QLineEdit> && !std::is_same_v<widgetType, QScrollArea>) {
        w->setText(text);
        if constexpr (!std::is_same_v<widgetType, QPushButton> && !std::is_same_v<widgetType, QCheckBox> && !std::is_same_v<widgetType, QRadioButton>) {
            w->setWordWrap(wordWrap);
        }
    }
    else {
        if constexpr (!std::is_same_v<widgetType, QScrollArea>) {
            w->setPlaceholderText(placeHolder);
        }
    }
    w->setObjectName(objName);
    w->setFont(font);
    return w;
}

void window::clearScreen() {
    QLayout* layout = ui->centralwidget->layout();
    if (!layout) {
        layout = new QVBoxLayout();
        layout->setSpacing(10);
        ui->centralwidget->setLayout(layout);
    }

    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->disconnect();
            item->widget()->deleteLater();
        }
        delete item;
    }
}
