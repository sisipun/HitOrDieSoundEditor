#include "soundeditor.h"
#include "./ui_soundeditor.h"

#include <QFileDialog>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QAudioOutput>

SoundEditor::SoundEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SoundEditor)
{
    mediaPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    mediaPlayer->setAudioOutput(audioOutput);
    ui->setupUi(this);
}

SoundEditor::~SoundEditor()
{
    delete ui;
}

void SoundEditor::on_timeline_valueChanged(int value)
{
    int maxSeconds = QTime(0, 0, 0).secsTo(ui->timer->maximumTime());
    int seconds = (value / (100.0f - 1.0f)) * maxSeconds;
    ui->timer->setTime(QTime::fromMSecsSinceStartOfDay(seconds * 1000));
}


void SoundEditor::on_loadsound_clicked()
{
    QString soundFileName = QFileDialog::getOpenFileName(this, tr("Open sound"), QString(), tr("Sound Files (*.wav)"));
    if (!soundFileName.isEmpty())
    {
        this->soundFileName = soundFileName;
        ui->soundfilename->setText(soundFileName);
        mediaPlayer->setSource(QUrl::fromLocalFile(this->soundFileName));
        mediaPlayer->play();
    }
}

