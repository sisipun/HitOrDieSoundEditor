#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QMediaPlayer;
class QAudioOutput;
class QSlider;
class QTimeEdit;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class Player : public QWidget
{
    Q_OBJECT
public:
    explicit Player(QWidget *parent = nullptr);

signals:

private slots:
    void onLoadSoundButtonClicked();
    void onPauseButtonClicked();
    void onDurationChanged(qint64 duration);
    void onPositionChanged(qint64 position);
    void onTimelinePressed();
    void onTimelineReleased();
    void onTimelineMoved(int value);
    void onTimelineValueChanged(int value);

private:
    QMediaPlayer *mediaPlayer;
    QAudioOutput *audioOutput;

    QLabel *title;
    QPushButton *pauseButton;
    QSlider *timeline;
    QTimeEdit *timer;
    QPushButton *loadSoundButton;
};

#endif // PLAYER_H
