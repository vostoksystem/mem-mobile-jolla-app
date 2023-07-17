#ifndef PS_H
#define PS_H

#include <QObject>
#include <qqml.h>
#include "processinfo.h"

/**
 * @brief return the list of current process
 * @ relies on desktop service
 * this class has to be put into a thread
 */
class Ps : public QObject {
    Q_OBJECT

public :
    explicit Ps(QObject *parent = 0);

private :  
    qint64 hertz;

    qint16 pageSize; // (en octet)

public slots:
    void start();

signals:
    void listAvailable(QList<ProcessInfo *> *list);
};


#endif // PS_H
