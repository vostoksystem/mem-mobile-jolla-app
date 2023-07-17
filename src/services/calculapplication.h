#ifndef CALCULAPPLICATION_H
#define CALCULAPPLICATION_H

#include <QObject>
#include <QQmlContext>
#include <QHash>
#include <QThread>
#include "../bean/tailleapplication.h"
#include "scannercalculapplication.h"
#include "../bean/redirectionapplication.h"

#define  CALCULAPPLICATION_QML_NAME "CalculApplication"


class CalculApplication : public QObject {
    Q_OBJECT

public:
    /**
     * @brief instance (singleton) for the service
     * @return
     */
    static CalculApplication& getInstance();

    /**
     * @brief register the service into the context so it can be used in qml
     * @param context
     * @param name : register service on this name,  default to CALCULAPPLICATION_QML_NAME
     */
    static void init(QQmlContext *context, QString name=CALCULAPPLICATION_QML_NAME);

    /**
     * @brief
     * @param id_desktop
     * @param force si true, force le calcul même su une valeur existe en cache
     * @return QVariant<TailleApplication> ou null si pas encore dispo
     */
    Q_INVOKABLE QVariant getTaille(const QString &id_desktop, bool force=false);

    /**
      * @brief retourne la version du soft
      * @param id_desktop
      * @return la verison ou vide
      */
    Q_INVOKABLE QString getVersion(const QString &id_desktop);

    /**
      * @brief retourne le non des app qui partage la configuration / data
      * @param id_desktop
      * @return liste de QString, peut être vide
      */
    Q_INVOKABLE QVariantList getPartage(const QString &id_desktop);

    /**
      * @brief retourne la date d'instalation ou mise à jour
      * @param id_desktop
      * @return timestamp
      */
    Q_INVOKABLE quint64 getDateInstall(const QString &id_desktop);

    /**
     * @brief retourne l'url vers le stockage data
     * @param app
     * @return liste des url, dir ou fichier (config, data,etc...)
     */
    QStringList dataUrl(const QString &app) const;

    /**
     * @brief cacheUrl
     * @param app
     * @return
     */
    QString cacheUrl(const QString &app) const;

private :
    explicit CalculApplication(QObject *parent = 0);

    ScannerCalculApplication scanner;
    QThread scannerThread;

    QHash<QString, TailleApplication *> liste;

    QHash<QString, RedirectionApplication *> redirection;

    QHash<QString, QString> cacheVersion;
    QHash<QString, quint64> cacheInstall;

    QString home;
signals:
    /**
     * @brief taille app disponible
     * @param id_desktop
     * @param taille QVariant<TailleApplication>
     */
    void calculDisponible(const QString &id_desktop, QVariant taille);


    // utilisation interne
    void debutScannerImp(const QString &id_desktop);

private slots:
    void finScanner(const QString &id_desktop, TailleApplication *taille);
};

#endif // CALCULAPPLICATION_H
