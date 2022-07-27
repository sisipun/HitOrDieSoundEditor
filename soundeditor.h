#ifndef SOUNDEDITOR_H
#define SOUNDEDITOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class SoundEditor; }
class QMediaPlayer;
class QAudioOutput;
QT_END_NAMESPACE

class SoundEditor : public QMainWindow
{
    Q_OBJECT

public:
    SoundEditor(QWidget *parent = nullptr);
    ~SoundEditor();

private slots:
    void on_timeline_valueChanged(int value);

    void on_loadsound_clicked();

private:
    Ui::SoundEditor *ui;
    QString soundFileName;
    QMediaPlayer *mediaPlayer;
    QAudioOutput *audioOutput;
};
#endif // SOUNDEDITOR_H
