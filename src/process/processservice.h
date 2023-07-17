#ifndef PROCESSSERVICE_H
#define PROCESSSERVICE_H

#include <QObject>
#include <QQmlContext>
#include <QThread>
#include "processinfo.h"
#include "ps.h"
#include "killer.h"

#define PROCESSUSSERVICE_QML_NAME "ProcessService"
#define PROCESSUSSERVICE_TIMEOUT (quint64)20

class ProcessService : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool running READ getRunning NOTIFY refreshStateChanged )
    Q_PROPERTY(quint64 systemMemory READ getSystemMemory CONSTANT )

    friend class Ps;
    friend class Killer;

public:
    static ProcessService& getInstance();
    static void init(QQmlContext *context, QString name=PROCESSUSSERVICE_QML_NAME);

    /**
      * @brief set the timeout whom cache will be obsolete
      * @param timeout : in second
      **/
    Q_INVOKABLE void setTimeout(quint64 timeout);

    /**
      * @brief load / refresh the list of current provessus
      * list will be available on signal processList
      * @param force, if true refresh the list
      * @return the last checked list or null if none yet
      * it will be available throught processList signal
      */
    Q_INVOKABLE QVariantList loadProcessList(bool force = false);

    /**
      * @brief ask to kill process with pid "pid"
      * @param pid ; pid of process to kill
      */
    Q_INVOKABLE void kill(quint32 pid);

    /**
      * @brief if pid currently being killed
      * @param pid
      * @return true if being killed
      **/
    Q_INVOKABLE bool inPurgatory(qint32 pid);

    /**
      * @brief list of all process waiting killing
      * @return the list of pid
      **/
    Q_INVOKABLE QList<qint32> getPurgatory() const;

    /**
      * @brief if service currently looking for the process list
      * @return true if currently running
      */
    Q_INVOKABLE bool getRunning() const {return running; }

    /**
      * @since 0.16.1
      * @brief return total system memory as octet
      * @return quint64 memry in octet
      */
    Q_INVOKABLE quint64 getSystemMemory() const { return systemMemory; }

private:
    explicit ProcessService(QObject *parent = 0);

    quint64 timeout;
    quint64 lastChecked;
    QList<ProcessInfo *>cache;

    quint64 systemMemory; // (octet)

    bool running;

    QThread scannerThread;
    Ps ps;

    // process currently destroyed
    QMap<qint32, QThread *> purgatory;

signals:
    void runningChanged();

    /**
     * @brief emit when starting / ending refreshing the process list
     */
    void refreshStateChanged();

    /**
     * @brief list of current process
     * @param list
     */
    void processListAvailable(QVariantList list);

    /**
     * @brief emited when a process was successfully killed
     * @param pid
     * @param name human readable name of process
     */
    void processKilled(qint32 pid, QString const &name);

    /**
     * @brief emited when failling to kill a process
     * @param pid
     * @param name human readable name of process
     */
    void processNOTKilled(qint32 pid, QString const &name);

    /**
     * @brief internal use
     */
    void getlistImp();

    /**
     * @brief internal use
     * @param pid
     * @param name human readable name of process
     */
    void doKill(qint32 pid, QString const &name);

protected slots:
    /**
     * @brief process list available
     * @param list
     */
    void listAvailableImpl(QList<ProcessInfo *> *list);

    /**
     * @brief finnished "killing" a process
     * @param pid
     * @param name human readable name of process
     * @param status
     */
    void endKill(qint32 pid, QString const &name, int status);
};

#endif // PROCESSSERVICE_H
