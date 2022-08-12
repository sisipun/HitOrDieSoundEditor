#ifndef SOUNDDATAFILE_H
#define SOUNDDATAFILE_H

#include <QList>
#include <QString>

QT_BEGIN_NAMESPACE
class SoundModel;
QT_END_NAMESPACE

class SoundDataFile {
public:
    SoundDataFile(QString filePath);

    void write(QList<SoundModel> sounds);
    QList<SoundModel> read();

private:
    QString filePath;
};

#endif // SOUNDDATAFILE_H
