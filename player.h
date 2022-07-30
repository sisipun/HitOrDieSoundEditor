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

    void play();
    void pause();
    void stop();
    bool isLoaded() const;
    float getSeconds() const;

signals:
    void loaded(bool loaded);

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
    QMediaPlayer *mediaPlayer = nullptr;
    QAudioOutput *audioOutput = nullptr;

    QLabel *title = nullptr;
    QPushButton *pauseButton = nullptr;
    QSlider *timeline = nullptr;
    QTimeEdit *timer = nullptr;
    QPushButton *loadSoundButton = nullptr;
};

#endif // PLAYER_H
