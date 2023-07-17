#ifndef APP_H
#define APP_H

#include <QObject>
#include <qqml.h>
#include <qregexp.h>

using namespace std;

class App  {

public :
    static const long BUFFER = 2048;
    static const uint TEMPSMAX = 900;
    static const long MINIMUM = 1024;

    static QString tailleHumaine(const long v);


};

#endif // APP_H
