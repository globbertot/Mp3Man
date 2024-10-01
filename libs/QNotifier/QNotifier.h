#ifndef QNOTIFIER_H
#define QNOTIFIER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScreen>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QApplication>

#include "styles.h"

class QNotifier : public QWidget
{
    Q_OBJECT

public:
    explicit QNotifier(QWidget* parent = nullptr, const QString& title = "Title", const QString& message = "Message",
                       bool canClose = true, QString widgetStyle = "", const QIcon& icon = QIcon(),
                       const QEasingCurve animCurveIn = QEasingCurve::InQuad,
                       const QEasingCurve animCurveOut = QEasingCurve::OutQuad, const int animPadX = 35,
                       const int animPadY = 35);
    void showNotif(const float timeout = 3.0);
    void clear();

    static QNotifier* quickNotif(QWidget* parent = nullptr, const QString& title = "Title", const QString& message = "Message",
                                 const float timeout = 3, bool canClose = true, QString widgetStyle = "",
                                 const QIcon& icon = QIcon(), const QEasingCurve animCurveIn = QEasingCurve::InQuad,
                                 const QEasingCurve animCurveOut = QEasingCurve::OutQuad, const int animPadX = 35,
                                 const int animPadY = 35) {
        QNotifier* notifier = new QNotifier(parent, title, message, canClose, widgetStyle, icon, animCurveIn, animCurveOut, animPadX, animPadY);
        notifier->showNotif(timeout);
        return notifier;
    }
signals:
    void complete();

private:
    styler style;
    QLabel* titleLabel;
    QLabel* messageLabel;
    QLabel* iconWidget;
    QPushButton* closeBtn;
    QVBoxLayout* mainLayout;

    QRect rOriginalG;
    QRect rNewG;
    QEasingCurve cAnimIn;
    QEasingCurve cAnimOut;
    QPropertyAnimation* enterAnim;
    QPropertyAnimation* exitAnim;

    QString sTitle, sMessage;
    bool bCanClose;
    QIcon iIcon;
    QString sWidgetStyle;
    int iPadX, iPadY;

    void setupWidgets();
    void setupStyle();
    void setupGeometry();
    void setupAnimations();
};

#endif
