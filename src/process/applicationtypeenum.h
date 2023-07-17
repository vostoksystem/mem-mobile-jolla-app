#ifndef APPLICATIONTYPEENUM_H
#define APPLICATIONTYPEENUM_H

#include <QObject>

using namespace std;

class ApplicationTypeEnum : public QObject
{
    Q_OBJECT
    Q_ENUMS(Type)
public:
    enum Type {
        SYSTEM=0, SAILFISH, ANDROID
    };

private:
   ApplicationTypeEnum (){}
};

#endif //
