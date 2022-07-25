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

