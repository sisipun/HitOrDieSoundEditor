#include "timing.h"

#include "actiontype.h"
#include "player.h"
#include "timingcsvparser.h"

#include <QtWidgets>

Timing::Timing(Player* player, QWidget* parent)
    : QWidget { parent }
{
    this->player = player;
    connect(this->player, &Player::loaded, this, &Timing::onPlayerLoaded);

    this->parser = new TimingCsvParser();

    timingsView = new QListWidget(this);

    actionLength = new QDoubleSpinBox(this);
    actionLength->setRange(0.01, 9999.99);
    actionLength->setValue(1.0);
    actionLength->setDisabled(true);

    actions = new QComboBox(this);
    actions->addItems(QStringList(STRING_TO_ACTION_TYPE.keys()));
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
    connect(
        exportButton, &QPushButton::clicked, this, &Timing::onExportButtonClicked);

    importButton = new QPushButton(this);
    importButton->setText(tr("Import"));
    connect(importButton, &QPushButton::clicked, this, &Timing::onImportButtonClicked);

    QBoxLayout* layout = new QVBoxLayout(this);

    QBoxLayout* listLayout = new QHBoxLayout;
    listLayout->addWidget(timingsView);

    QBoxLayout* timingsActionsLayout = new QHBoxLayout;
    timingsActionsLayout->addWidget(actionLength, 1);
    timingsActionsLayout->addWidget(actions, 1);
    timingsActionsLayout->addWidget(addButton, 1);
    timingsActionsLayout->addWidget(removeButton, 1);

    QBoxLayout* parsingActionsLayout = new QHBoxLayout;
    parsingActionsLayout->addWidget(exportButton, 1);
    parsingActionsLayout->addWidget(importButton, 1);

    layout->addLayout(listLayout);
    layout->addLayout(timingsActionsLayout);
    layout->addLayout(parsingActionsLayout);

    setLayout(layout);
}

Timing::~Timing()
{
    delete parser;
}

void Timing::onPlayerLoaded(bool loaded)
{
    actionLength->setDisabled(!loaded);
    actions->setDisabled(!loaded);
    addButton->setDisabled(!loaded);
    removeButton->setDisabled(!loaded);
    exportButton->setDisabled(!loaded);
    timings.clear();
    actionLength->setValue(1.0);
    reloadTimingsView();
}

void Timing::onAddButtonClicked()
{
    float seconds = player->getSeconds();
    QString action = actions->currentText();
    timings[seconds] = STRING_TO_ACTION_TYPE[action];
    reloadTimingsView();
}

void Timing::onRemoveButtonClicked()
{
    for (QListWidgetItem* item : timingsView->selectedItems()) {
        timings.remove(item->data(Qt::UserRole).toFloat());
    }

    reloadTimingsView();
}

void Timing::onExportButtonClicked()
{
    player->pause();

    QString exportFilePath = QFileDialog::getSaveFileName(
        this, tr("Export file"), QString(), tr("Csv Files (*.csv)"));
    if (exportFilePath.isEmpty()) {
        return;
    }

    QFile exportFile(exportFilePath);
    if (!exportFile.open(QIODevice::WriteOnly)) {
        QMessageBox::information(
            this, tr("Unable to open file"), exportFile.errorString());
        return;
    }

    QTextStream out(&exportFile);
    out << ",Sound,ActionTimings,ActionLenght\n";
    out << parser->write(
        { player->getSoundName(), timings, float(actionLength->value()) });
}

void Timing::onImportButtonClicked()
{
    player->pause();

    QString importFilePath = QFileDialog::getOpenFileName(
        this, tr("Import file"), QString(), tr("Csv Files (*.csv)"));
    if (importFilePath.isEmpty()) {
        return;
    }

    QFile importFile(importFilePath);
    if (!importFile.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), importFile.errorString());
        return;
    }

    QTextStream in(&importFile);
    in.readLine();
    TimingData data = parser->read(in.readLine());

    if (data.soundFilePath.isEmpty()) {
        QMessageBox::information(this, tr("Illegal format"), tr("File has illegal format"));
        return;
    }

    player->load(data.soundFilePath);
    timings = data.timings;
    actionLength->setValue(data.actionLength);
    reloadTimingsView();
    player->play();
}

void Timing::reloadTimingsView()
{
    timingsView->clear();
    QList<float> timingsKeys = timings.keys();
    for (float seconds : timingsKeys) {
        ActionType action = timings[seconds];
        QListWidgetItem* item = new QListWidgetItem(QString("%1 - %2").arg(seconds).arg(ACTION_TYPE_TO_STRING[action]), timingsView);
        item->setData(Qt::UserRole, seconds);
    }
}
