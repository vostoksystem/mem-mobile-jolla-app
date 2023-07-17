#ifndef BACKUPSERVICE_H
#define BACKUPSERVICE_H

#include <QObject>
#include <QQmlContext>
#include <QList>

#define BACKUPSERVICE_QML_NAME "BackupService"

class BackupService : public QObject {
    Q_OBJECT

    // si en cours de backup ou de restauration
    Q_PROPERTY(bool entravail READ getEnTravail NOTIFY enTravailChanged)

    // nb d'app en attente de backup
    Q_PROPERTY(quint16 nbBackup READ getNbBackup NOTIFY nbBackupChanged)

public:
    /**
     * @brief instance (singleton) for the service
     * @return
     */
    static BackupService& getInstance();

    /**
     * @brief register the service into the context so it can be used in qml
     * @param context
     * @param name : register service on this name,  default to BACKUPSERVICE_QML_NAME
     */
    static void init(QQmlContext *context, QString name=BACKUPSERVICE_QML_NAME);

    /**
      * @brief retourne la liste des backup pour cette app
      * @param app_id : id de l'application
      * @return peut être vide si aucun
      */
    Q_INVOKABLE QVariantList listePour(const QString &app_id);

    /**
      * @brief fait un backup
      * @param app_id : id de l'application
      * @return true si peut lancer un backup (sdcard et pas en cours)
      */
    Q_INVOKABLE bool backupPour(const QString &app_id);

    /**
      * @brief efface un backup
      * @param app_id : id de l'application
      */
    Q_INVOKABLE void effaceBackup(const QString &app_id, const QString backupid);

    /**
     * @ remplace la config existante par le backup
      * @param app_id : id de l'application
      * @return true si restauration possible
     */
    Q_INVOKABLE bool restaureBackup(const QString &app_id, const QString backupid);

    /**
      * @brief retourne true si peut faire une restauration de app_id : pas déjà en cours et pas en backup
      * @value app_id
      * @return
      **/
    Q_INVOKABLE bool backupRestaurePossible(const QString &app_id) { return backingUpList.contains(app_id) == false && restaureId.compare(app_id) != 0; }

    /**
      *@brief true si en cours de backup ou de restauration
      * @return
      **/
    Q_INVOKABLE bool getEnTravail() { return backingUpList.isEmpty() == false || restaureId.length()>0; }

    /**
      *@brief nb d'app en cours de backup
      *@return
      **/
    Q_INVOKABLE quint16 getNbBackup() { return backingUpList.length(); }

    /**
      * @brief efface le cache pour app_id
      * @param app_id : id de l'application
      */
    Q_INVOKABLE void clearCache(const QString &app_id);

private :
    explicit BackupService(QObject *parent = 0);

    /**
     * @brief liste des app en cours de backup
     */
    QList<QString> backingUpList;

    // id de l'app en cours de restauration, 1 seule à la fois
    QString restaureId;

    static void doBackup(const QString &app_id);

    static void doClearCache(const QString &app_id,const QString &str);
    static void boucle(const QString &str);

    static void doRestaure(const QString &app_id,const QString &str);

    static QString home;
signals:
    /**
     * @brief quand un backup a été terminé
     * @param app_id
     */
    void backupFait(const QString &app_id);

    // utilisation interne
    void backupFaitIntr(const QString &app_id);

    /**
     * @brief si le backup a été éffacé
     * @param app_id id de l'application
     * @param backupid non du backup
     */
    void backupSupprimer(const QString &app_id,const QString backupid);

    /**
     * @brief sur fin d'effacement du cache
     * @param app_id id de l'application
     */
    void cacheEfface(const QString &app_id);

    /**
     * @brief quand la restauration  est terminé
     * @param app_id id de l'application
     */
    void restaureFait(const QString &app_id);

    /**
     * @brief sur debut / fin de backup ou restauration
     */
    void enTravailChanged();

    /**
     * @brief nb d'app en backup a changé
     */
    void nbBackupChanged();

protected slots:
    /**
     * @brief fin de backup
     */
    void finBackupImpl(const QString &app_id);

};

#endif // BACKUPSERVICE_H
