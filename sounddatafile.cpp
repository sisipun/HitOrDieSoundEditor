#include "sounddatafile.h"

#include "sounddataparser.h"
#include "soundmodel.h"

#include <QFile>
#include <QTextStream>

#include <QDebug>

SoundDataFile::SoundDataFile(QString filePath)
    : filePath(filePath)
{
}

void SoundDataFile::write(QList<SoundModel> sounds)
{
    QFile exportFile(filePath);
    if (!exportFile.open(QIODevice::WriteOnly)) {
        return;
    }

    QTextStream out(&exportFile);

    // Write header
    out << ",Sound,ActionTimings\n";

    SoundDataParser parser;
    for (const SoundModel& sound : sounds) {
        out << parser.write(sound);
    }
}

QList<SoundModel> SoundDataFile::read()
{
    QFile importFile(filePath);
    if (!importFile.open(QIODevice::ReadOnly)) {
        return {};
    }

    QTextStream in(&importFile);
    if (in.atEnd()) {
        return {};
    }

    // Skip first line
    in.readLine();

    QList<SoundModel> result;
    SoundDataParser parser;
    while (!in.atEnd()) {
        result.append(parser.read(in.readLine()));
    }

    return result;
}
