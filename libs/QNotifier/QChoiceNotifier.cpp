#include "QChoiceNotifier.h"

QChoiceNotifier::QChoiceNotifier(QWidget* parent, const QString& title, const QString& message,
                                 const QString& yesText, const QString& noText, const QIcon& icon,
                                 QString widgetStyle, const QEasingCurve animCurveIn,
                                 const QEasingCurve animCurveOut, const int animPadX, const int animPadY) :
    QWidget(parent), sTitle(title), sMessage(message), sYesText(yesText), sNoText(noText), iIcon(icon),
    sWidgetStyle(widgetStyle), cAnimIn(animCurveIn), cAnimOut(animCurveOut), iPadX(animPadX), iPadY(animPadY)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip);
    setMinimumWidth(200);
    setMinimumHeight(200);

    setupWidgets();
    setupGeometry();
    setupAnimations();

    connect(yesBtn, &QPushButton::clicked, this, &QChoiceNotifier::clear);
    connect(noBtn, &QPushButton::clicked, this, &QChoiceNotifier::clear);
}

void QChoiceNotifier::clear() {
    connect(exitAnim, &QPropertyAnimation::finished, this, &QChoiceNotifier::deleteLater);
    exitAnim->start();
}

void QChoiceNotifier::showNotif() {
    enterAnim->start();
    show();
}

void QChoiceNotifier::setupConnection(const int& type, std::function<void()> callback) {
    switch (type) {
        case 0:
            connect(yesBtn, &QPushButton::clicked, this, callback);
            break;
        case 1:
            connect(noBtn, &QPushButton::clicked, this, callback);
            break;
        default:
            qWarning() << "Could not setup callbacks correctly, please check the type.";
    };
}

void QChoiceNotifier::setupWidgets() {
    setupStyle();

    titleLabel = new QLabel(this);
    titleLabel->setObjectName("titleLabel");
    titleLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Maximum);
    titleLabel->setText(sTitle);

    messageLabel = new QLabel(this);
    messageLabel->setObjectName("messageLabel");
    messageLabel->setText(sMessage);
    messageLabel->setWordWrap(true);
    messageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    iconWidget = new QLabel(this);
    iconWidget->setObjectName("iconWidget");
    if (!iIcon.isNull()) {
        iconWidget->setVisible(true);
        iconWidget->setFixedSize(32, 32);
        iconWidget->setPixmap(iIcon.pixmap(32, 32));
    } else {
        iconWidget->setVisible(false);
    }

    choiceWidget = new QWidget(this);
    choiceLayout = new QHBoxLayout(this);

    choiceWidget->setLayout(choiceLayout);

    yesBtn = new QPushButton(this);
    yesBtn->setText(sYesText);
    yesBtn->setObjectName("yesBtn");
    choiceLayout->addWidget(yesBtn, 0, Qt::AlignLeft);

    noBtn = new QPushButton(this);
    noBtn->setText(sNoText);
    noBtn->setObjectName("noBtn");
    choiceLayout->addWidget(noBtn, 0, Qt::AlignRight);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(titleLabel, 0, Qt::AlignHCenter);
    mainLayout->addWidget(iconWidget, 0, Qt::AlignLeft);
    mainLayout->addWidget(messageLabel, 0, Qt::AlignCenter);
    mainLayout->addWidget(choiceWidget, 0, Qt::AlignBottom);

    setLayout(mainLayout);
    setStyleSheet(sWidgetStyle);
    adjustSize();
    show();
}

void QChoiceNotifier::setupStyle() {
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

void QChoiceNotifier::setupGeometry() {
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

void QChoiceNotifier::setupAnimations() {
    enterAnim = new QPropertyAnimation(this, "geometry");
    enterAnim->setEasingCurve(QEasingCurve::InQuad);
    enterAnim->setStartValue(this->geometry());
    enterAnim->setEndValue(rNewG);

    exitAnim = new QPropertyAnimation(this, "geometry");
    enterAnim->setEasingCurve(QEasingCurve::OutQuad);
    exitAnim->setStartValue(rNewG);
    exitAnim->setEndValue(rOriginalG);
}
