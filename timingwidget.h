#ifndef TIMINGWIDGET_H
#define TIMINGWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class PlayerWidget;
class SoundDataParser;
class TimingModel;
enum class ActionType;
class QPushButton;
class QComboBox;
class QListWidget;
class QListWidgetItem;
class QDoubleSpinBox;
QT_END_NAMESPACE

class TimingWidget : public QWidget {
    Q_OBJECT
public:
    explicit TimingWidget(PlayerWidget* player, QWidget* parent = nullptr);
    ~TimingWidget();

    void reloadTimingsView();

private slots:
    void onPlayerLoaded(bool loaded);
    void onAddButtonClicked();
    void onUpdateButtonClicked();
    void onRemoveButtonClicked();
    void onTmingsDoubleClocked(QListWidgetItem* item);
    void onExportButtonClicked();
    void onImportButtonClicked();
    void keyPressEvent(QKeyEvent* event) override;

private:
    PlayerWidget* player;
    SoundDataParser* parser;

    QListWidget* timingsView = nullptr;
    QDoubleSpinBox* actionLengthInput = nullptr;
    QComboBox* actionSelect = nullptr;
    QPushButton* addButton = nullptr;
    QPushButton* removeButton = nullptr;
    QPushButton* exportButton = nullptr;
    QPushButton* importButton = nullptr;

    QMap<float, TimingModel> timings;
};

#endif // TIMINGWIDGET_H
