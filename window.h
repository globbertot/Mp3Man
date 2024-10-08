#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include "ui_window.h"

#include <QString>
#include <QLayoutItem>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QCheckBox>
#include <QLineEdit>
#include <QRadioButton>
#include <QTimer>
#include <QClipboard>
#include <QUrl>
#include <QSettings>
#include <QMessageBox>
#include <QDir>
#include <QThread>
#include <QDesktopServices>

#include "libs/QNotifier/QNotifier.h"
#include "libs/QNotifier/QChoiceNotifier.h"

#include "updater.h"
#include "youtube.h"

class window : public QMainWindow
{
    Q_OBJECT

public:
    window(QWidget *parent = nullptr);
    ~window();
    const QString VERSION = "V0.0.1";

    void clearScreen();
    template<typename widgetType>
    widgetType* createWidget(const QString& text = "", const QString& placeHolder = "",
                             const QString& objName = "", const QFont& font = QFont("Arial", 12),
                             const bool& wordWrap = true);

    template<typename widgetType>
    void addWidgets(widgetType*, const std::vector<QWidget*>&, const std::vector<Qt::Alignment>& alignments = {});
    void addWidgets(QLayout* layout, const std::vector<QWidget*>& widgets, const std::vector<Qt::Alignment>& alignments = {});

private:
    Ui::window *ui;
    Updater* update;
    Youtube* yt;

    // Pages
    void updatePage();
    void homePage();
    void settingsPage();

    void downloadComplete();
    bool canDownload = true;

    void noFFmpeg(const QString& packageManager);
    void loadSettings();
    void resetSettings();
    QSettings* setts;
    QString path;
    bool windowsUser = false;
    bool bItunesIntergration;
    bool bOpenSongs;

    const QString stylesheet = R"(
    /* QLabel styling */
    QLabel {
        color: #00ffff;
        background-color: #222222;
        border: 1px solid #00ffff;
        padding: 5px;
        border-radius: 5px;
    }

    /* QLineEdit styling */
    QLineEdit {
        color: #ffffff;
        background-color: #333333;
        border: 1px solid #00ffff;
        padding: 5px;
        border-radius: 5px;
    }
    QLineEdit:focus {
        border: 1px solid #66ccff;
    }

    /* QPushButton styling */
    QPushButton {
        color: #00ffff;
        background-color: #222222;
        border: 1px solid #00ffff;
        padding: 5px 10px;
        border-radius: 5px;
    }
    QPushButton:hover {
        background-color: #333333;
        border: 1px solid #66ccff;
    }
    QPushButton:pressed {
        background-color: #111111;
        border: 1px solid #00ffff;
    }
    QPushButton:disabled {
        color: #777777;
        background-color: #222222;
        border: 1px solid #555555;
    }

    /* QCheckBox styling */
    QCheckBox {
        color: #ffffff;
    }

    QCheckBox::indicator {
        background-color: #333333;
        border: 1px solid #00ffff;
        width: 15px;
        height: 15px;
    }

    QCheckBox::indicator:checked {
        background-color: #00ffff;
        border: 1px solid #00ffff;
    }

    QCheckBox::indicator:checked:hover {
        background-color: #66ccff;
        border: 1px solid #66ccff;
    }

    QCheckBox::indicator:unchecked {
        background-color: #333333;
        border: 1px solid #00ffff;
    }

    QCheckBox::indicator:unchecked:hover {
        background-color: #555555;
        border: 1px solid #66ccff;
    }

    QCheckBox::indicator:disabled {
        background-color: #222222;
        border: 1px solid #555555;
    }

    QCheckBox::indicator:checked:disabled {
        background-color: #222222;
        border: 1px solid #555555;
    }

    /* QMainWindow styling */
    QMainWindow {
        background-color: #121212;
    }

    /* Central widget styling */
    #centralwidget {
        background-color: #121212;
    }
    )";
};
#endif // WINDOW_H
