#include "killer.h"
#include <QDebug>
#include <stdio.h>
#include "signal.h"

/**
 * @brief killer::killer
 */
Killer::Killer(QObject *parent) : QObject(parent){
}

/**
  * @brief killer::start
  * @param pid
  */
void Killer::doKill(qint32 pid, QString const &name) {

 //   int status=kill(pid, SIGQUIT);
    int status=kill(pid, SIGINT);
    if(status != 0) {
        status=kill(pid, SIGKILL);
    }

    emit done(pid, name, status);
}
