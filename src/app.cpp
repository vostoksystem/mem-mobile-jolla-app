#include "app.h"
#include <QFile>
#include <QDir>
#include <QMap>

#include<QTextStream>

static QRegExp lignenom("^Name=(.*)");


/**
 * @param v
 * @return
 */
QString App::tailleHumaine(const long v) {
    if(v>512000) {
        return QString::number( ((float)v/1048576.0), 'f',2 ) + "Gb";
    }
    return QString::number(((float)v/1024.0), 'f',2  ) + "Mb";
}

