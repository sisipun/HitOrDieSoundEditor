#include "timingcsvparser.h"

#include <QObject>

TimingCsvParser::TimingCsvParser() { }

QString
TimingCsvParser::write(const TimingData& data)
{
    QStringList result;
    result.append("\"\"");
    result.append(QString("\"%1\"").arg(data.soundFilePath));

    QStringList timings;
    QList<float> timingsKeys = data.timings.keys();
    for (float key : timingsKeys) {
        timings.append(QString("(%1,%2)").arg(key).arg(data.timings[key]));
    }

    result.append(QString("\"(%1)\"").arg(timings.join(",")));
    result.append(QString("\"%1\"").arg(data.actionLength));

    return result.join(",");
}

TimingData
TimingCsvParser::read(QString line)
{
    TimingData data;
    QStringList result = line.mid(1, line.length() - 2).split("\",\"");
    if (result.size() != 4) {
        return data;
    }

    data.soundFilePath = result[1];

    QStringList importedTimings = result[2].mid(2, result[2].length() - 4).split("),(");
    for (const QString& importedTiming : importedTimings) {
        QStringList splittedTiming = importedTiming.split(",");
        data.timings[splittedTiming[0].toFloat()] = splittedTiming[1];
    }

    data.actionLength = result[3].toFloat();

    return data;
}
