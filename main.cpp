#include <QApplication>

#include "sound_editor.h"

#include <iostream>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    SoundEditor w;
    std::cout << "Hello" << std::endl;
    w.show();
    return a.exec();
}