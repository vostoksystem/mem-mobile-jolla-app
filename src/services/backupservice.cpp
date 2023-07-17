#include "backupservice.h"
#include <QDir>
#include <QFileInfo>
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>
#include <QtDebug>
#include <QStandardPaths>
#include <nemonotifications-qt5/notification.h>
#include <stdio.h>
#include "../sdcardlocator/sdcardlocator.h"
#include "../vi18n/vi18n.h"
#include "../desktop/desktop.h"
#include "calculapplication.h"

QString BackupService::home= QStandardPaths::writableLocation( QStandardPaths::HomeLocation);

/**
 * @brief BackupService::BackupService
 * @param parent
 */
BackupService::BackupService(QObject *parent) : QObject(parent),restaureId("") {

    connect(this, SIGNAL(backupFaitIntr(QString)), this, SLOT(finBackupImpl(QString)));
}

/**
 * @brief instance (singleton) for the service
 * @return
 */
BackupService& BackupService::getInstance() {
    static  BackupService instance;
    return instance;
}

/**
 * @brief register the service into the context so it can be used in qml
 * @param context
 * @param name : register service on this name,  default to BACKUPSERVICE_QML_NAME
 */
void BackupService::init(QQmlContext *context, QString name) {
    context->setContextProperty(name,&BackupService::getInstance());
}

/**
  * @brief retourne la liste des backup pour cette app
  * @param app_id : id de l'application
  * @return peut être vide si aucun
  */
QVariantList BackupService::listePour(const QString &app_id) {
    QVariantList list;

    if(SdcardLocator::getInstance().isPresent()==false) {
        return list;
    }

    QDir dir(SdcardLocator::getInstance().getLocation() + "/backup/" + app_id );

    if( dir.exists() == false || dir.isReadable() == false) {
        return list;
    }

    dir.setFilter(QDir::Files|QDir::Readable);
    dir.setNameFilters(QStringList()<<"*.tgz");

    QFileInfoList l = dir.entryInfoList();
    foreach(QFileInfo f,l) {
        list.append(f.fileName());
    }

    return list;
}

/**
  * @brief fait un backup
  * @param app_id : id de l'application
  */
bool BackupService::backupPour(const QString &app_id) {

    if(SdcardLocator::getInstance().isPresent()==false) {
        return false;
    }

    if(backupRestaurePossible(app_id)==false) {
        return false;
    }

    backingUpList.append(app_id);
    emit nbBackupChanged();

    if(backingUpList.size()==1) {
        // rien en cours, lance
        emit enTravailChanged();
        QFuture<void> f = QtConcurrent::run(doBackup,backingUpList.first());
    }

    return true;
}

/**
 * @brief BackupService::doBackup
 * @param app_id
 * @param str
 */
void BackupService::doBackup(const QString &app_id) {

    QDir dir(SdcardLocator::getInstance().getLocation() + "/backup/" + app_id );

    if(dir.mkpath(".")) {
        QDateTime d=QDateTime::currentDateTime();

        QString cmd("tar czpf " +
                    dir.canonicalPath() + "/" + QString::number( d.toTime_t()) + ".tgz -C " + home + " " +
                     CalculApplication::getInstance().dataUrl(app_id).join(" ") + " " +
                    ".config/" + app_id);

        FILE *fichier = popen(cmd.toStdString().c_str(), "r");
        if (fichier != NULL) {
            pclose(fichier);
        }

        QFile f(dir.canonicalPath() + "/" + QString::number( d.toTime_t()) + ".tgz");
        if(f.exists() && f.size() <50) {
            // c'est une archive vide
           f.remove();
        }
    }

    emit BackupService::getInstance().backupFaitIntr(app_id);
}

/**
 * @ remplace la config existante par le backup
  * @param app_id : id de l'application
 */
bool BackupService::restaureBackup(const QString &app_id, const QString backupid) {

    if(SdcardLocator::getInstance().isPresent()==false) {
        return false;
    }

    if(backupRestaurePossible(app_id)==false) {
        return false;
    }

    QString str = SdcardLocator::getInstance().getLocation() + "/backup/" + app_id + "/" + backupid;

    QFileInfo info(str);
    if(info.exists()==false || info.isReadable() == false ) {
        return false;
    }

    restaureId = app_id;

    emit enTravailChanged();
    QFuture<void> f = QtConcurrent::run(doRestaure,app_id,str);
    return true;
}

/**
 * @brief BackupService::doRestaure
 * @param app_id
 * @param str
 */
void BackupService::doRestaure(const QString &app_id,const QString &str) {

    QString cmd("tar xzpf " + str + " -C " + home + " " );

    FILE *fichier = popen(cmd.toStdString().c_str(), "r");
    if (fichier != NULL) {
        pclose(fichier);
    }

    BackupService::getInstance().restaureId="";
    CalculApplication::getInstance().getTaille(app_id,true);

    emit BackupService::getInstance().restaureFait(app_id);
    emit BackupService::getInstance().enTravailChanged();

    Notification n;
    n.setPreviewBody(vI18n::getInstance().get( "app.restorefait" ));
    n.setHintValue("x-nemo-icon", "image://theme/icon-s-sync");
    n.publish();
}

/**
  * @brief efface un backup
  * @param app_id : id de l'application
  */
void BackupService::effaceBackup(const QString &app_id, const QString backupid) {

    if(SdcardLocator::getInstance().isPresent()==false) {
        return;
    }

    if(backingUpList.contains(app_id)) {
        return;
    }

    QFile f(SdcardLocator::getInstance().getLocation() + "/backup/" + app_id + "/"+ backupid );

    if(f.remove()) {
        emit backupSupprimer(app_id, backupid);
    }
}

/**
  * @brief efface le cache pour app_id
  * @param app_id : id de l'application
  */
void BackupService::clearCache(const QString &app_id) {

    QString str = CalculApplication::getInstance().cacheUrl(app_id);
    if(str.length()==0) {
        emit cacheEfface(app_id);
        return;
    }

    QFuture<void> f = QtConcurrent::run(doClearCache,app_id, home + "/" + str);
}

/**
 * @brief BackupService::doClearCache
 * @param str
 */
void BackupService::doClearCache(const QString &app_id, const QString &str) {

    boucle(str);
    CalculApplication::getInstance().getTaille(app_id,true);

    emit  BackupService::getInstance().cacheEfface(app_id);
}

/**
 * @brief doClearCache
 * @param str
 */
void BackupService::boucle(const QString &str) {

    QDir dir(str);
    if(dir.exists() == false || dir.isReadable() == false) {
        return;
    }

    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot|QDir::Hidden|QDir::Readable);
    QFileInfoList l= dir.entryInfoList();
    foreach (QFileInfo f, l ) {
        if(f.isDir()) {
            boucle(f.absoluteFilePath());
            QDir d(f.absoluteFilePath());
            d.remove(".");
        }

        QFile file(f.absoluteFilePath());
        file.remove();
    }
}

/**
 * @brief fin de backup
 */
void BackupService::finBackupImpl(const QString &app_id) {

    backingUpList.removeOne(app_id);
    emit nbBackupChanged();

    emit(backupFait(app_id));
    emit enTravailChanged();

    // fin de liste ?
    if(backingUpList.isEmpty()) {
        Notification n;
        n.setPreviewBody(vI18n::getInstance().get( "app.backupfait" ));
        n.setHintValue("x-nemo-icon", "image://theme/icon-s-sync");
        n.publish();
        return;
    }

    QFuture<void> f = QtConcurrent::run(doBackup,backingUpList.first());
}
