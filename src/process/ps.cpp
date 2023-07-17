#include "ps.h"
#include <QDebug>
#include <QRegularExpression>
#include <QDir>
#include <QFile>
#include <QList>
#include <stdio.h>
#include <syscall.h>
#include <unistd.h>
#include "src/desktop/desktop.h"
#include "util.h"

#define PS_BUFFER_SIZE 1024

Ps::Ps(QObject *parent) : QObject(parent) {

    hertz = sysconf(_SC_CLK_TCK);
    qDebug() << "Hertz : " << hertz;

    //PAGESIZE
    pageSize = sysconf(_SC_PAGESIZE);
    qDebug() << "pageSize : " << pageSize;

}

/**
 * @brief Ps::debut
 */
void Ps::start() {

    static QRegularExpression cmdReg("\\((.+)\\)");
    static QRegularExpression nocmdReg("\\(.+\\)");
    

    QList<ProcessInfo *> *pList= new QList<ProcessInfo *>();
    
    
    // sysconf(_SC_CLK_TCK)
    // pour avoir le nombre de click (pour le calcul de l'utilisation cpu
    
    // optenir la taille de la memoire
    
    // parcourir /proc, list des rep -> si un nombre
    // --> recup pid depuis le nom
    // --> recup le non de l'utilisateur depuis le champ user
    // --> recup le nom depuis comm ou cmdline ou stat(1)
    // --> recup taille memoire stat (23) -> 270536704
    
    // --> calcule cpu
    // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat


    /*(base 1 -> pid à 1 et pas 0)
#14 utime - CPU time spent in user code, measured in clock ticks
#15 stime - CPU time spent in kernel code, measured in clock ticks
#16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
#17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
#22 starttime - Time when the process started, measured in clock ticks

total_time = utime + stime
seconds = uptime - (starttime / Hertz)
cpu_usage = 100 * ((total_time / Hertz) / seconds)



    ps --sort=-rss -eo pid=,%mem=,size=,pcpu=,rss=,vsz=,euser=,cmd=
 889253  0.2 21568  0.0 43956 264196 jerome   /usr/lib/x86_64-linux-gnu/libexec/kf5/kiod5
 
cat comm
kiod5

cat cmdline
/usr/lib/x86_64-linux-gnu/libexec/kf5/kiod5

     cat statm
    66049 10989 9477 5 0 5392 0
    
    cat stat
    889253 (kiod5) S 1510 1553 1553 0 -1 4194304 2687 0 6 0 1333 1259 0 0 20 0 3 0 191899219 270536704 10989 18446744073709551615
94055091228672 94055091248149 140722662405840 0 0 0 0 4096 0 0 0 0 17 1 0 0 0 0 0 94055091255856 94055091257360 94055100411904 140722662410580
140722662410624 140722662410624 140722662412236 0
    */
    
    // on commence par recuperer utime
    QStringList uptimeStr = Util::fileToString("/proc/uptime").split( QLatin1Char(' ') );

    qint64 uptime = (qint64)((QString)uptimeStr.at(0)).toFloat();


//        qDebug() << "------------------------------------------";
    //    qDebug() << "uptime " << uptime;

    // on parcoure /proc et recup tout les process
    // @FIXME : doit on faire un filtrage des proc systeme ???
    QDir proc("/proc");
    QFileInfoList li = proc.entryInfoList();
    foreach(QFileInfo f, li) {
        
        if(f.isDir() == false) {
            continue;
        }
        if(f.fileName().startsWith(".")==true) {
            continue;
        }
        
        if(f.isSymLink()==true) {
            continue;
        }
        
        const quint64 pid = f.fileName().toULongLong();
        if(pid < 1) {
            continue;
        }

        // ok on a trouvé un process

        // stat du process
        /*
        #0 PID
        #13 utime - CPU time spent in user code, measured in clock ticks
        #14 stime - CPU time spent in kernel code, measured in clock ticks
        #15 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
        #16 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
        #21 starttime - Time when the process started, measured in clock ticks
        #23 rss - en page -> * 4096 = en octets
        #22 vsz - en octets

        https://stackoverflow.com/questions/1420426/how-to-calculate-the-cpu-usage-of-a-process-by-pid-in-linux-from-c/1424556#1424556
        */



        QString statRaw = Util::fileToString("/proc/" + f.fileName() + "/stat");
        
        QString name = "system"; // si cmdline est vide \ default name
        QRegularExpressionMatch m = cmdReg.match(statRaw);
        if(m.hasMatch()) {
            name =  m.captured(1);
        }

        QStringList stat = statRaw.replace(nocmdReg, "xx").split( QLatin1Char(' ') );

        QStringList statm = Util::fileToString("/proc/" + f.fileName() + "/statm").split( QLatin1Char(' ') );

        qint64 mem = ((QString)statm.at(1)).toLong() * pageSize;
        if( mem == 0) {
            continue;
        }

        // vire les zombies
        if(statm.at(2).compare("Z") == 0) {
            continue;
        }

        // process valide, on continue
        ProcessInfo *p = new ProcessInfo();
        p->setPid( pid );
        p->setUser( f.owner() );
        p->setCommand( Util::fileToString("/proc/" + f.fileName() + "/cmdline").trimmed() );

        // determine la commande / application & type d'application
        ApplicationType type;
        type.setName(name); // nom par defaut ; type déjà sur SYSTEM (defaut)

        // on recherche la commande réélle (sans args, lancher etc...)
        // et on change le nom qui été issu de stat, on va
        Util::processName(p->getCommand(), type);

        p->setName( type.getName() );
        p->setProcess( type.getName() );
        p->setType( type.getType() );

        // et on recherche une traduction pour cette app dans les .destop
        DesktopItem *d = DesktopEntryService::getInstance().foundFor(p->getProcess());
        if(d != NULL) {
            p->setName(d->getName());
            p->setIcon(d->getIcon());
        }

        // calcule l'utilisation memoire
        p->setMemory( mem );
        p->setFullMemory( ((QString)statm.at(2)).toLong() * pageSize + mem );

        // cpu
        qint64 utime = ((QString)stat.at(13)).toLong();
        qint64 stime = ((QString)stat.at(14)).toLong();
        qint64 starttime = ((QString)stat.at(21)).toLong();
        qint64 total_time = utime + stime;
        qint64 seconds = uptime - (starttime / hertz);
        float cpu = ((float)total_time / (float)hertz) / (float)seconds;

        p->setCpu( cpu < 0.01 ? 0.0 : cpu );
               
//        qDebug() << "## " << p->getPid() << " " << p->getProcess() << " " << p->getName() << " -- " << p->getIcon() ;

        pList->append(p);
    }

    emit listAvailable(pList);
}

