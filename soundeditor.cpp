#include "soundeditor.h"
#include "./ui_soundeditor.h"

#include "playerwidget.h"
#include "timingwidget.h"

SoundEditor::SoundEditor(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::SoundEditor)
{
    ui->setupUi(this);

    player = new PlayerWidget(this);
    timing = new TimingWidget(player, this);

    ui->gridLayout->addWidget(player);
    ui->gridLayout->addWidget(timing);
}

SoundEditor::~SoundEditor()
{
    delete ui;
}
