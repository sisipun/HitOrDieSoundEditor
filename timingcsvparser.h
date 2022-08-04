#ifndef TIMINGCSVPARSER_H
#define TIMINGCSVPARSER_H

#include <QMap>
#include <QString>

QT_BEGIN_NAMESPACE
enum class ActionType;
QT_END_NAMESPACE

struct TimingData {
    QString soundFilePath;
    QMap<float, ActionType> timings;
    float actionLength;
};

class TimingCsvParser {
public:
    TimingCsvParser();

    QString write(const TimingData& data);
    TimingData read(QString line);
};

#endif // TIMINGCSVPARSER_H
