import QtQuick 2.2
import Sailfish.Silica 1.0

import "../composents"
import "../hints"
import "../js/fichier.js" as FichierJS

Page {
    id: page
    width: root.width

    property bool active: status===PageStatus.Active

    property var _systemeData
    property var _interneData
    property var _sdcardData
    property var _memoireData

    // si root et home sur le même point de montage
    property int _shared :0 //  0 pas d'info, 1 oui, 2 nom

    property real timestamp :0

    //-----------------------------------------
    function rafraichir(mode) {
        if(mode === false) {
            if(timestamp+ 120000 > Date.now()) {
                return; // pas la peine, fait il y a moins de 2min
            }
        }

        _memoireData=MemoireService.utilisation();
        _interneData=FichierService.utilisation("/home");
        _systemeData=FichierService.utilisation("/");

        _shared = (_interneData.mount === _systemeData.mount ? 1 : 2);

        if(SdcardLocator.present) {
            _sdcardData = FichierService.utilisation(SdcardLocator.location);
        }

        timestamp= Date.now();
    }

    SilicaFlickable {
        contentWidth: parent.width
        contentHeight: parent.height
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: vi18n.get("menu.apropos")
                onClicked: pageStack.push(Qt.resolvedUrl("apropos.qml"))
            }
            MenuItem {
                text: vi18n.get("menu.config")
                onClicked: pageStack.push(Qt.resolvedUrl("config.qml"))
            }

            MenuItem {
                text: vi18n.get("menu.applications")
                onClicked: pageStack.push(Qt.resolvedUrl(
                                       serv_config.disclaimer ?
                                           "Applications.qml" :
                                           "Disclaimer.qml"
                                       ))
            }

            MenuItem {
                text: vi18n.get("menu.recharger")
                onClicked: rafraichir(true)
            }
        }

        VerticalScrollDecorator { }

        Column {
            id: column
            x:Theme.horizontalPageMargin
            width: page.width
            spacing: Theme.paddingMedium
            anchors.fill: parent
            anchors.margins: Theme.horizontalPageMargin

            PageHeader {
                title: vi18n.get("main.titre")
            }

            // system
            SectionHeader {
                text: vi18n.get("main.systeme")
            }
            BackgroundItem {
                Vuemetre {
                    visible: _systemeData && _systemeData.valide && (_shared == 2)
                    pourcentage: _systemeData ? _systemeData.pourcentage : 0.0
                    actuel: _systemeData ? _systemeData.actuel : 0
                    total: _systemeData ? _systemeData.total : 0
                    titre: vi18n.get("main.statgb")
                    icone: "image://theme/icon-m-developer-mode"
                }

                Label {
                    visible: _shared == 1
                    width: parent.width
                    text: vi18n.get("main.shared")
                    horizontalAlignment: Text.AlignRight
                    color: Theme.highlightColor
                    anchors.rightMargin: Theme.horizontalPageMargin
                    anchors.right: parent.right
                }
            }

            // interne
            SectionHeader {
                text: vi18n.get("main.interne")
            }
            BackgroundItem {
                onClicked:pageStack.push(Qt.resolvedUrl("fichier.qml"),{titre:"interne.titre",source:"/home"})
                Vuemetre {
                    visible: _interneData && _interneData.valide
                    pourcentage: _interneData ? _interneData.pourcentage : 0.0
                    actuel: _interneData ? _interneData.actuel : 0
                    total: _interneData ? _interneData.total : 0
                    titre: vi18n.get("main.statgb")
                    icone: "qrc:///img/phone"
                }
            }

            // sdcard
            SectionHeader {
                text: vi18n.get("main.sdcard").arg(SdcardLocator.type.length>0 ? "(" + SdcardLocator.type + ")" : "")
            }
            BackgroundItem {
                visible: SdcardLocator.present && _sdcardData.valide
                onClicked:pageStack.push(Qt.resolvedUrl("fichier.qml"),{titre:"sdcard.titre",source:SdcardLocator.location})
                Vuemetre {
                    pourcentage: _sdcardData ? _sdcardData.pourcentage : 0.0
                    actuel: _sdcardData ? _sdcardData.actuel : 0
                    total: _sdcardData ? _sdcardData.total : 0
                    titre: vi18n.get("main.statgb")
                    icone: "qrc:///img/sdcard"
                }
            }

            Label {
                visible: SdcardLocator.present === false
                text: vi18n.get("main.pasdesdcard")
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
            }

            //  memoire
            SectionHeader {
                text: vi18n.get("main.memoire")
            }

            BackgroundItem {
                onClicked:pageStack.push(Qt.resolvedUrl("processus.qml"))
                Vuemetre {
                    visible: _memoireData && _memoireData.valide
                    pourcentage: _memoireData ? _memoireData.pourcentage : 0.0
                    actuel: _memoireData ? _memoireData.actuel : 0
                    total: _memoireData ? _memoireData.total : 0
                    diviseur: 1024
                    titre: vi18n.get("main.statmb")
                    icone: "qrc:///img/ram"
                }
            }
        }
    }

    HintPress {
        id:hin1
        clee : "/vostok/memory/hint/menu"
        texte: vi18n.get("hint.menu")
        onFin: hin2.lancer()
    }

    Hint {
        id:hin2
        clee : "/vostok/memory/hint/applibackup"
        texte: vi18n.get("hint.applist")
        direction:TouchInteraction.Down
        interactionMode:TouchInteraction.Swipe
    }

    onActiveChanged: {
        if(active) {
            rafraichir(false);
        }
    }

    Component.onCompleted:  {
        hin1.lancer();
    }
}


