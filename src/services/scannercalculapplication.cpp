#include "scannercalculapplication.h"
#include <QDir>
#include <QDebug>
#include <QHash>
#include <QStandardPaths>
#include "calculapplication.h"

QString ScannerCalculApplication::home= QStandardPaths::writableLocation( QStandardPaths::HomeLocation);

ScannerCalculApplication::ScannerCalculApplication(QObject *parent) : QObject(parent){
}

/**
 * @brief lance le scan pour une application
 * @param id_desktop
 */
void ScannerCalculApplication::debut(const QString &id_desktop) {
    TailleApplication *taille = new TailleApplication();

    QFileInfo f("/usr/bin/" + id_desktop);
    if(f.exists()) {


        quint64 t = f.size();
        t += boucle("/usr/share/" + id_desktop);
        taille->setApplication(t);
        taille->setConfig(boucle(home+"/.config/"+ id_desktop));

        quint64 td = 0;
        QStringList list = CalculApplication::getInstance().dataUrl(id_desktop);
        foreach (QString str, list) {
            td += boucle(home + "/" + str);
        }
        taille->setDonnee(td);

        taille->setCache(boucle(home+"/"+CalculApplication::getInstance().cacheUrl(id_desktop)));
    }

    emit fin(id_desktop, taille);
}

/**
 * @brief scanne le path et retourne la taille en octet
 * @param source
 * @return
 */
quint64 ScannerCalculApplication::boucle(const QString &path) {

    QFileInfo info(path);
    if(info.exists() && info.isFile()) {
        return info.size();
    }

    QDir dir(path);
    if(dir.exists()==false) {
        return 0;
    }

    quint64 total = 0;

    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot|QDir::NoSymLinks|QDir::Hidden|QDir::Readable);
    QFileInfoList l= dir.entryInfoList();
    foreach (QFileInfo f, l ) {

        if(f.isDir()) {
            total += boucle(f.filePath());
            continue;
        }

        total += f.size();
    }

    return total;
}

