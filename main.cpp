#include "soundeditor.h"

#include <QApplication>
#include <QTimeLine>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTimeLine t;
    SoundEditor w;
    w.show();
    return a.exec();
}
