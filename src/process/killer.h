#ifndef KILLER_H
#define KILLER_H

#include <QObject>
#include <qqml.h>

/**
 * @brief kill a process by pid ; user must have the privilege
 * to be used inside a Thread
 */
class Killer :  public QObject {
    Q_OBJECT

public :
    explicit Killer(QObject *parent = 0);

public slots:
    /**
     * @brief kill the process pid
     * @param name human readable name of process
     * @param pid
     */
    void doKill(qint32 pid, QString const &name);

signals:
    /**
     * @brief process stopped, or not, see status
     * @param pid
     * @param name human readable name of process
     * @param status
     */
    void done(qint32 pid, QString const &name, int status);
};

#endif // KILLER_H
