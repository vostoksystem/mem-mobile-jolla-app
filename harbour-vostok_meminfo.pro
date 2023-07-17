TARGET = harbour-vostok_meminfo

CONFIG += sailfishapp
QT += dbus
LIBS += -lnemonotifications-qt5

DEFINES += APP_VERSION=\"\\\"$${VERSION}\\\"\"
DEFINES += APP_COPYRIGHT=\"\\\"$${COPYRIGHT}\\\"\"
DEFINES += APP_NAME=\"\\\"$${TARGET}\\\"\"

include(src/vi18n/vi18n.pri)
include(src/sdcardlocator/sdcardlocator.pri)
include(src/iconprovider/iconprovider.pri)
include(src/desktop/desktop.pri)
include(src/process/process.pri)

HEADERS += \
    src/bean/Serie.h \
    src/services/memoire.h \
    src/bean/Etat.h \
    src/app.h \
    src/services/config.h \
    src/bean/MimeType.h \
    src/services/fichier.h \
    src/services/scannerfichier.h \
    src/services/calculapplication.h \
    src/bean/tailleapplication.h \
    src/services/scannercalculapplication.h \
    src/bean/redirectionapplication.h \
    src/services/backupservice.h

SOURCES += src/harbour-vostok_meminfo.cpp \
    src/bean/Serie.cpp \
    src/services/memoire.cpp \
    src/bean/Etat.cpp \
    src/app.cpp \
    src/services/config.cpp \
    src/bean/MimeType.cpp \
    src/services/fichier.cpp \
    src/services/scannerfichier.cpp \
    src/services/calculapplication.cpp \
    src/bean/tailleapplication.cpp \
    src/services/scannercalculapplication.cpp \
    src/bean/redirectionapplication.cpp \
    src/services/backupservice.cpp

OTHER_FILES += \
    qml/pages/*.qml \
    qml/cover/*.qml \
    qml/hints/*.qml \
    qml/composents/*.qml \
    qml/js/*.js \
    rpm/harbour-vostok_meminfo.spec \
    rpm/harbour-vostok_meminfo.yaml \
    rpm/harbour-vostok_meminfo.changes \
    harbour-vostok_meminfo.desktop \
    info.txt \
    qml/harbour-vostok_memory.qml

RESOURCES += harbour-vostok_meminfo.qrc

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172 256x256

DISTFILES += \
    harbour-vostok_meminfo.qrc \
    qml/pages/fichier.qml \
    qml/pages/Applications.qml \
    qml/composents/RowApplicationDelegate.qml \
    qml/pages/application.qml \
    qml/composents/Spacer.qml \
    qml/composents/DialogRestaure.qml \
    qml/pages/Disclaimer.qml
