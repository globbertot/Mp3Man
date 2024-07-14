#include "window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    window w;

    a.setApplicationDisplayName("MP3 Man - "+w.VERSION);
    a.setApplicationName("MP3 Man - "+w.VERSION);

    w.show();
    return a.exec();
}
