#ifndef TIMINGCSVPARSER_H
#define TIMINGCSVPARSER_H

#include <QMap>
#include <QString>

struct TimingData {
    QString soundFilePath;
    QMap<float, QString> timings;
    float actionLength;
};

class TimingCsvParser {
public:
    TimingCsvParser();

    QString write(const TimingData& data);
    TimingData read(QString line);
};

#endif // TIMINGCSVPARSER_H
