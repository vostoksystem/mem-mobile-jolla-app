#ifndef DESKTOPSERVICE_H
#define DESKTOPSERVICE_H

#include <QObject>
#include <QQmlContext>
#include <QList>
#include <QHash>
#include <QFileSystemWatcher>
#include "desktopitem.h"

#define  DESKTOPSERVICE_QML_NAME "DesktopService"
#define  DESKTOPSERVICE_RELOAD_THRESHOLD 1200000

using namespace std;

/**
 * @brief service to manage the list of desktop entry (application)
 * For obvious privilege raison, this service cannot makes any changes, just read data
 * @author levreau.jerome@vostoksystem.eu
 * @version 1.0
 */
class  DesktopEntryService : public QObject {
    Q_OBJECT

    Q_PROPERTY(QVariantList list READ getList NOTIFY listChanged )

    friend class QFileSystemWatcher;

    enum IconSize {
        TINY=0,
        MEDIUM,
        LARGE
    };

public:
    /**
     * @brief instance (singleton) for the service
     * @return
     */
    static DesktopEntryService& getInstance();

    /**
     * @brief register the service into the context so it can be used in qml
     * @param context
     * @param name : register service on this name,  default to DESKTOPSERVICE_QML_NAME
     */
    static void init(QQmlContext *context, QString name=DESKTOPSERVICE_QML_NAME);

    /**
      * if true, won't list service entry ; must be called before the fist getList or a getList(true)
      * @param nodisplay ; true : don't list service, just ui app
      */
     Q_INVOKABLE void setNoDisplay(bool nodisplay=true) {this->nodisplay=nodisplay;}

    /**
      * @brief return  the list of desktop entry
      * @param force : force a fresh reload, else use cached entry
      * @return QVariantList<DesktopItem>
      */
    Q_INVOKABLE QVariantList getList(bool force=false);

    /**
     * @brief try to found a desktop entry matching the process named process
     * @param process : process name
     * @return entry or null if not found
     */
    DesktopItem *foundFor(const QString &process) const;

    /***
     * @brief same as foundFor but as a qvariant
     * @param process : process name
     * @return  empty qvariant if not found
     **/
    Q_INVOKABLE QVariant foundForAsQVariant(const QString &process);



private :
    QHash<QString, DesktopItem *> list; // process name, data
    QHash<QString, QString> iconeCache; // cache des icones / process


    QFileSystemWatcher watcher;

    explicit DesktopEntryService(QObject *parent = 0);

    bool nodisplay;

    /**
     * @brief reload the cache from disk entries
     */
    void reload();

    /**
     * @brief load data from an desktop url
     * @param v path to the desktop file
     * @return
     */
    DesktopItem  *desktopFile(const QString &v);

    /**
     * @brief foundIconFor
     * @param name
     * @return
     */
    QString iconFor(const QString &name);

signals:
    void listChanged();

private slots :
    void watcherImp(const QString&);
};

#endif // DESKTOPSERVICE_H
