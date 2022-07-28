#include "soundeditor.h"
#include "./ui_soundeditor.h"

#include "player.h"

SoundEditor::SoundEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SoundEditor)
{
    ui->setupUi(this);

    player = new Player(this);
    ui->gridLayout->addWidget(player);
}

SoundEditor::~SoundEditor()
{
    delete ui;
}
