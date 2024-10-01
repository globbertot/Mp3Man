#ifndef STYLES_H
#define STYLES_H
#include <QString>

class styler {
public:
    QString darkStyle = R"(
    /* Main widget background */
    QWidget {
        background-color: #333333;
        border: 1px solid #555555;
        border-radius: 3px;
    }

    /* Title label styling */
    QLabel#titleLabel {
        color: #ffffff;
        font-size: 16px;
        font-weight: bold;
        padding: 5px;
    }

    /* Message label styling */
    QLabel#messageLabel {
        color: #cccccc;
        font-size: 15px;
    }

    /* Close button styling */
    QPushButton#closeBtn {
        background-color: #555555;
        color: #ffffff;
        border: none;
        padding: 5px;
        margin: 5px;
        border-radius: 3px;
    }

    QPushButton#closeBtn:hover {
        background-color: #666666;
    }

    /* Yes button styling */
    QPushButton#yesBtn {
        background-color: #555555;
        color: #ffffff;
        border: none;
        padding: 5px;
        margin: 5px;
        border-radius: 3px;
    }

    QPushButton#yesBtn:hover {
        background-color: #666666;
    }

    /* No button styling */
    QPushButton#noBtn {
        background-color: #555555;
        color: #ffffff;
        border: none;
        padding: 5px;
        margin: 5px;
        border-radius: 3px;
    }

    QPushButton#noBtn:hover {
        background-color: #666666;
    }
)";
    QString greenStyle = R"(
    /* Main widget background */
    QWidget {
        background-color: #2d3e2d;
        border: 1px solid #3c5d3c;
        border-radius: 3px;
    }

    /* Title label styling */
    QLabel#titleLabel {
        color: #e0f8e0;
        font-size: 16px;
        font-weight: bold;
        padding: 5px;
    }

    /* Message label styling */
    QLabel#messageLabel {
        color: #c8e6c8;
        font-size: 15px;
    }

    /* Close button styling */
    QPushButton#closeBtn {
        background-color: #3c5d3c;
        color: #e0f8e0;
        border: none;
        padding: 5px;
        margin: 5px;
        border-radius: 3px;
    }

    QPushButton#closeBtn:hover {
        background-color: #4d7a4d;
    }

    /* Yes button styling */
    QPushButton#yesBtn {
        background-color: #3c5d3c;
        color: #e0f8e0;
        border: none;
        padding: 5px;
        margin: 5px;
        border-radius: 3px;
    }

    QPushButton#yesBtn:hover {
        background-color: #4d7a4d;
    }

    /* No button styling */
    QPushButton#noBtn {
        background-color: #3c5d3c;
        color: #e0f8e0;
        border: none;
        padding: 5px;
        margin: 5px;
        border-radius: 3px;
    }

    QPushButton#noBtn:hover {
        background-color: #4d7a4d;
    }
)";
    QString purpleStyle = R"(
    /* Main widget background */
    QWidget {
        background-color: #3b2d4f;
        border: 1px solid #564b6b;
        border-radius: 3px;
    }

    /* Title label styling */
    QLabel#titleLabel {
        color: #e8e0f8;
        font-size: 16px;
        font-weight: bold;
        padding: 5px;
    }

    /* Message label styling */
    QLabel#messageLabel {
        color: #d4c8e6;
        font-size: 15px;
    }

    /* Close button styling */
    QPushButton#closeBtn {
        background-color: #564b6b;
        color: #e8e0f8;
        border: none;
        padding: 5px;
        margin: 5px;
        border-radius: 3px;
    }

    QPushButton#closeBtn:hover {
        background-color: #6b5d85;
    }

    /* Yes button styling */
    QPushButton#yesBtn {
        background-color: #564b6b;
        color: #e8e0f8;
        border: none;
        padding: 5px;
        margin: 5px;
        border-radius: 3px;
    }

    QPushButton#yesBtn:hover {
        background-color: #6b5d85;
    }

    /* No button styling */
    QPushButton#noBtn {
        background-color: #564b6b;
        color: #e8e0f8;
        border: none;
        padding: 5px;
        margin: 5px;
        border-radius: 3px;
    }

    QPushButton#noBtn:hover {
        background-color: #6b5d85;
    }
)";
    QString blueStyle = R"(
    /* Main widget background */
    QWidget {
        background-color: #2d3e4f;
        border: 1px solid #3c5d7a;
        border-radius: 3px;
    }

    /* Title label styling */
    QLabel#titleLabel {
        color: #e0f0f8;
        font-size: 16px;
        font-weight: bold;
        padding: 5px;
    }

    /* Message label styling */
    QLabel#messageLabel {
        color: #c8d4e6;
        font-size: 15px;
    }

    /* Close button styling */
    QPushButton#closeBtn {
        background-color: #3c5d7a;
        color: #e0f0f8;
        border: none;
        padding: 5px;
        margin: 5px;
        border-radius: 3px;
    }

    QPushButton#closeBtn:hover {
        background-color: #4d7aa6;
    }

    /* Yes button styling */
    QPushButton#yesBtn {
        background-color: #3c5d7a;
        color: #e0f0f8;
        border: none;
        padding: 5px;
        margin: 5px;
        border-radius: 3px;
    }

    QPushButton#yesBtn:hover {
        background-color: #4d7aa6;
    }

    /* No button styling */
    QPushButton#noBtn {
        background-color: #3c5d7a;
        color: #e0f0f8;
        border: none;
        padding: 5px;
        margin: 5px;
        border-radius: 3px;
    }

    QPushButton#noBtn:hover {
        background-color: #4d7aa6;
    }
)";
    QString redStyle = R"(
    /* Main widget background */
    QWidget {
        background-color: #4f2d2d;
        border: 1px solid #7a3c3c;
        border-radius: 3px;
    }

    /* Title label styling */
    QLabel#titleLabel {
        color: #f8e0e0;
        font-size: 16px;
        font-weight: bold;
        padding: 5px;
    }

    /* Message label styling */
    QLabel#messageLabel {
        color: #e6c8c8;
        font-size: 15px;
    }

    /* Close button styling */
    QPushButton#closeBtn {
        background-color: #7a3c3c;
        color: #f8e0e0;
        border: none;
        padding: 5px;
        margin: 5px;
        border-radius: 3px;
    }

    QPushButton#closeBtn:hover {
        background-color: #a64d4d;
    }

    /* Yes button styling */
    QPushButton#yesBtn {
        background-color: #7a3c3c;
        color: #f8e0e0;
        border: none;
        padding: 5px;
        margin: 5px;
        border-radius: 3px;
    }

    QPushButton#yesBtn:hover {
        background-color: #a64d4d;
    }

    /* No button styling */
    QPushButton#noBtn {
        background-color: #7a3c3c;
        color: #f8e0e0;
        border: none;
        padding: 5px;
        margin: 5px;
        border-radius: 3px;
    }

    QPushButton#noBtn:hover {
        background-color: #a64d4d;
    }
)";
    QString orangeStyle = R"(
    /* Main widget background */
    QWidget {
        background-color: #4f3e2d;
        border: 1px solid #7a5d3c;
        border-radius: 3px;
    }

    /* Title label styling */
    QLabel#titleLabel {
        color: #f8f0e0;
        font-size: 16px;
        font-weight: bold;
        padding: 5px;
    }

    /* Message label styling */
    QLabel#messageLabel {
        color: #e6d4c8;
        font-size: 15px;
    }

    /* Close button styling */
    QPushButton#closeBtn {
        background-color: #7a5d3c;
        color: #f8f0e0;
        border: none;
        padding: 5px;
        margin: 5px;
        border-radius: 3px;
    }

    QPushButton#closeBtn:hover {
        background-color: #a67a4d;
    }

    /* Yes button styling */
    QPushButton#yesBtn {
        background-color: #7a5d3c;
        color: #f8f0e0;
        border: none;
        padding: 5px;
        margin: 5px;
        border-radius: 3px;
    }

    QPushButton#yesBtn:hover {
        background-color: #a67a4d;
    }

    /* No button styling */
    QPushButton#noBtn {
        background-color: #7a5d3c;
        color: #f8f0e0;
        border: none;
        padding: 5px;
        margin: 5px;
        border-radius: 3px;
    }

    QPushButton#noBtn:hover {
        background-color: #a67a4d;
    }
)";
    QString yellowStyle = R"(
    /* Main widget background */
    QWidget {
        background-color: #4f4f2d;
        border: 1px solid #7a7a3c;
        border-radius: 3px;
    }

    /* Title label styling */
    QLabel#titleLabel {
        color: #f8f8e0;
        font-size: 16px;
        font-weight: bold;
        padding: 5px;
    }

    /* Message label styling */
    QLabel#messageLabel {
        color: #e6e6c8;
        font-size: 15px;
    }

    /* Close button styling */
    QPushButton#closeBtn {
        background-color: #7a7a3c;
        color: #f8f8e0;
        border: none;
        padding: 5px;
        margin: 5px;
        border-radius: 3px;
    }

    QPushButton#closeBtn:hover {
        background-color: #a6a64d;
    }

    /* Yes button styling */
    QPushButton#yesBtn {
        background-color: #7a7a3c;
        color: #f8f8e0;
        border: none;
        padding: 5px;
        margin: 5px;
        border-radius: 3px;
    }

    QPushButton#yesBtn:hover {
        background-color: #a6a64d;
    }

    /* No button styling */
    QPushButton#noBtn {
        background-color: #7a7a3c;
        color: #f8f8e0;
        border: none;
        padding: 5px;
        margin: 5px;
        border-radius: 3px;
    }

    QPushButton#noBtn:hover {
        background-color: #a6a64d;
    }
)";

};

#endif // STYLES_H
