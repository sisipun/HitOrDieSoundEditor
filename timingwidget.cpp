#include "timingwidget.h"

#include "actiontype.h"
#include "playerwidget.h"
#include "sounddataparser.h"
#include "soundmodel.h"
#include "timingmodel.h"
#include "timingview.h"

#include <QtWidgets>

TimingWidget::TimingWidget(PlayerWidget* player, QWidget* parent)
    : QWidget { parent }
{
    this->player = player;
    connect(this->player, &PlayerWidget::loaded, this, &TimingWidget::onPlayerLoaded);
    connect(this->player, &PlayerWidget::positionChanged, this, &TimingWidget::onPositionChanged);

    this->parser = new SoundDataParser();

    timingsView = new TimingView(this);
    timingsView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(timingsView, &TimingView::itemDoubleClicked, this, &TimingWidget::onTmingsDoubleClocked);

    actionLengthInput = new QDoubleSpinBox(this);
    actionLengthInput->setRange(0.01, 9999.99);
    actionLengthInput->setValue(1.0);
    actionLengthInput->setDisabled(true);

    actionSelect = new QComboBox(this);
    actionSelect->addItems(QStringList(STRING_TO_ACTION_TYPE.keys()));
    actionSelect->setCurrentIndex(0);
    actionSelect->setDisabled(true);

    addButton = new QPushButton(this);
    addButton->setText(tr("+"));
    addButton->setDisabled(true);
    connect(addButton, &QPushButton::clicked, this, &TimingWidget::onAddButtonClicked);

    removeButton = new QPushButton(this);
    removeButton->setText(tr("-"));
    removeButton->setDisabled(true);
    connect(removeButton, &QPushButton::clicked, this, &TimingWidget::onRemoveButtonClicked);

    exportButton = new QPushButton(this);
    exportButton->setText(tr("Export"));
    exportButton->setDisabled(true);
    connect(
        exportButton, &QPushButton::clicked, this, &TimingWidget::onExportButtonClicked);

    importButton = new QPushButton(this);
    importButton->setText(tr("Import"));
    connect(importButton, &QPushButton::clicked, this, &TimingWidget::onImportButtonClicked);

    QBoxLayout* layout = new QVBoxLayout(this);

    QBoxLayout* listLayout = new QHBoxLayout;
    listLayout->addWidget(timingsView);

    QBoxLayout* timingsActionsLayout = new QHBoxLayout;
    timingsActionsLayout->addWidget(actionLengthInput, 1);
    timingsActionsLayout->addWidget(actionSelect, 1);
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

TimingWidget::~TimingWidget()
{
    delete parser;
}

void TimingWidget::onPlayerLoaded(bool loaded)
{
    actionLengthInput->setDisabled(!loaded);
    actionSelect->setDisabled(!loaded);
    addButton->setDisabled(!loaded);
    removeButton->setDisabled(!loaded);
    exportButton->setDisabled(!loaded);
    timings.clear();
    actionLengthInput->setValue(1.0);
    reloadTimingsView();
}

void TimingWidget::onPositionChanged(float position)
{
    int currentRow = 0;
    QList<float> timingsKeys = timings.keys();
    for (float key : timingsKeys) {
        const TimingModel& timing = timings[key];
        if (position > timing.startSecond && position < timing.endSecond) {
            timingsView->setCurrentRow(currentRow);
            timingsView->scrollToItem(timingsView->currentItem(), QAbstractItemView::ScrollHint::PositionAtCenter);
            break;
        }

        currentRow++;
    }
}

void TimingWidget::onAddButtonClicked()
{
    float startSecond = player->getPosition();
    float actionLength = actionLengthInput->value();
    QString action = actionSelect->currentText();
    timings[startSecond] = { startSecond, startSecond + actionLength, STRING_TO_ACTION_TYPE[action] };
    reloadTimingsView();
}

void TimingWidget::onUpdateButtonClicked()
{
    for (QListWidgetItem* item : timingsView->selectedItems()) {
        TimingModel& timing = timings[item->data(Qt::UserRole).toFloat()];
        float actionLength = actionLengthInput->value();
        QString action = actionSelect->currentText();
        timing.endSecond = timing.startSecond + actionLength;
        timing.action = STRING_TO_ACTION_TYPE[action];
    }

    reloadTimingsView();
}

void TimingWidget::onRemoveButtonClicked()
{
    for (QListWidgetItem* item : timingsView->selectedItems()) {
        timings.remove(item->data(Qt::UserRole).toFloat());
    }

    reloadTimingsView();
}

void TimingWidget::onTmingsDoubleClocked(QListWidgetItem* item)
{
    player->setPosition(item->data(Qt::UserRole).toFloat());
}

void TimingWidget::onExportButtonClicked()
{
    player->pause();

    QString exportFilePath = QFileDialog::getSaveFileName(
        this, tr("Export file"), QString(), tr("Csv Files (*.csv)"));
    if (exportFilePath.isEmpty()) {
        return;
    }

    QFile exportFile(exportFilePath);
    if (!exportFile.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), exportFile.errorString());
        return;
    }

    QTextStream out(&exportFile);

    out << ",Sound,ActionTimings,ActionLenght\n";
    out << parser->write({ player->getSoundName(), timings.values() });
}

void TimingWidget::onImportButtonClicked()
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
    const SoundModel& data = parser->read(in.readLine());

    if (data.soundFilePath.isEmpty()) {
        QMessageBox::information(this, tr("Illegal format"), tr("File has illegal format"));
        return;
    }

    player->load(data.soundFilePath);
    for (const TimingModel& timing : data.timings) {
        timings[timing.startSecond] = timing;
    }

    reloadTimingsView();
    player->play();
}

void TimingWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        onUpdateButtonClicked();
    } else if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete) {
        onRemoveButtonClicked();
    } else {
        QWidget::keyPressEvent(event);
    }
}

void TimingWidget::reloadTimingsView()
{
    timingsView->clear();
    QList<float> timingsKeys = timings.keys();
    float lastEndSecond = 0.0;
    for (float seconds : timingsKeys) {
        const TimingModel& timing = timings[seconds];
        float distnaceFromPrevious = timing.startSecond - lastEndSecond;
        QString title = QString("Action: %1, Timing: [%2 - %3], Distance from previous: %4")
                            .arg(ACTION_TYPE_TO_STRING[timing.action])
                            .arg(timing.startSecond)
                            .arg(timing.endSecond)
                            .arg(distnaceFromPrevious);
        QListWidgetItem* item = new QListWidgetItem(title, timingsView);
        if (distnaceFromPrevious < 0.0) {
            item->setBackground(Qt::red);
        } else if (distnaceFromPrevious < 0.3) {
            item->setBackground(Qt::darkYellow);
        }

        item->setData(Qt::UserRole, seconds);
        lastEndSecond = timing.endSecond;
    }
}
