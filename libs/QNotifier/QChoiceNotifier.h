#ifndef QCHOICENOTIFIER_H
#define QCHOICENOTIFIER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScreen>
#include <QApplication>
#include <QEasingCurve>
#include <QPropertyAnimation>

#include "styles.h"

class QChoiceNotifier : public QWidget
{
    Q_OBJECT
public:
    explicit QChoiceNotifier(QWidget* parent = nullptr, const QString& title = "Title", const QString& message = "Message",
                             const QString& yesText = "Yes", const QString& noText = "No",
                             const QIcon& icon = QIcon(), QString widgetStyle = "",
                             const QEasingCurve animCurveIn = QEasingCurve::InQuad,
                             const QEasingCurve animCurveOut = QEasingCurve::OutQuad, const int animPadX = 35,
                             const int animPadY = 35);
    void setupConnection(const int& type, std::function<void()> callback);
    void showNotif();
    void clear();

    static QChoiceNotifier* quickNotif(QWidget* parent = nullptr, const QString& title = "Title", const QString& message = "Message",
                                       const QString& yesText = "Yes!", const QString& noText = "No",
                                       const QIcon& icon = QIcon(), QString widgetStyle = "",
                                       const QEasingCurve animCurveIn = QEasingCurve::InQuad,
                                       const QEasingCurve animCurveOut = QEasingCurve::OutQuad, const int animPadX = 35,
                                       const int animPadY = 35) {
        QChoiceNotifier* notifier = new QChoiceNotifier(parent, title, message, yesText, noText, icon, widgetStyle, animCurveIn, animCurveOut, animPadX, animPadY);
        notifier->showNotif();
        return notifier;
    }

private:
    // Widgets
    styler style;
    QLabel* titleLabel;
    QLabel* messageLabel;
    QLabel* iconWidget;
    QPushButton* yesBtn;
    QPushButton* noBtn;
    QVBoxLayout* mainLayout;

    QWidget* choiceWidget;
    QHBoxLayout* choiceLayout;

    // Animations-Geometry
    QRect rOriginalG;
    QRect rNewG;
    QEasingCurve cAnimIn;
    QEasingCurve cAnimOut;
    QPropertyAnimation* enterAnim;
    QPropertyAnimation* exitAnim;

    // Variables
    QString sTitle, sMessage, sYesText, sNoText;
    QIcon iIcon;
    QString sWidgetStyle;
    int iPadX, iPadY;

    void setupWidgets();
    void setupStyle();
    void setupGeometry();
    void setupAnimations();
};

#endif // QCHOICENOTIFIER_H
