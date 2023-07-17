#include "calculapplication.h"
#include <QDebug>
#include "../desktop/desktop.h"

CalculApplication::CalculApplication(QObject *parent) : QObject(parent) {

    RedirectionApplication *r = new RedirectionApplication();
    r->ajouterDonnee(".local/share/org.sailfishos/sailfish-browser");
    r->ajouterDonnee(".mozilla/mozembed/storage");
    r->ajouterDonnee(".mozilla/mozembed/key3.db");
    r->ajouterDonnee(".mozilla/mozembed/webappsstore.sqlite");
    r->ajouterDonnee(".mozilla/mozembed/webappsstore.sqlite-shm");
    r->ajouterDonnee(".mozilla/mozembed/webappsstore.sqlite-wal");

    r->setCache(".mozilla/mozembed/cache2");
    redirection.insert("sailfish-browser",r );

    r = new RedirectionApplication();
    r->ajouterDonnee(".local/share/org.sailfishos/Settings");
    r->setCache(".cache/org.sailfishos/Settings");
    redirection.insert("jolla-settings",r );

    r = new RedirectionApplication();
    r->ajouterDonnee(".local/share/commhistory");
    r->ajouterPartage("voicecall-ui");
    redirection.insert("jolla-messages",r );

    r = new RedirectionApplication();
    r->ajouterDonnee(".local/share/commhistory");
    r->ajouterPartage("jolla-messages");
    redirection.insert("voicecall-ui",r );

    r = new RedirectionApplication();
    r->setCache(".cache/media-art");
    r->ajouterPartage("jolla-mediaplayer");
    redirection.insert("jolla-gallery",r );

    r = new RedirectionApplication();
    r->setCache(".cache/media-art");
    r->ajouterPartage("jolla-gallery");
    redirection.insert("jolla-mediaplayer",r );

    scanner.moveToThread(&scannerThread);
    connect(&scannerThread, SIGNAL(finished()), &scanner, SLOT(deleteLater()));
    connect(&scanner, SIGNAL(fin(QString,TailleApplication*)), this, SLOT(finScanner(QString,TailleApplication*)), Qt::QueuedConnection );
    connect(this, SIGNAL(debutScannerImp(QString)), &scanner, SLOT(debut(QString)), Qt::QueuedConnection );
    scannerThread.start();
}

/**
 * @brief instance (singleton) for the service
 * @return
 */
CalculApplication& CalculApplication::getInstance() {
    static CalculApplication instance;
    return instance;
}

/**
 * @brief register the service into the context so it can be used in qml
 * @param context
 * @param name : register service on this name,  default to CALCULAPPLICATION_QML_NAME
 */
void CalculApplication::init(QQmlContext *context, QString name) {
    context->setContextProperty(name,&CalculApplication::getInstance());
}

/**
 * @brief
 * @param id_desktop
     * @param force si true, force le calcul même su une valeur existe en cache
 * @return QVariant<TailleApplication> ou null si pas encore dispo
 */
QVariant CalculApplication::getTaille(const QString &id_desktop, bool force) {

    if(force == false) {
        TailleApplication * taille = liste.value(id_desktop);

        if( taille != NULL) {
            TailleApplication * tmp = new TailleApplication();
            tmp->setApplication(taille->getApplication());
            tmp->setConfig(taille->getConfig());
            tmp->setCache(taille->getCache());
            tmp->setDonnee(taille->getDonnee());
            return QVariant::fromValue(tmp);
        }
    }
    emit debutScannerImp(id_desktop);

    return QVariant();
}

/**
  * @brief retourne le non des app qui partage la configuration / data
  * @param id_desktop
  * @return liste de QString, peut être vide
  */
QVariantList CalculApplication::getPartage(const QString &id_desktop) {
    QVariantList list;

    if(redirection.contains(id_desktop)) {
        QList<QString> partage = redirection.value(id_desktop)->getPartage();
        foreach (QString str, partage) {
            DesktopItem *d = DesktopEntryService::getInstance().foundFor(str);
            list.append( d != NULL ? d->getName(): str );
        }
    }

    return list;
}

/**
 * @brief CalculApplication::finScanner
 * @param id_desktop
 * @param taille
 */
void CalculApplication::finScanner(const QString &id_desktop, TailleApplication *taille){

    TailleApplication * tmp = new TailleApplication();
    tmp->setApplication(taille->getApplication());
    tmp->setConfig(taille->getConfig());
    tmp->setCache(taille->getCache());
    tmp->setDonnee(taille->getDonnee());
    liste.insert(id_desktop,tmp);

    //delete taille;
    emit( calculDisponible(id_desktop,QVariant::fromValue(taille)));
}

/**
  * @brief retourne la version du soft
  * @param id_desktop
  * @return la verison ou vide
  */
QString CalculApplication::getVersion(const QString &id_desktop) {
    // which harbour-vostok_meminfo
    // rpm -q -f harbour-vostok_meminfo
    // rpm -q --queryformat '%{VERSION}\n' harbour-vostok_meminfo-0.8.12-1.armv7hl

    if(cacheVersion.contains(id_desktop)) {
        return cacheVersion.value(id_desktop);
    }

    QString cmd("rpm -q --queryformat '%{VERSION}\n' $(rpm -qf $(which " + id_desktop + "))");

    QString val;

    FILE *fichier = popen(cmd.toStdString().c_str(), "r");
    if (fichier != NULL) {
        char ligne[64];
        if(fgets(ligne, 64, fichier) != NULL) {
            QString str(ligne);
            if(str.simplified().startsWith("rpm")==false) {
                val = str.simplified();
            }
        }
        pclose(fichier);
    }

    cacheVersion.insert(id_desktop,val);
    return val;
}


/**
  * @brief retourne la date d'instalation ou mise à jour
  * @param id_desktop
  * @return timestamp
  */
quint64 CalculApplication::getDateInstall(const QString &id_desktop) {

    if(cacheInstall.contains(id_desktop)) {
        return cacheInstall.value(id_desktop);
    }

    QString cmd("rpm -q --queryformat '%{INSTALLTIME}\n' $(rpm -qf $(which " + id_desktop + "))");

    quint64 val = 0;

    FILE *fichier = popen(cmd.toStdString().c_str(), "r");
    if (fichier != NULL) {
        char ligne[64];
        if(fgets(ligne, 64, fichier) != NULL) {
            QString str(ligne);
            if(str.simplified().startsWith("rpm")==false) {
                val= str.simplified().toLong();
            }
        }
        pclose(fichier);
    }

    cacheInstall.insert(id_desktop,val);
    return val;
}

/**
 * @brief retourne l'url vers le stockage data
 * @param app
 * @return
 */
QStringList CalculApplication::dataUrl(const QString &app) const {

    RedirectionApplication *r = redirection.value(app);

    if( r != NULL) {
        if(r->getDonnee().length() !=0 ) {
            return r->getDonnee();
        }
    }

    return QStringList() << ".local/share/" + app;
}

/**
 * @brief ScannerCalculApplication::cacheUrl
 * @param app
 * @return
 */
QString CalculApplication::cacheUrl(const QString &app) const {

    RedirectionApplication *r = redirection.value(app);

    if( r != NULL) {
        if(r->getCache() != NULL) {
            return r->getCache();
        }
    }

    return ".cache/" + app;
}
