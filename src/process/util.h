#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include "applicationtype.h"

class Util {

public:
    Util();

    /**
     * @brief convenience method to get the name of a file/ app from a full path
     * added here to prevent a new dependecy to expternal project
     * @param v
     * @return
     */
    static QString getName(const QString &v);

    /**
     * @brief open a file into a string from a full path
     * @param path
     * @return
     */
//    static QString fileToString(const QString &path, const char *charset = "ISO 8859-1");
    static QString fileToString(const QString &path);

    /**
     * @brief try to fill info with the real process from command line arguments and type of app
     * @param commandLine
     * @param ApplicationType
     * @return true if found a match
     */
    static bool processName(const QString &, ApplicationType &info);


};

#endif // UTIL_H
