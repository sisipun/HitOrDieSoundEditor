#ifndef TIMINGMODEL_H
#define TIMINGMODEL_H

#include "actiontype.h"

#include <Qt>

struct TimingModel {
    float startSecond;
    float endSecond;
    ActionType action;
};

#endif // TIMINGMODEL_H
