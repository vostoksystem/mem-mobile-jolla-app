#include "processservice.h"
#include <QtQml>
#include <QMetaType>
#include <QQmlEngine>
#include <QDateTime>
#include <QRegularExpression>
#include "util.h"

ProcessService::ProcessService(QObject *parent) : QObject(parent),
    timeout(PROCESSUSSERVICE_TIMEOUT), lastChecked(0), running(false){

    // on calcule la taille totale de memoire
    QStringList data = Util::fileToString("/proc/meminfo").split( "\n" );
    QRegularExpression rx("(\\d+)");
    systemMemory = rx.match(data.at(0)).captured(1).toLong();
    systemMemory *= 1024;
    qDebug() << "totalMem : " << systemMemory ;

    ps.moveToThread(&scannerThread);
    connect(&scannerThread, SIGNAL(finished()), &ps, SLOT(deleteLater()));
    connect(&ps, SIGNAL(listAvailable(QList<ProcessInfo*>*)), this, SLOT(listAvailableImpl(QList<ProcessInfo*>*)), Qt::QueuedConnection);
    connect(this, SIGNAL(getlistImp()), &ps, SLOT(start()) , Qt::QueuedConnection );
    scannerThread.start();
}

/**
 * @brief return singleton instance
 * @return ProcessusService instance
 */
ProcessService& ProcessService::getInstance() {
    static ProcessService instance;
    return instance;
}

/**
 * @brief ProcessusService::init
 * @param context
 */
void ProcessService::init(QQmlContext *context, QString name) {
    context->setContextProperty(name, &ProcessService::getInstance());

    qmlRegisterUncreatableType<ProcessInfo>("ProcessInfo", 1, 0, "ProcessInfo","");
    qRegisterMetaType<ProcessInfoList>( "ProcessInfoList" );
    qmlRegisterUncreatableType<ApplicationType>("ApplicationType", 1, 0, "","ApplicationType");

}
/**
  * @brief set the timeout whom cache will be obsolete
  * @param timeout : in second
  **/
void ProcessService::setTimeout(quint64 timeout) {
    this->timeout=(timeout <1 ? 1 : timeout)*1000;
}

/**
  * @brief load / refresh the list of current provessus
  * list will be available on signal processList
  * @param force, if true refresh the list
  * @return the last checked list or null if none yet
  * it will be available throught processList signal
  */
QVariantList ProcessService::loadProcessList(bool force) {

    QVariantList v;

    if(running) {
        return v;
    }

    if(force
            || (cache.length()==0)
            || ((quint64)(QDateTime::currentDateTime().currentMSecsSinceEpoch()) > (lastChecked + timeout))
            ) {
        running=true;
        emit refreshStateChanged();
        emit getlistImp();
        return v;
    }

    foreach (ProcessInfo *t, cache) {
        v.append(QVariant::fromValue(t));
    }

    return v;
}

/**
  * @brief ask to kill process with pid "pid"
  * @param pid ; pid of process to kill
  */
void ProcessService::kill(quint32 pid) {

    if(purgatory.contains(pid)) {
        return;
    }

    QThread *th=new QThread();
    Killer *t = new Killer();
    t->moveToThread(th);
    connect(th, SIGNAL(finished()), t, SLOT(deleteLater()));
    connect(t, SIGNAL(done(qint32,QString,int)), this, SLOT(endKill(qint32,QString,int)), Qt::QueuedConnection );
    connect(this, SIGNAL(doKill(qint32,QString)), t, SLOT(doKill(qint32,QString)), Qt::QueuedConnection );

    purgatory.insert(pid,th);
    th->start();

    foreach (ProcessInfo *t, cache) {

        if(t->getPid()==pid) {
            emit doKill(pid,t->getName());
            return;
        }
    }

    emit doKill(pid, QString::number(pid));
}

/**
  * @brief if pid currently being killed
  * @param pid
  * @return true if being killed
  **/
bool ProcessService::inPurgatory(qint32 pid) {
    return purgatory.contains(pid);
}

/**
   * @brief list of all process waiting killing
   * @return the list of pid
   **/
QList<qint32> ProcessService::getPurgatory() const {
    return purgatory.keys();
}

/**
  * @brief process list available
  * @param list
  */
void ProcessService::listAvailableImpl(QList<ProcessInfo *> *list) {
    cache.empty();

    QVariantList v;

    QList<ProcessInfo *>::iterator j;
    for (j = list->begin(); j != list->end(); ++j)
    {
        ProcessInfo *i = new ProcessInfo();
        i->setPid((*j)->getPid());
        i->setName((*j)->getName());
        i->setUser((*j)->getUser());
        i->setCommand((*j)->getCommand());
        i->setCpu((*j)->getCpu());
        i->setIcon((*j)->getIcon());
        i->setMemory((*j)->getMemory());
        i->setFullMemory((*j)->getFullMemory());

        cache.append(i);
        v.append(QVariant::fromValue(i));
        delete (*j);
    }
    delete list;

    running=false;
    emit refreshStateChanged();
    emit processListAvailable(v);
}

/**
  * @brief finnished "killing" a process
  * @param pid
  * @param name human readable name of process
  * @param status
  */
void ProcessService::endKill(qint32 pid, QString const &name, int status) {
    purgatory.remove(pid);

    switch (status) {
    case 0:
        emit processKilled(pid,name);
        break;
    default:
        emit processNOTKilled(pid,name);
        break;
    }
}

