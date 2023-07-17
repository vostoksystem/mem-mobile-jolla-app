#include "scannerfichier.h"
#include <QDebug>
#include <QList>
#include <qregexp.h>
#include <QDir>
#include <stdio.h>
#include "config.h"
#include "../vi18n/vi18n.h"
#include "../app.h"
/**
 * @brief ScannerFichier::ScannerFichier
 * @param parent
 */
ScannerFichier::ScannerFichier(QObject *parent) : QObject(parent) {
}

/**
 * @brief ScannerFichier::debut
 */
void ScannerFichier::debut(const QString &chemin) {
    QList<Serie *> *serie = new QList<Serie *>();

    qint64 valeur[TABLEAU]={0,0,0,0};
    QString nom[TABLEAU]={"video", "musique","image","doc"};
    QRegularExpression *exp[TABLEAU]={NULL, NULL,NULL,NULL};

    for (int k=0; k < TABLEAU;k++) {
        MimeType *type=config::getInstance().getMime(nom[k]);
        if(type->getExtensionPossible()==false || type->getActif()==false) {
            continue;
        }
        exp[k]= new QRegularExpression(
                    "\\.(" + type->getExtension().replace(" ", "|") + ")$",
                    QRegularExpression::CaseInsensitiveOption);
    }

    boucle(chemin,valeur,exp);

    for (int k=0; k < TABLEAU;k++) {
        MimeType *type=config::getInstance().getMime(nom[k]);
        if(type->getExtensionPossible()==false || type->getActif()==false) {
            continue;
        }

        if(valeur[k]< App::MINIMUM) {
            continue;
        }

        QString id("media.");
        id.append(type->getNom());

        Serie *s= new Serie();
        s->setIdentifiant(nom[k]);
        s->setNom(vI18n::getInstance().get(id.toUtf8().constData()));
        s->setCouleur(type->getCouleur().name());
        s->setValeur(valeur[k]/1024 );
        serie->append(s);
    }

    // penser à detruire les regexp
    for (int k=0; k < TABLEAU;k++) {
        delete(exp[k]);
    }

    emit fin(chemin, serie);
}

/**
  * scanne le rep source pour les exp du tableau exp et ecrit le resultat dans le tableau valeur
  * les tableaux sont de dimension 4 ; si une exp doit être passé, elle est à 0
  * @brief ScannerFichier::boucle
  * @param source
  * @param valeur
  * @param exp
  */
void ScannerFichier::boucle(const QString source, qint64 * valeur, QRegularExpression ** exp) {

    QDir dir(source);
    if(dir.exists()==false) {
        return;
    }

    bool android = config::getInstance().getAndroidScan();

    QFileInfoList l=  dir.entryInfoList();
    QFileInfoList::iterator i;
    for (i = l.begin(); i != l.end(); ++i) {
        if((*i).fileName().startsWith(".")==true) {
            continue;
        }

        if((*i).isReadable()==false) {
            continue;
        }
        if((*i).isSymLink()==true) {
            continue;
        }

        if((*i).isDir()) {

            if(android) {
                if((*i).fileName().compare(ANDROID_SCAN_DIR)==0) {
                    continue;
                }
            }

            // qDebug()<< "sur " << (*i).fileName() ;

            boucle((*i).filePath(),valeur,exp);
            continue;
        }

        for(int k=0;k<TABLEAU;k++) {

            if(exp[k]==NULL) {
                continue;
            }

            if(exp[k]->match((*i).fileName()).hasMatch()) {
                valeur[k] +=(*i).size();
                break;
            }
        }
    }
}

/**
 * @param ou
 * @param quoi
 * @return
 */
long ScannerFichier::calculer(QString ou, QString quoi) {

    FILE *fichier;
    char buf[App::BUFFER];

    QString commande("find " + ou + " -iregex '.*\\." +quoi +"' -print0 2>/dev/null | du --files0-from=- -c | tail -1");

    fichier = popen(commande.toUtf8().constData(), "r");
    if (fichier == NULL) {
        return 0;
    }

    QRegExp expression("^(\\d+)\\s+.*$");
    expression.setCaseSensitivity(Qt::CaseInsensitive);

    while (fgets(buf, App::BUFFER, fichier) != NULL) {
        QString chaine = QString::fromUtf8(buf);
        try {
            if (expression.indexIn(chaine) != -1) {
                pclose(fichier);

                return expression.cap(1).toLong();
            }
        }catch(int n) {
        }
    }

    pclose(fichier);
    return 0;
}

