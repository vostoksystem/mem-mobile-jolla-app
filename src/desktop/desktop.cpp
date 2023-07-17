#include "desktop.h"
#include <qdebug.h>
#include <QtQml>
#include <QMetaType>
#include <QQmlEngine>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QRegularExpression>

/**
 * @brief DesktopService::DesktopService
 * @param parent
 */
DesktopEntryService::DesktopEntryService(QObject *parent) : QObject(parent),nodisplay(true) {

    reload();

    // because desktop def go there
    watcher.addPath("/usr/share/applications");
    connect(&watcher, SIGNAL(fileChanged(QString)), this, SLOT(watcherImp(QString)));
}

/**
 * @brief instance (singleton) for the service
 * @return
 */
DesktopEntryService& DesktopEntryService::getInstance() {
    static DesktopEntryService instance;
    return instance;
}

/**
 * @brief register the service into the context so it can be used in qml
 * @param context
 * @param name : register service on this name,  default to DESKTOPSERVICE_QML_NAME
 */
void DesktopEntryService::init(QQmlContext *context, QString name) {
    context->setContextProperty(name,&DesktopEntryService::getInstance());

    qmlRegisterUncreatableType<DesktopItem>("DesktopItem", 1, 0,"DesktopItem","");
    qRegisterMetaType<DesktopItemList>( "DesktopItemList" );
}

/**
  * @brief return the list of desktop entry
  * @param force : force a fresh reload, else use cached entry
  * @return
  */
QVariantList DesktopEntryService::getList(bool force) {

    if(force || list.isEmpty() ) {
        reload();
    }

   qDebug() << "??? desktop list " << list.size();
    QVariantList v;

    foreach (DesktopItem *i, list) {
        v.append(QVariant::fromValue(i));
    }

    return v;
}

/**
 * @brief reload the cache from disk entries
 */
void DesktopEntryService::reload() {

    static QRegularExpression desktopExp( "\\.desktop$", QRegularExpression::CaseInsensitiveOption);

    list.clear();

    // yep, durty but it's here and nowhere else
    QDir dir("/usr/share/applications");
    if((dir.exists()==false) || (dir.isReadable()==false)) {
        return;
    }

    dir.setFilter(QDir::Files|QDir::Readable|QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Name);

    QFileInfoList l = dir.entryInfoList();
    foreach (QFileInfo f, l ) {

        if(desktopExp.match(f.fileName()).hasMatch()==false) {
            continue;
        }

        DesktopItem *d = desktopFile(f.filePath() );
        if(d != NULL) {
            list.insert(d->getId(), d);

            qDebug() << "desk : " << d->getId() << " " << d->getIcon();
        }
    }

    emit listChanged();
}

/**
 * @brief try to found a desktop entry matchin the process named process
 * @param process : process name
 * @return entry or null if not found
 */
DesktopItem * DesktopEntryService::foundFor(const QString &process) const {
    if(list.contains(process)==true) {
        return list.value(process);
    }

    return NULL;
}

/**
 * @brief foundForAsQVariant
 * @param process
 * @return
 */
QVariant DesktopEntryService::foundForAsQVariant(const QString &process) {

    DesktopItem *d = foundFor(process);
    if(d != NULL) {
        // FIX ME : storing d into qvariant cause a seg fault, is there a better solution !!?
        DesktopItem *tmp=new DesktopItem();
        tmp->setExec(d->getExec());
        tmp->setIcon(d->getIcon());
        tmp->setId(d->getId());
        tmp->setName(d->getName());

        return QVariant::fromValue(tmp);
    }

    return QVariant();
}

/**
 * @brief watcherImp
 * @param str
 */
void DesktopEntryService::watcherImp(const QString& ) {
    reload();
}

/**
 * @brief load data from an desktop url
 * @param v path to the desktop file
 * @return
 */
