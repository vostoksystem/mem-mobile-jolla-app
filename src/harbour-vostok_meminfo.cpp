//#ifdef QT_QML_DEBUG
#include <QtQuick>
//#endif

#include <QCoreApplication>
#include <QMetaType>
#include <sailfishapp.h>
#include "vi18n/vi18n.h"
#include "sdcardlocator/sdcardlocator.h"
#include "iconprovider/iconprovider.h"
#include "desktop/desktop.h"
#include "process/processservice.h"
#include "bean/Etat.h"
#include "bean/Serie.h"
#include "bean/tailleapplication.h"
#include "bean/MimeType.h"
#include "services/memoire.h"
#include "services/config.h"
#include "services/fichier.h"
#include "services/calculapplication.h"
#include "services/backupservice.h"
#include "app.h"
#include "signal.h"

int main(int argc, char *argv[])
{


    qmlRegisterUncreatableType<Etat>("Etat", 1, 0, "Etat","");
    qmlRegisterUncreatableType<Serie>("Serie", 1, 0, "Serie","");
    qRegisterMetaType< SerieListe>( "SerieListe" );

    qmlRegisterUncreatableType<MimeType>("MimeType", 1, 0, "MimeType","");
    qRegisterMetaType< MimeTypeListe>( "MimeTypeListe" );

    qmlRegisterUncreatableType<TailleApplication>("TailleApplication", 1, 0, "TailleApplication","");

    // pour le qsetting
    qRegisterMetaType<MimeTypeData>("MimeTypeData");
    qRegisterMetaTypeStreamOperators<MimeTypeData>("MimeTypeData");

    qmlRegisterUncreatableType<config>("Configuration", 1, 0, "Configuration","");

    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    app->setOrganizationName(QStringLiteral("com.vostoksystem"));
    app->setApplicationName(QStringLiteral("Mem"));
    app->setApplicationVersion(QString(APP_VERSION));

    QScopedPointer<QQuickView> view(SailfishApp::createView());

    vI18n::init(view->rootContext());
    IconProvider::init(view.data(),"mem");
    SdcardLocator::init(view->rootContext());
    DesktopEntryService::init(view->rootContext());
    ProcessService::init(view->rootContext());
    CalculApplication::init(view->rootContext());
    FichierService::init(view->rootContext());
    memoireService::init(view->rootContext());
    BackupService::init(view->rootContext());
    config::init(view->rootContext());

  //  ProcessService::getInstance().loadProcessList();


   // CalculApplication::getInstance().getTaille("harbour-vostok_muuzik");

    view->setSource(SailfishApp::pathTo("qml/harbour-vostok_memory.qml"));
    view->show();

    QCoreApplication::applicationVersion();
    return app->exec();
}

