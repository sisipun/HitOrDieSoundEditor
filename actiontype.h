#ifndef ACTIONTYPE_H
#define ACTIONTYPE_H

#include <QMap>
#include <QString>

enum class ActionType {
    FIRE,
    GRENADE
};

const QMap<ActionType, QString> ACTION_TYPE_TO_STRING({
                                                          std::make_pair(ActionType::FIRE, "FIRE"),
                                                          std::make_pair(ActionType::GRENADE, "GRENADE")
                                                      });
const QMap<QString, ActionType> STRING_TO_ACTION_TYPE({
                                                          std::make_pair("FIRE", ActionType::FIRE),
                                                          std::make_pair("GRENATE", ActionType::GRENADE)
                                                      });

#endif // ACTIONTYPE_H
