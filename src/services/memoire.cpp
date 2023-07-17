#include "memoire.h"
#include <qregexp.h>
#include <stdio.h>
#include <qdebug.h>
#include <QFile>
#include <QStringList>
#include "../app.h"


/**
 * @brief memoire::memoire
 * @param parent
 */
memoireService::memoireService(QObject *parent) : QObject(parent) {
}

/**
 * @brief memoire::getInstance
 * @return
 */
memoireService& memoireService::getInstance() {
    static memoireService instance;
    return instance;
}

/**
 * @brief memoire::init
 * @param context
 * @param name
 */
void memoireService::init(QQmlContext *context, QString name) {
    context->setContextProperty(name,&memoireService::getInstance());
}

/**
  * @brief memoire::etat
  * @param v
  * @deprecated
  */
Etat *memoireService::utilisationOld(){

    FILE *fichier;
    char ligne[App::BUFFER];

    Etat *etat = new Etat();
    etat->setSource("mem:");

    fichier = popen("free", "r");
    if (fichier == NULL) {
        return etat;
    }

    QRegExp expression("^Mem:\\s*(\\d+)\\s*(\\d+)\\s*(\\d+)\\s*(\\d+)\\s*(\\d+)\\s*(\\d+)");
    while (fgets(ligne, App::BUFFER, fichier) != NULL) {
        qDebug() << "trouve une ligne " << ligne;

        if (expression.indexIn(ligne) != -1) {
            etat->setVal(
                        expression.cap(2).toLong() - expression.cap(5).toLong() - expression.cap(6).toLong(),
                        expression.cap(1).toLong()
                        );
            break;
        }
    }

    pclose(fichier);

    return etat;
}

/**
  * @brief memoire::etat
  * @param v
  * @since 0.13.1
  */
Etat *memoireService::utilisation(){

    Etat *etat = new Etat();
    etat->setSource("mem:");

    QFile f("/proc/meminfo");
    if( f.open(QIODevice::ReadOnly|QIODevice::Text) ==false) {
        return etat;
    }

    quint64 total = 0;          // MemTotal:
    quint64 free = 0;           // MemFree:
    quint64 buffers = 0;        // Buffers:
    quint64 cached = 0;         // Cached:

    // @TIP : memeinfo renvoi toujours 0, un qteststream sur l'io ne peut PAS fonctionner
    QByteArray contents = f.readAll();

    QTextStream in(&contents);
    while(!in.atEnd()) {
        QStringList l = in.readLine().split(QRegExp("\\s+"));
        if(l.length() != 3) {
            continue;
        }

        if( ((QString)l.at(0)).compare("MemTotal:") == 0 ) {
            total = ((QString)l.at(1)).toLongLong();
        } else if( ((QString)l.at(0)).compare("MemFree:") == 0 ) {
            free = ((QString)l.at(1)).toLongLong();
        } else if( ((QString)l.at(0)).compare("Buffers:") == 0 ) {
            buffers = ((QString)l.at(1)).toLongLong();
        } else if( ((QString)l.at(0)).compare("Cached:") == 0 ) {
            cached  = ((QString)l.at(1)).toLongLong();
        }
    }

    etat->setVal(free+cached+buffers, total);

    f.close();

    return etat;
}

