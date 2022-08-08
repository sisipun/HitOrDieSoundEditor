#include "sounddataparser.h"

#include "actiontype.h"

#include <QObject>

SoundDataParser::SoundDataParser() { }

QString SoundDataParser::write(const SoundData& data)
{
    QStringList result;
    result.append("\"\"");
    result.append(QString("\"%1\"").arg(data.soundFilePath));

    QStringList timings;
    for (const TimingData& timing : data.timings) {
        timings.append(QString("(StartSecond=%1").arg(timing.startSecond));
        timings.append(QString("EndSecond=%1").arg(timing.endSecond));
        timings.append(QString("Action=%1)").arg(ACTION_TYPE_TO_STRING[timing.action]));
    }

    result.append(QString("\"(%1)\"").arg(timings.join(",")));

    return result.join(",");
}

SoundData SoundDataParser::read(QString line)
{
    SoundData data;
    QStringList result = line.mid(1, line.length() - 2).split("\",\"");
    if (result.size() != 3) {
        return data;
    }

    data.soundFilePath = result[1];

    QStringList importedTimings = result[2].mid(2, result[2].length() - 4).split("),(");
    for (const QString& importedTiming : importedTimings) {
        QStringList timingParams = importedTiming.split(",");
        TimingData timing;
        for (const QString& timingParam : timingParams) {
            QStringList splittedTimingParam = timingParam.split("=");
            QString key = splittedTimingParam[0];
            QString value = splittedTimingParam[1];
            if (key == "StartSecond") {
                timing.startSecond = value.toFloat();
            } else if (key == "EndSecond") {
                timing.endSecond = value.toFloat();
            } else if (key == "Action") {
                timing.action = STRING_TO_ACTION_TYPE[value];
            }
        }
        data.timings.append(timing);
    }

    return data;
}
