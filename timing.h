#ifndef TIMING_H
#define TIMING_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class Player;
class SoundDataParser;
enum class ActionType;
class QPushButton;
class QComboBox;
class QListWidget;
class QListWidgetItem;
class QDoubleSpinBox;
QT_END_NAMESPACE

class Timing : public QWidget {
    Q_OBJECT
public:
    explicit Timing(Player* player, QWidget* parent = nullptr);
    ~Timing();

    void reloadTimingsView();

private slots:
    void onPlayerLoaded(bool loaded);
    void onAddButtonClicked();
    void onRemoveButtonClicked();
    void onTmingsDoubleClocked(QListWidgetItem* item);
    void onExportButtonClicked();
    void onImportButtonClicked();

private:
    Player* player;
    SoundDataParser* parser;

    QListWidget* timingsView = nullptr;
    QDoubleSpinBox* actionLength = nullptr;
    QComboBox* actions = nullptr;
    QPushButton* addButton = nullptr;
    QPushButton* removeButton = nullptr;
    QPushButton* exportButton = nullptr;
    QPushButton* importButton = nullptr;

    QMap<float, ActionType> timings;
};

#endif // TIMING_H
