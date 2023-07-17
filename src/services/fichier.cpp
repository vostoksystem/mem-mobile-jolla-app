#include "fichier.h"
#include <qdebug.h>
#include <QSharedPointer>
#include "../app.h"
#include "config.h"
#include "../vi18n/vi18n.h"

/**
 * @brief service / singleton pour des op sur les fichiers
 * @param parent
 */
FichierService::FichierService(QObject *parent) : QObject(parent), raf(false) {
    scanner.moveToThread(&scannerThread);
    connect(&scannerThread, SIGNAL(finished()), &scanner, SLOT(deleteLater()));
    connect(&scanner, SIGNAL(fin(QString,SerieListe)), this, SLOT(finScanner(QString,SerieListe)), Qt::QueuedConnection );
    connect(this, SIGNAL(debutScanner(const QString &)), &scanner, SLOT(debut(const QString &)), Qt::QueuedConnection );
    scannerThread.start();
}

/**
 * @brief return singleton instance
 * @return FichierService instance
 */
FichierService& FichierService::getInstance() {
    static FichierService instance;
    return instance;
}

/**
 * @brief FichierService::init
 * @param context
 */
void FichierService::init(QQmlContext *context, QString name) {
    context->setContextProperty(name, &FichierService::getInstance());
}

/**
  *@brief utilisation du media
  *@return
  **/
Etat *FichierService::utilisation(const QString &source) {

    FILE *fichier;
    char ligne[App::BUFFER];

    Etat *etat = new Etat();
    etat->setSource(source);

    fichier = popen("df -Pk", "r");
    if (fichier == NULL) {
        return etat;
    }

    while (fgets(ligne, App::BUFFER, fichier) != NULL) {
        QString chaine = QString::fromUtf8(ligne);
        QStringList liste = chaine.split(QRegExp("\\s+"));


        try {
            if(liste.length()< 6) {
                continue;
            }

            if(liste.at(0).startsWith("/")==false) {
                continue;
            }

            if( liste.at(5).compare(source)==0) {
                etat->setTotal(liste.at(1).toLong());
                etat->setActuel(liste.at(2).toLong());
                etat->setMount(liste.at(0));

                //     etat->setVal(liste.at(2).toLong(),liste.at(1).toLong());
                break;
            }
        }catch(...) {

        }
    }

    pclose(fichier);

    return etat;
}

/**
   * @brief recupere les stat sous une source
   * @param force le rechargement
   * @return liste vide si scan en cours, ou alors les donnée depuis le cache
   */
QVariantList FichierService::statistique(const QString &source,bool force) {
    QVariantList v;

    if(raf) {
        return v;
    }

    if(force) {
        raf=true;
        emit debutScanner(source);
        return v;
    }

    QSharedPointer<CacheInfo> tmp = cache.value(source);
    if(tmp.isNull()) {
        // on trouve pas en cache
        raf=true;
        emit debutScanner(source);
        return v;
    }

    QSharedPointer<Etat> etat(FichierService::getInstance().utilisation(source));
    if(etat.isNull()) {
        // connais pas ce point
        return v;
    }

    if((quint64)(etat->getActuel()) < tmp->taille + FICHIERSERVICE_THRESHOLD) {
        // on et à jour
        foreach (Serie *t, tmp->serie) {
            v.append(QVariant::fromValue(t));
        }

        return v;
    }

    raf=true;
    emit debutScanner(source);
    return v;
}

/**
  * @brief FichierService::finScanner
  */
void FichierService::finScanner(const QString &source,SerieListe serie) {
    quint64 somme=0;

    QSharedPointer<Etat> etat(FichierService::getInstance().utilisation(source));
    QSharedPointer< CacheInfo > tmp(new CacheInfo);
    tmp->taille=etat->getActuel();

    QList<Serie *>::iterator j;
    for (j = serie->begin(); j != serie->end(); ++j)
    {
        Serie *s =new Serie();
        s->setIdentifiant((*j)->getIdentifiant());
        s->setCouleur((*j)->getCouleur());
        s->setNom((*j)->getNom());
        s->setValeur((*j)->getValeur());
        s->setInfo((*j)->getInfo());

        tmp->serie.append(s);

        somme += s->getValeur();

        delete(*j);
    }
    delete serie;

    // ajoute autre / libre

    Serie *autre =new Serie();
    autre->setIdentifiant("autre");
    autre->setNom(vI18n::getInstance().get("media.autre"));
    autre->setCouleur(config::getInstance().getMime("autre")->getCouleur().name());
    autre ->setValeur( etat->getActuel() - somme);
    tmp->serie.append(autre);

    Serie *libre = new Serie();
    libre->setIdentifiant("libre");
    libre->setNom(vI18n::getInstance().get("media.libre"));
    libre->setCouleur(config::getInstance().getMime("libre")->getCouleur().name());
    libre ->setValeur( etat->getTotal() - etat->getActuel() );
    tmp->serie.append(libre);

    cache.insert(source, tmp);

    raf=false;

    emit fin(source);
}

