#include "soundeditor.h"
#include "./ui_soundeditor.h"

#include "player.h"
#include "timing.h"

SoundEditor::SoundEditor(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::SoundEditor)
{
    ui->setupUi(this);

    player = new Player(this);
    timing = new Timing(player, this);

    ui->gridLayout->addWidget(player);
    ui->gridLayout->addWidget(timing);
}

SoundEditor::~SoundEditor()
{
    delete ui;
}
