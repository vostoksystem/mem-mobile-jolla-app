#ifndef CONFIG_H
#define CONFIG_H
#include <QObject>
#include <QQmlContext>
#include <QSettings>
#include <QDateTime>
#include "../bean/MimeType.h"

using namespace std;

#define CONFIGSERVICE_QML_NAME "serv_config"


#define CONFIG_MEDIA_REFRESH_DELAIS  900
#define MEDIA  "media/"
#define PROCESSUS_TOUS  "processus/tous"
#define PROCESSUS_TOUS_DEF true
#define PROCESSUS_TAILLE  "processus/taillemin"
#define PROCESSUS_TAILLE_DEF  20
#define ANDROID_SCAN "android/scan"
#define ANDROID_SCAN_DEF true // true pour ne PAS inclure les datas android dans le scan
#define DISCLAIMER "disclaimer"
//#define ANDROID_SCAN_DIR "android_storage"
#define ANDROID_SCAN_DIR "Android"

class config : public QObject {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<MimeType> listeMime READ getListeMime NOTIFY statChanged )
    Q_PROPERTY(bool processusSysteme READ getProcessusSysteme WRITE setProcessusSysteme NOTIFY processusChanged )
    Q_PROPERTY(qint64 tailleMinProcessus READ getTailleMinProcessus WRITE setTailleMinProcessus NOTIFY processusChanged)
    Q_PROPERTY(bool androidScan READ getAndroidScan WRITE setAndroidScan NOTIFY androidChanged )
    Q_PROPERTY(bool disclaimer READ getDisclaimer WRITE setDisclaimer NOTIFY disclaimerChanged )

public:
    static config& getInstance();
    static void init(QQmlContext *context);

    // pour les medias / mimes
    Q_INVOKABLE QQmlListProperty<MimeType> getListeMime() { return QQmlListProperty<MimeType>(this, listeMime);}

    MimeType *getMime(QString type);

    // pour les processus systeme
    bool getProcessusSysteme() const {return processusSysteme;}
    void setProcessusSysteme(const bool);

    qint64 getTailleMinProcessus() const {return tailleMinProcessus;}
    void setTailleMinProcessus(qint64);

    // scan
    bool getAndroidScan() const { return androidScan;}
    void setAndroidScan(const bool);

    bool getDisclaimer() const {return disclaimer;}
    void setDisclaimer(const bool);


    // remplace la config utilisateur par les valeurs par defaut
    Q_INVOKABLE void parDefaut();

    // retourne la derniere date de modif de la serie des types mimes
    QDateTime dateModifMime() const;

private :
    explicit config(QObject *parent = 0);

    QSettings settings;

    //mime pour les stat
    // les valeurs par defaut
    QHash<QString, MimeTypeData *> listeMimeDef;
    QList<MimeType *> listeMime;
    QDateTime modifMime;

    // processus
    bool processusSysteme; // true, affiche aussi les processus système
    qint64 tailleMinProcessus;
    QDateTime modifProcess;

    // scan
    bool androidScan;// true pour ne PAS inclure les datas android dans le scan

    bool disclaimer;

    void changementConfig();

signals:
    void statChanged();
    void processusChanged();
    void androidChanged();
    void valeurParDefautChanged();
    void disclaimerChanged();

public slots:
    void changementMime(const QString &);
};

#endif // CONFIG_H
