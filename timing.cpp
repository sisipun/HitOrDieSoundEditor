#include "timing.h"

#include "player.h"

#include <QtDebug>
#include <QtWidgets>

Timing::Timing(Player* player, QWidget* parent)
    : QWidget { parent }
{
    this->player = player;
    connect(this->player, &Player::loaded, this, &Timing::onPlayerLoaded);

    timingsView = new QListWidget(this);
    timingsView->setSortingEnabled(true);

    actionLength = new QDoubleSpinBox(this);
    actionLength->setRange(0.01, 9999.99);
    actionLength->setValue(1.0);
    actionLength->setDisabled(true);

    actions = new QComboBox(this);
    actions->addItems(QStringList({ "FIRE", "RELOAD" }));
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

void Timing::onPlayerLoaded(bool loaded)
{
    actionLength->setDisabled(!loaded);
    actions->setDisabled(!loaded);
    addButton->setDisabled(!loaded);
    removeButton->setDisabled(!loaded);
    exportButton->setDisabled(!loaded);
    timings.clear();
    reloadTimingsView();
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
    for (QListWidgetItem* item : timingsView->selectedItems()) {
        timings.remove(item->data(Qt::UserRole).toFloat());
    }

    reloadTimingsView();
}

void Timing::onExportButtonClicked()
{
    player->pause();

    QString exportFilePath = QFileDialog::getSaveFileName(this, tr("Export file"), QString(), tr("Csv Files (*.csv)"));
    if (exportFilePath.isEmpty()) {
        return;
    }

    QStringList result;
    result.append("\"\"");
    result.append(QString("\"%1\"").arg(player->getSoundName()));

    QStringList exportedTimings;
    QList<float> timingsKeys = timings.keys();
    for (float key : timingsKeys) {
        exportedTimings.append(QString("(%1, %2)").arg(key).arg(timings[key]));
    }

    result.append(QString("\"(%1)\"").arg(exportedTimings.join(",")));
    result.append(QString("\"%1\"").arg(actionLength->value()));

    QFile exportFile(exportFilePath);
    if (!exportFile.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), exportFile.errorString());
        return;
    }

    QTextStream out(&exportFile);
    out << ",Sound,ActionTimings,ActionLenght\n";
    out << result.join(",");
}

void Timing::onImportButtonClicked()
{
    player->pause();

    QString importFilePath = QFileDialog::getOpenFileName(this, tr("Import file"), QString(), tr("Csv Files (*.csv)"));
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
    QString text = in.readLine();
    QStringList result = text.mid(1, text.length() - 2).split("\",\"");
    if (result.size() != 4) {
        QMessageBox::information(this, tr("Illegal format"), tr("File has illegal format"));
        return;
    }

    QString soundFilePath = result[1].trimmed();
    player->load(soundFilePath);

    QStringList importedTimings = result[2].mid(2, result[2].length() - 4).split("),(");
    for (const QString& importedTiming : importedTimings) {
        QStringList splittedTiming = importedTiming.split(",");
        timings[splittedTiming[0].trimmed().toFloat()] = splittedTiming[1].trimmed();
    }
    actionLength->setValue(result[3].trimmed().toDouble());

    reloadTimingsView();
    player->play();
}

void Timing::reloadTimingsView()
{
    timingsView->clear();
    QList<float> timingsKeys = timings.keys();
    for (float seconds : timingsKeys) {
        QString action = timings[seconds];
        QListWidgetItem* item = new QListWidgetItem(QString("%1 - %2").arg(seconds).arg(action), timingsView);
        item->setData(Qt::UserRole, seconds);
    }
}
