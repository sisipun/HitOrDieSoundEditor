#ifndef SOUNDEDITOR_H
#define SOUNDEDITOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class SoundEditor; }
class Player;
QT_END_NAMESPACE

class SoundEditor : public QMainWindow
{
    Q_OBJECT

public:
    SoundEditor(QWidget *parent = nullptr);
    ~SoundEditor();

private:
    Ui::SoundEditor *ui;
    Player *player;
};
#endif // SOUNDEDITOR_H
