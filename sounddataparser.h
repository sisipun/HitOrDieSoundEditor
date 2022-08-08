#ifndef SOUNDDATAPARSER_H
#define SOUNDDATAPARSER_H

#include <QList>
#include <QString>

QT_BEGIN_NAMESPACE
enum class ActionType;
QT_END_NAMESPACE

struct TimingData {
    float startSecond;
    float endSecond;
    ActionType action;
};

struct SoundData {
    QString soundFilePath;
    QList<TimingData> timings;
};

class SoundDataParser {
public:
    SoundDataParser();

    QString write(const SoundData& data);
    SoundData read(QString line);
};

#endif // SOUNDDATAPARSER_H
