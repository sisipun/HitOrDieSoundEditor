#ifndef TIMING_H
#define TIMING_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class Player;
class QPushButton;
class QComboBox;
class QListWidget;
QT_END_NAMESPACE

class Timing : public QWidget
{
    Q_OBJECT
public:
    explicit Timing(Player* player, QWidget *parent = nullptr);

    void reloadTimingsView();

private slots:
    void onAddButtonClicked();
    void onRemoveButtonClicked();
    void onExportButtonClicked();
    void onPlayerLoaded(bool loaded);

private:
    Player *player;

    QListWidget *timingsView = nullptr;
    QComboBox *actions = nullptr;
    QPushButton *addButton = nullptr;
    QPushButton *removeButton = nullptr;
    QPushButton *exportButton = nullptr;

    QMap<float, QString> timings;
};

#endif // TIMING_H
