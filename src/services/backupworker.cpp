#include "backupworker.h"
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <stdio.h>
#include "calculapplication.h"
#include "../sdcardlocator/sdcardlocator.h"

/**
 * @brief BackupWorker::BackupWorker
 * @param parent
 */
BackupWorker::BackupWorker(QObject *parent) : QObject(parent) {

}

/**
 * @brief BackupWorker::debut
 * @param app_id
 */
void BackupWorker::debut(const QString &app_id) {
    QDir dir(SdcardLocator::getInstance().getLocation() + "/backup/" + app_id );

    if(dir.mkpath(".")) {
        QDateTime d=QDateTime::currentDateTime();

        QString cmd("tar czpf " +
                    dir.canonicalPath() + "/" + QString::number( d.toTime_t()) + ".tgz --ignore-failed-read -C /home/nemo " +
                    CalculApplication::getInstance().dataUrl(app_id).join(" ") + " " +
                    ".config/" + app_id);

        FILE *fichier = popen(cmd.toStdString().c_str(), "r");
        if (fichier != NULL) {
            pclose(fichier);
        }
    }

    emit(fin(app_id));
}



