#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>
#include <QString>
#include <QDebug>
#include "applicationtype.h"

using namespace std;

/**
 * @brief The ProcessInfo class
 * @since 0.16.1 percentMemory removed, call systemMemory from ProcessService ; state signal removed
 */
class ProcessInfo : public QObject {

    Q_OBJECT
    Q_PROPERTY(quint32 pid READ getPid CONSTANT )
    Q_PROPERTY(QString name READ getName CONSTANT )             // display name
    Q_PROPERTY(QString process READ getProcess CONSTANT )       // name of the real process (ex ; harbour_mem)
    Q_PROPERTY(QString command READ getCommand CONSTANT )       // full command
    Q_PROPERTY(QString icon READ getIcon CONSTANT )
    Q_PROPERTY(quint64 memory READ getMemory CONSTANT )
    Q_PROPERTY(quint64 fullMemory READ getFullMemory CONSTANT )
    Q_PROPERTY(float cpu READ getCpu CONSTANT )
    Q_PROPERTY(ApplicationType::Type type READ getType CONSTANT )
    Q_PROPERTY(QString user READ getUser CONSTANT )

private:
    quint32 pid;
    QString name;
    QString process;
    QString command;
    QString icon;
    float cpu; // pourcent of cpu usage
    quint64 memory; // octet
    quint64 fullMemory; // octet

    ApplicationType::Type type;
    QString user;

public :
    explicit ProcessInfo(QObject *parent = 0);

    /**
     * @brief Human readable name of application
     * @return
     */
    QString getName() const {return name;}
    void setName(const QString &v) {name=v;}

    /**
     * @brief name of the command
     * @return
     */
    QString getProcess() const {return process;}
    void setProcess(const QString &v) {process=v;}

    /**
     * @brief full command with path and argument
     * @return
     */
    QString getCommand() const {return command;}
    void setCommand(const QString &v) {command=v;}

    /**
     * @brief icon if found in desktop file
     * @return
     */
    QString getIcon() const {return icon;}
    void setIcon(const QString &v) {icon=v;}

    /**
     * @brief memory usage in kb
     * @return
     */
    quint64 getMemory() const {return memory;}
    void setMemory(const quint64 v) {memory=v;  }

    /**
      @since 0.16.1
     * @brief full memory usage in kb (real + shared)
     * @return
     */
    quint64 getFullMemory() const {return fullMemory;}
    void setFullMemory(const quint64 v) {fullMemory=v; }

    /**
     * @brief cpu usage in percent
     * @return
     */
    float getCpu() const {return cpu;}
    void setCpu(const float v) {cpu=v;}

    /**
      @since 0.16.1
     * @brief getType
     * @return
     */
    ApplicationType::Type getType() const {return type;}
    void setType(const ApplicationType::Type v) {type = v;}

    /**
     * @brief getPid
     * @return
     */
    quint32 getPid() const {return pid;}
    void setPid(const quint32 v){pid = v;}

    /**
     * @brief User running the process
     * @return
     */
    QString getUser() const {return user;}
    void setUser(const QString &v) {user=v;}

};
typedef QList<ProcessInfo *> * ProcessInfoList;


#endif // PROCESS_H
