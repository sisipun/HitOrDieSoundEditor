#include "timing.h"

#include "player.h"

#include <QtWidgets>

Timing::Timing(Player *player, QWidget *parent)
    : QWidget{parent}
{
    this->player = player;

    actions = new QComboBox(this);
    actions->addItems(QStringList({"FIRE", "RELOAD"}));
    actions->setCurrentIndex(0);

    addButton = new QPushButton(this);
    addButton->setText(tr("Add"));
    connect(addButton, &QPushButton::clicked, this, &Timing::onAddButtonClicked);

    timingsView = new QListWidget(this);

    QBoxLayout *layout = new QVBoxLayout(this);

    QBoxLayout *listLayout = new QHBoxLayout(this);
    listLayout->addWidget(timingsView);

    QBoxLayout *actionsLayout = new QHBoxLayout(this);
    actionsLayout->addWidget(actions);
    actionsLayout->addWidget(addButton);

    layout->addLayout(listLayout);
    layout->addLayout(actionsLayout);

    setLayout(layout);
}

void Timing::onAddButtonClicked()
{
    if (player->isLoaded())
    {
        float seconds = player->getSeconds();
        QString action = actions->currentText();
        timings[seconds] = action;
        timingsView->addItem(QString("%1 - %2").arg(seconds).arg(action));
    }
}
