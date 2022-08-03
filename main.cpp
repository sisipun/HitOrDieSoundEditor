#include <QApplication>

#include "player.h"
#include "soundeditor.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    SoundEditor w;
    w.show();

    return a.exec();
}
