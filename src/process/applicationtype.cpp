#include "applicationtype.h"

ApplicationType::ApplicationType(QObject *parent) : QObject(parent), type(SYSTEM) {

}

/**
 * @brief plain name of application
 * @return
 */
 QString ApplicationType::getName() const {return name;}
void ApplicationType::setName(const QString &v) {name=v;}

/**
 * @brief type of application
 * @return
 */
ApplicationType::Type ApplicationType::getType() const {return type;}
void ApplicationType::setType(Type v) {type=v;}
