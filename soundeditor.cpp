#include "soundeditor.h"
#include "./ui_soundeditor.h"

#include "playerwidget.h"
#include "timingwidget.h"

#include <QKeyEvent>

SoundEditor::SoundEditor(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::SoundEditor)
{
    ui->setupUi(this);

    player = new PlayerWidget(this);
    timing = new TimingWidget(player, this);

    ui->gridLayout->addWidget(player);
    ui->gridLayout->addWidget(timing);

    setFocusPolicy(Qt::StrongFocus);
}

SoundEditor::~SoundEditor()
{
    delete ui;
}

void SoundEditor::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Space) {
        if (player->isPlaying()) {
            player->pause();
        } else {
            player->play();
        }
    }
}
