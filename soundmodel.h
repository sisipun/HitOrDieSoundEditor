#ifndef SOUNDMODEL_H
#define SOUNDMODEL_H

#include <QList>
#include <QString>

QT_BEGIN_NAMESPACE
class TimingModel;
QT_END_NAMESPACE

struct SoundModel {
    QString soundFilePath;
    QList<TimingModel> timings;
};

#endif // SOUNDMODEL_H