DesktopItem *DesktopEntryService::desktopFile(const QString &v) {

    static QRegularExpression ligneExp("^([^=]+)=(.*)");
    static QRegularExpression nameExp("([^\\/]+)$");
    //static QRegularExpression silicaExp( "^invoker.+/usr/bin/([^ ]+)", QRegularExpression::CaseInsensitiveOption);
    //static QRegularExpression silicaExp( "^.*silica-qt5[^/]+/usr/bin/([^ ]+)", QRegularExpression::CaseInsensitiveOption);
    //static QRegularExpression jollaExp( "([^/ ]+)$", QRegularExpression::CaseInsensitiveOption);

    QFile file(v);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {

        QHash<QString,QString> data;

        QTextStream in(&file);
        while(!in.atEnd()) {
            QString line = in.readLine();

            QRegularExpressionMatch ml = ligneExp.match(line);
            if(ml.hasMatch()) {
                data.insert(ml.captured(1).toLower(),ml.captured(2) );
            }
        }

        file.close();

        if(data.value("exec").length()==0) {
            return NULL;
        }

        if( (data.value("nodisplay") == "true") && nodisplay ) {
            return NULL;
        }

        DesktopItem *d = new DesktopItem();
        d->setId(QFileInfo(file).fileName().replace(".desktop", "").toLower());
        d->setName(data.value("name"));
        d->setIcon(data.value("icon").startsWith('/') ? data.value("icon") : iconFor(data.value("icon")) );
        d->setExec(data.value("exec"));

        // android ?
        // X-apkd-packageName=com.here.app.maps
        // note : less app android on aussi x-nemo-application-type
        if(data.contains("x-apkd-packagename")) {
            // android
            d->setId(data.value("x-apkd-packagename"));
            d->setType("ANDROID");
            return d;
        }

        // silica / meego / jolla
        // X-MeeGo-Logical-Id=settings-ap-name
        // Exec=/usr/bin/sailjail -p jolla-email.desktop /usr/bin/jolla-email
        // @FIXME : est s'il y a des arguments !!???
        if(data.contains("x-meego-logical-id")) {
            QRegularExpressionMatch m = nameExp.match(data.value("Exec"));
            if(m.hasMatch()) {
                d->setId(m.captured(1));
                d->setType("SAILFISH");
                return d;
            }
        }

        // app type harbour
        // X-Nemo-Application-Type=silica-qt5
        //  Exec=harbour-file-browser %f
        if(data.contains("x-nemo-application-type")) {
            // harbour app
            QStringList l = data.value("exec").split(' '); // il peut y avoir des arguments
            d->setId(l.at(0));
            d->setType("SAILFISH");
            return d;
        }

        d->setType("SYSTEM");
        return d;
    }

    return NULL;
}

/**
 * @brief DesktopEntryService::foundIconFor
 * @param name
 * @param size
 * @return
 */
QString DesktopEntryService::iconFor(QString const &name) {
    // special case for sailfish icon
//    static QString sailfishIcoPath = []() -> QString {

//            QDir d("/usr/share/themes/sailfish-default/meegotouch");
//            if(d.exists() == false ) {
//            return "";
//}

//            d.setFilter(QDir::NoDotAndDotDot|QDir::Dirs);
//            d.setSorting(QDir::Name|QDir::IgnoreCase|QDir::Reversed);

//            QStringList list = d.entryList();
//            if(list.length()==0) {
//            return "";
//}

//            return "/usr/share/themes/sailfish-default/meegotouch/" + list.at(0) + "/icons/";
//}();

    // verif dans le cache
    if( iconeCache.contains(name)) {
        return iconeCache.value(name);
    }

    // on recherche un icone d'app (harbour / android)
    QDir dut("/usr/share/icons/hicolor/");
    dut.setFilter(QDir::NoDotAndDotDot|QDir::Dirs);

    QString path = "";
    qint64 size = 0;

    QFileInfoList dutl = dut.entryInfoList();
    for ( const auto& parent : dutl  ) {

        QFileInfo info( parent.absoluteFilePath() + "/apps/" + name + ".png");
        if(info.exists() == false) {
            continue;
        }

        if(info.size() <= size) {
            continue;
        }

        size = info.size();
        path = info.absoluteFilePath();
    }

    if( path.length()>0) {
    //    qDebug() << "hicolor : " << name << " - " << path;
        return path;
    }

    // trouve pas dans les icones utilisateur, on cherche dans le theme
    // les themes sont nommées z1.0 z1.25 etc... la date de creation n'est pas valable
    // parcourrir par ordre inverse est la sol la plus à me de retourner la dernière version

    QDir dth("/usr/share/themes/sailfish-default/meegotouch");
    dth.setFilter(QDir::NoDotAndDotDot|QDir::Dirs);
    dth.setSorting(QDir::Name|QDir::IgnoreCase|QDir::Reversed);

    // ! si on est là, path est size n'ont pas été modifés

    QFileInfoList dthl = dth.entryInfoList();
    // on parcours les rep à la recherche de la première occurence
    for ( const auto& parent : dthl  ) {

        QFileInfo info( parent.absoluteFilePath() + "/icons/" + name + ".png");
        if(info.exists() == true) {
      //      qDebug() << "theme : " << name << " - " << info.absoluteFilePath();
            return info.absoluteFilePath();
        }
    }

    return "res";
}

