#include "player.h"

#include <QAudioOutput>
#include <QMediaPlayer>
#include <QtWidgets>

Player::Player(QWidget* parent)
    : QWidget { parent }
{
    mediaPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    mediaPlayer->setAudioOutput(audioOutput);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &Player::onDurationChanged);
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &Player::onPositionChanged);

    title = new QLabel(this);
    title->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    pauseButton = new QPushButton();
    pauseButton->setText(tr(">"));
    pauseButton->setDisabled(true);
    connect(pauseButton, &QPushButton::clicked, this, &Player::onPauseButtonClicked);

    timeline = new QSlider(Qt::Horizontal, this);
    timeline->setDisabled(true);
    connect(timeline, &QSlider::sliderPressed, this, &Player::onTimelinePressed);
    connect(timeline, &QSlider::sliderReleased, this, &Player::onTimelineReleased);
    connect(timeline, &QSlider::sliderMoved, this, &Player::onTimelineMoved);
    connect(timeline, &QSlider::valueChanged, this, &Player::onTimelineValueChanged);

    timer = new QTimeEdit(this);
    timer->setDisplayFormat("HH:mm:ss");
    timer->setTime(QTime(0, 0));
    timer->setDisabled(true);
    timer->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);

    loadSoundButton = new QPushButton(this);
    loadSoundButton->setText(tr("Load sound"));
    connect(loadSoundButton, &QPushButton::clicked, this, &Player::onLoadSoundButtonClicked);

    QBoxLayout* layout = new QVBoxLayout(this);

    QBoxLayout* titleLayout = new QHBoxLayout;
    titleLayout->addWidget(title);

    QBoxLayout* playerLayout = new QHBoxLayout;
    playerLayout->addWidget(pauseButton);
    playerLayout->addWidget(timeline);
    playerLayout->addWidget(timer);

    QBoxLayout* actionsLayout = new QHBoxLayout;
    actionsLayout->addWidget(loadSoundButton);

    layout->addLayout(titleLayout);
    layout->addLayout(playerLayout);
    layout->addLayout(actionsLayout);

    setLayout(layout);
}

void Player::load(const QString& soundFilePath)
{
    title->setText(soundFilePath);
    timeline->setDisabled(false);
    pauseButton->setDisabled(false);
    mediaPlayer->setSource(QUrl::fromLocalFile(soundFilePath));
    emit loaded(true);
}

void Player::play()
{
    mediaPlayer->play();
    pauseButton->setText(tr("||"));
}

void Player::pause()
{
    mediaPlayer->pause();
    pauseButton->setText(tr(">"));
}

void Player::stop()
{
    mediaPlayer->stop();
    timeline->setValue(0);
    pauseButton->setText(tr(">"));
}

void Player::setPosition(float seconds)
{
    mediaPlayer->setPosition(seconds * 1000);
}

bool Player::isLoaded() const
{
    return timeline->isEnabled();
}

float Player::getSeconds() const
{
    return timeline->value() / 1000.0;
}

QString Player::getSoundName() const
{
    return title->text();
}

void Player::onLoadSoundButtonClicked()
{
    pause();
    QString soundFilePath = QFileDialog::getOpenFileName(
        this, tr("Open sound"), QString(), tr("Sound Files (*.wav *.mp3 *.ogg)"));
    if (soundFilePath.isEmpty()) {
        return;
    }

    load(soundFilePath);
    play();
}

void Player::onPauseButtonClicked()
{
    if (mediaPlayer->playbackState() == QMediaPlayer::PlaybackState::PlayingState) {
        pause();
    } else {
        play();
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
    if (timeline->maximum() != timeline->value()) {
        play();
    } else {
        stop();
    }
}

void Player::onTimelineMoved(int value)
{
    mediaPlayer->setPosition(value);
}

void Player::onTimelineValueChanged(int value)
{
    timer->setTime(QTime::fromMSecsSinceStartOfDay(value));

    if (timeline->maximum() == value && !timeline->isSliderDown()) {
        stop();
    }
}
