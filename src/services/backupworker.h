#ifndef BACKUPWORKER_H
#define BACKUPWORKER_H

#include <QObject>

class BackupWorker : public QObject {
    Q_OBJECT
public:
    explicit BackupWorker(QObject *parent = 0);

signals:
    void fin(const QString &app_id);


public slots:
    void debut(const QString &app_id);

};

#endif // BACKUPWORKER_H
