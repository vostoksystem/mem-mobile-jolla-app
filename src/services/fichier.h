#ifndef FICHIERSERVICE_H
#define FICHIERSERVICE_H

#include <QObject>
#include <QQmlContext>
#include <QThread>
#include <QSharedPointer>
#include "../services/scannerfichier.h"
#include "../bean/Etat.h"
#include <QHash>

#define FICHIERSERVICE_QML_NAME "FichierService"
#define FICHIERSERVICE_THRESHOLD (quint64)20480 // 20MB de dif minimum

class FichierService : public QObject {
    Q_OBJECT

    friend class ScannerFichier;

    class CacheInfo: public QObject
    {
    public:
        explicit CacheInfo(QObject *parent = 0): QObject(parent){}

        quint64 taille;
        QList<Serie *> serie;
    };


public:
    static FichierService& getInstance();
    static void init(QQmlContext *context,QString name =FICHIERSERVICE_QML_NAME);

    /**
      *@brief utilisation du media
      *@return
      **/
    Q_INVOKABLE Etat* utilisation(const QString &source);

    /**
      * @brief recupere les stat sous une source
      * @param force le rechargement (donc toujours liste vide
      * @return liste depuis le cache ou  vide si lancement d'un scan
      * quand le scan et fait, dispo sous signal fin
      */
    Q_INVOKABLE QVariantList statistique(const QString &source,bool force);

private :
    explicit FichierService(QObject *parent = 0);

    ScannerFichier scanner;
    QThread scannerThread;
    bool raf;

    QHash<QString, QSharedPointer<CacheInfo>> cache;

signals:
    // utilisation interne
    void debutScanner(const QString &source);

    // liste des stat pour source disponible
    void fin(const QString &source);

public slots:
    void finScanner(const QString &source,SerieListe serie);
};

#endif // FICHIERSERVICE_H
