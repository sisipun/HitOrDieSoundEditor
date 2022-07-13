#include "sound_editor.h"

#include "ui_sound_editor.h"

SoundEditor::SoundEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SoundEditor)
{
    ui->setupUi(this);
}

SoundEditor::~SoundEditor()
{
    delete ui;
}