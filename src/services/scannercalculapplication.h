#ifndef SCANNERCALCULAPPLICATION_H
#define SCANNERCALCULAPPLICATION_H

#include <QObject>
#include "../bean/tailleapplication.h"

class ScannerCalculApplication : public QObject {
    Q_OBJECT

public:
    explicit ScannerCalculApplication(QObject *parent = 0);

private :

    /**
     * @brief scanne le path et retourne la taille en octet
     * @param source
     * @return
     */
    static quint64 boucle(const QString &path);

    static QString home;

public slots:
    /**
     * @brief lance le scan pour une application
     * @param id_desktop
     */
    void debut(const QString &id_desktop);

signals:
    /**
     * @brief trouve les info pour l'application
     * @param id_desktop
     * @param taille jamias null mais pet être vide si app inconnu
     */
    void fin(const QString &id_desktop, TailleApplication *taille);
};

#endif // SCANNERCALCULAPPLICATION_H
