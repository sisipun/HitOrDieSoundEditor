#include "timing.h"

#include "player.h"

#include <QtWidgets>
#include <QtDebug>

Timing::Timing(Player *player, QWidget *parent)
    : QWidget{parent}
{
    this->player = player;
    connect(this->player, &Player::loaded, this, &Timing::onPlayerLoaded);

    timingsView = new QListWidget(this);

    actions = new QComboBox(this);
    actions->addItems(QStringList({"FIRE", "RELOAD"}));
    actions->setCurrentIndex(0);
    actions->setDisabled(true);

    addButton = new QPushButton(this);
    addButton->setText(tr("+"));
    addButton->setDisabled(true);
    connect(addButton, &QPushButton::clicked, this, &Timing::onAddButtonClicked);

    removeButton = new QPushButton(this);
    removeButton->setText(tr("-"));
    removeButton->setDisabled(true);
    connect(removeButton, &QPushButton::clicked, this, &Timing::onRemoveButtonClicked);

    QBoxLayout *layout = new QVBoxLayout(this);

    QBoxLayout *listLayout = new QHBoxLayout(this);
    listLayout->addWidget(timingsView);

    QBoxLayout *actionsLayout = new QHBoxLayout(this);
    actionsLayout->addWidget(actions);
    actionsLayout->addWidget(addButton);
    actionsLayout->addWidget(removeButton);

    layout->addLayout(listLayout);
    layout->addLayout(actionsLayout);

    setLayout(layout);
}

void Timing::onPlayerLoaded(bool loaded)
{
    actions->setDisabled(!loaded);
    addButton->setDisabled(!loaded);
    removeButton->setDisabled(!loaded);
}

void Timing::onAddButtonClicked()
{
    float seconds = player->getSeconds();
    QString action = actions->currentText();
    timings[seconds] = action;
    QListWidgetItem *item = new QListWidgetItem(QString("%1 - %2").arg(seconds).arg(action), timingsView);
    item->setData(Qt::UserRole, seconds);
}

void Timing::onRemoveButtonClicked()
{
    for (QListWidgetItem *item : timingsView->selectedItems())
    {
        timings.remove(item->data(Qt::UserRole).toFloat());
    }

    qDeleteAll(timingsView->selectedItems());
}
