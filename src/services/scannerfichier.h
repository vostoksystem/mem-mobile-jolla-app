#ifndef SCANNER_H
#define SCANNER_H

#include <QObject>
#include <QRegularExpression>
#include <qqml.h>
#include "../bean/Serie.h"

class ScannerFichier :  public QObject {
    Q_OBJECT

protected:
    static const long BUFFER = 2048;
    static const long   MINIMUM = 1024;
    static const long   TABLEAU = 4;

    static long calculer(QString ou, QString quoi);

    /**
     * @brief scanne le rep source pour les exp du tableau exp et ecrit le resultat dans le tableau valeur
     * les tableaux sont de dimension 4 ; si une exp doit être passé, elle est à 0
     * @param source
     * @param valeur
     * @param exp
     */
    static void boucle(const QString source, qint64 * valeur, QRegularExpression ** exp);


public :
   explicit ScannerFichier(QObject *parent = 0);

public slots:
    /**
     * @brief lance le scan pour source
     */
    void debut(const QString &source);

signals:
    /**
     * @brief fin de scan pour source
     * @param source
     * @param serie
     */
    void fin(const QString &source, SerieListe serie);
};


#endif // SCANNER_H
