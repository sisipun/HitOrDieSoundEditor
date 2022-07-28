#include "player.h"

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QPushButton>
#include <QtWidgets>

Player::Player(QWidget *parent)
    : QWidget{parent}
{
    mediaPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    mediaPlayer->setAudioOutput(audioOutput);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &Player::onDurationChanged);
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &Player::onPositionChanged);

    title = new QLabel(this);
    title->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    timeline = new QSlider(Qt::Horizontal, this);
    timeline->setDisabled(true);
    timer = new QTimeEdit(this);
    timer->setDisplayFormat("HH:mm:ss");
    timer->setTime(QTime(0, 0));
    timer->setDisabled(true);
    loadSoundButton = new QPushButton(this);
    loadSoundButton->setText(tr("Load sound"));
    connect(loadSoundButton, &QPushButton::clicked, this, &Player::onLoadSoundButtonClicked);
    connect(timeline, &QSlider::sliderPressed, this, &Player::onTimelinePressed);
    connect(timeline, &QSlider::sliderReleased, this, &Player::onTimelineReleased);
    connect(timeline, &QSlider::sliderMoved, this, &Player::onTimelineMoved);
    connect(timeline, &QSlider::valueChanged, this, &Player::onTimelineValueChanged);

    QBoxLayout *layout = new QVBoxLayout(this);
    QBoxLayout *titleLayout = new QHBoxLayout(this);
    titleLayout->addWidget(title);
    QBoxLayout *playerLayout = new QHBoxLayout(this);
    playerLayout->addWidget(timeline);
    playerLayout->addWidget(timer);
    QBoxLayout *actionsLayout = new QHBoxLayout(this);
    actionsLayout->addWidget(loadSoundButton);

    layout->addLayout(titleLayout);
    layout->addLayout(playerLayout);
    layout->addLayout(actionsLayout);

    setLayout(layout);
}

void Player::onLoadSoundButtonClicked()
{
    QString soundFileName = QFileDialog::getOpenFileName(this, tr("Open sound"), QString(), tr("Sound Files (*.wav)"));
    if (!soundFileName.isEmpty())
    {
        title->setText(soundFileName);
        timeline->setDisabled(false);
        mediaPlayer->setSource(QUrl::fromLocalFile(soundFileName));
        mediaPlayer->play();
    }
}

void Player::onDurationChanged(qint64 duration)
{
    timeline->setMaximum(duration);
}

void Player::onPositionChanged(qint64 position)
{
    timeline->setValue(position);
}

void Player::onTimelinePressed()
{
    mediaPlayer->pause();
}

void Player::onTimelineReleased()
{
    if (timeline->maximum() != timeline->value())
    {
        mediaPlayer->play();
    }
    else
    {
        mediaPlayer->stop();
        timeline->setValue(0);
    }
}

void Player::onTimelineMoved(int value)
{
    mediaPlayer->setPosition(value);
}

void Player::onTimelineValueChanged(int value)
{
    timer->setTime(QTime::fromMSecsSinceStartOfDay(value));

    if (timeline->maximum() == value && !timeline->isSliderDown())
    {
        mediaPlayer->stop();
        timeline->setValue(0);
    }
}
