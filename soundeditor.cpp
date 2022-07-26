#include "soundeditor.h"
#include "./ui_soundeditor.h"

SoundEditor::SoundEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SoundEditor)
{
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

