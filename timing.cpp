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
    timingsView->setSortingEnabled(true);

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

    exportButton = new QPushButton(this);
    exportButton->setText(tr("Export"));
    exportButton->setDisabled(true);
    connect(exportButton, &QPushButton::clicked, this, &Timing::onExportButtonClicked);

    QBoxLayout *layout = new QVBoxLayout(this);

    QBoxLayout *listLayout = new QHBoxLayout;
    listLayout->addWidget(timingsView);

    QBoxLayout *actionsLayout = new QHBoxLayout;
    actionsLayout->addWidget(actions);
    actionsLayout->addWidget(addButton);
    actionsLayout->addWidget(removeButton);
    actionsLayout->addWidget(exportButton);

    layout->addLayout(listLayout);
    layout->addLayout(actionsLayout);

    setLayout(layout);
}

void Timing::onPlayerLoaded(bool loaded)
{
    actions->setDisabled(!loaded);
    addButton->setDisabled(!loaded);
    removeButton->setDisabled(!loaded);
    exportButton->setDisabled(!loaded);
}

void Timing::onAddButtonClicked()
{
    float seconds = player->getSeconds();
    QString action = actions->currentText();
    timings[seconds] = action;
    reloadTimingsView();
}

void Timing::onRemoveButtonClicked()
{
    for (QListWidgetItem *item : timingsView->selectedItems())
    {
        timings.remove(item->data(Qt::UserRole).toFloat());
    }

    reloadTimingsView();
}

void Timing::onExportButtonClicked()
{
    player->pause();

    QString exportFilePath = QFileDialog::getSaveFileName(this, tr("Export file"), QString(), tr("Csv Files (*.csv)"));
    if (exportFilePath.isEmpty())
    {
        return;
    }

    QStringList result;
    result.append("");
    result.append(player->getSoundName());

    QStringList exportedTimings;
    QList<float> timingsKeys = timings.keys();
    for (float key : timingsKeys)
    {
        exportedTimings.append(QString("(%1, %2)").arg(key).arg(timings[key]));
    }

    result.append(QString("(%1)").arg(exportedTimings.join(",")));
    result.append("1");

    QFile exportFile(exportFilePath);
    if (!exportFile.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, tr("Unable to open file"), exportFile.errorString());
        return;
    }

    QTextStream out(&exportFile);
    out << ",Sound,ActionTimings,ActionLenght\n";
    out << result.join(",");
}

void Timing::reloadTimingsView()
{
    timingsView->clear();
    QList<float> timingsKeys = timings.keys();
    for (float seconds : timingsKeys)
    {
        QString action = timings[seconds];
        QListWidgetItem *item = new QListWidgetItem(QString("%1 - %2").arg(seconds).arg(action), timingsView);
        item->setData(Qt::UserRole, seconds);
    }
}
