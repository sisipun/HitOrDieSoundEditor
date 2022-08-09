#ifndef TIMINGMODEL_H
#define TIMINGMODEL_H

#include <Qt>

QT_BEGIN_NAMESPACE
enum class ActionType;
QT_END_NAMESPACE

struct TimingModel {
    float startSecond;
    float endSecond;
    ActionType action;
};

#endif // TIMINGMODEL_H
