#include "window.h"

window::window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::window)
    , setts(new QSettings(this))
{
    ui->setupUi(this);
    ui->centralwidget->setStyleSheet(styleSheet);
    update->connect(update, &Updater::updateFinished, this, &window::homePage);

    yt->connect(yt, &Youtube::complete, this, &window::downloadComplete);

    update->checkForUpdate();
    updatePage();
    loadSettings();
}
window::~window() { delete ui; }

void window::loadSettings() {
    path = setts->value("path", QStandardPaths::writableLocation(QStandardPaths::MusicLocation)).toString();
}

void window::resetSettings() {
    if (QMessageBox::Yes == QMessageBox::question(this, tr("Confirmation"), tr("This will delete all your settings. **NOT MP3 FILES** Are you sure?"))) {
        setts->setValue("path", QStandardPaths::writableLocation(QStandardPaths::MusicLocation));

        qApp->quit();
    }
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
    outPathLineEdit->setText(path);
    QPushButton* resetSettingsBtn = createWidget<QPushButton>("Reset settings to default (Restart required)");
    QPushButton* goBackBtn = createWidget<QPushButton>("Go back");

    connect(outPathLineEdit, &QLineEdit::returnPressed, this, [this, outPathLineEdit]() {
        path = outPathLineEdit->text();
        setts->setValue("path", path);
        outPathLineEdit->setText("Updated");
        QTimer::singleShot(1500, this, [this, outPathLineEdit]() {
            outPathLineEdit->setText(path);
        });
    });
    connect(resetSettingsBtn, &QPushButton::clicked, this, &window::resetSettings);
    connect(goBackBtn, &QPushButton::clicked, this, &window::homePage);

    addWidgets(ui->centralwidget->layout(), {titleLabel, outPathLineEdit, resetSettingsBtn, goBackBtn});
}

void window::downloadComplete() {
    canDownload = true;
    homePage();
}

void window::homePage() {
    clearScreen();

    QLabel* titleLabel = createWidget<QLabel>("---HOME PAGE---");
    QLineEdit* downloadLine = createWidget<QLineEdit>("", "Enter url here");
    QPushButton* pasteBtn = createWidget<QPushButton>("Paste");
    QPushButton* downloadBtn = createWidget<QPushButton>("Download!");
    QPushButton* settingsBtn = createWidget<QPushButton>("Settings");
    downloadBtn->setEnabled(canDownload);

    connect(pasteBtn, &QPushButton::clicked, this, [this, downloadLine]() {
        QClipboard* clipboard = QGuiApplication::clipboard();
        downloadLine->setText(clipboard->text());
    });

    connect(downloadBtn, &QPushButton::clicked, this, [this, downloadLine, downloadBtn]() {
        const QUrl& text = QUrl(downloadLine->text());

        if (text.isEmpty()) { downloadLine->setText("Link cannot be empty.");return; }
        if (!text.host().contains("youtube")) { downloadLine->setText("Only youtube links allowed.");return; }

        downloadBtn->setEnabled(false);
        downloadBtn->setText("Downloading please wait..");
        yt->download(QUrl(downloadLine->text()), path);
    });

    connect(settingsBtn, &QPushButton::clicked, this, &window::settingsPage);

    addWidgets(ui->centralwidget->layout(), {titleLabel, downloadLine, pasteBtn, downloadBtn, settingsBtn});
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
