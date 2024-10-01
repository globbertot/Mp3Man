#include "QNotifier.h"
#include "qscreen.h"

QNotifier::QNotifier(QWidget* parent, const QString& title, const QString& message,
                     bool canClose, QString widgetStyle, const QIcon& icon, const QEasingCurve animCurveIn,
                     const QEasingCurve animCurveOut, const int animPadX, int animPadY) :
    QWidget(parent), sTitle(title), sMessage(message), bCanClose(canClose),
    iIcon(icon), sWidgetStyle(widgetStyle), cAnimIn(animCurveIn), cAnimOut(animCurveOut),
    iPadX(animPadX), iPadY(animPadY)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip);
    setMinimumWidth(200);
    setMinimumHeight(200);

    setupWidgets();
    setupGeometry();
    setupAnimations();
}

void QNotifier::clear() {
    if (enterAnim->state() == QAbstractAnimation::Running) { enterAnim->stop(); }
    if (exitAnim->state() == QAbstractAnimation::Running) { exitAnim->stop(); }

    emit complete();
    deleteLater();
}

void QNotifier::showNotif(const float timeout) {
    QSequentialAnimationGroup* animations = new QSequentialAnimationGroup(this);
    animations->addAnimation(enterAnim);
    animations->addPause(timeout * 1000);
    animations->addAnimation(exitAnim);

    connect(animations, &QSequentialAnimationGroup::finished, this, &QNotifier::clear);
    animations->start(QAbstractAnimation::DeleteWhenStopped);
    show();
}

void QNotifier::setupWidgets() {
    setupStyle();

    titleLabel = new QLabel(this);titleLabel->setObjectName("titleLabel");
    titleLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Maximum);
    titleLabel->setText(sTitle);

    messageLabel = new QLabel(this);
    messageLabel->setObjectName("messageLabel");
    messageLabel->setText(sMessage);
    messageLabel->setWordWrap(true);
    messageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    iconWidget = new QLabel(this);iconWidget->setObjectName("iconWidget");
    if (!iIcon.isNull()) {
        iconWidget->setVisible(true);
        iconWidget->setFixedSize(32, 32);
        iconWidget->setPixmap(iIcon.pixmap(32, 32));
    } else { iconWidget->setVisible(false); }

    closeBtn = new QPushButton("Close", this);
    closeBtn->setObjectName("closeBtn");
    closeBtn->setVisible(bCanClose);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(titleLabel, 0, Qt::AlignHCenter);
    mainLayout->addWidget(iconWidget, 0, Qt::AlignLeft);
    mainLayout->addWidget(messageLabel, 0, Qt::AlignCenter);
    mainLayout->addWidget(closeBtn, 0, Qt::AlignRight);

    setLayout(mainLayout);
    setStyleSheet(sWidgetStyle);
    adjustSize();

    connect(closeBtn, &QPushButton::clicked, this, &QNotifier::clear);
}

void QNotifier::setupStyle() {
    if (sWidgetStyle.isEmpty() || sWidgetStyle.toLower() == "dark") { sWidgetStyle = style.darkStyle; }
    if (sWidgetStyle.contains("CUSTOM |")) {
        QStringList parts = sWidgetStyle.split("|");
        if (parts.size() == 2) {
            sWidgetStyle = parts.at(1);
        } else { qDebug() << "Incorrect custom stylesheet size: " << parts.size(); }
    }
    if (sWidgetStyle.toLower() == "green") { sWidgetStyle = style.greenStyle; }
    if (sWidgetStyle.toLower() == "purple") { sWidgetStyle = style.purpleStyle; }
    if (sWidgetStyle.toLower() == "blue") { sWidgetStyle = style.blueStyle; }
    if (sWidgetStyle.toLower() == "red") { sWidgetStyle = style.redStyle; }
    if (sWidgetStyle.toLower() == "orange") { sWidgetStyle = style.orangeStyle; }
    if (sWidgetStyle.toLower() == "yellow") { sWidgetStyle = style.yellowStyle; }
}

void QNotifier::setupGeometry() {
    QRect screen = QApplication::primaryScreen()->availableGeometry();
    QSize size = this->size();

    int ogX = screen.right() - size.width();
    int ogY = screen.bottom() - size.height();

    int newX = screen.right() - size.width() - iPadX;
    int newY = screen.bottom() - size.height() - iPadY;

    rOriginalG = QRect(ogX, ogY, size.width(), size.height());
    rNewG = QRect(newX, newY, size.width(), size.height());

    setGeometry(rOriginalG);
}

void QNotifier::setupAnimations() {
    enterAnim = new QPropertyAnimation(this, "geometry");
    enterAnim->setEasingCurve(QEasingCurve::InQuad);
    enterAnim->setStartValue(this->geometry());
    enterAnim->setEndValue(rNewG);

    exitAnim = new QPropertyAnimation(this, "geometry");
    enterAnim->setEasingCurve(QEasingCurve::OutQuad);
    exitAnim->setStartValue(rNewG);
    exitAnim->setEndValue(rOriginalG);
}
