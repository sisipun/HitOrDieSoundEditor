#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class SoundEditor;
}
QT_END_NAMESPACE

class SoundEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit SoundEditor(QWidget *parent = nullptr);
    ~SoundEditor();

private:
    Ui::SoundEditor *ui;
};