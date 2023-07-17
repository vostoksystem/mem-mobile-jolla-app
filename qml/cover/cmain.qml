import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {

    property bool active: status === Cover.Active
    property string affichage: "interne"
    property var _donnee
    property string _icone

    //---------------------------------------
    function charger_donnee() {
        switch(affichage) {
            case "systeme" :
                _donnee=FichierService.utilisation("/");
                _icone="image://theme/icon-m-developer-mode";
                break;
            case "interne" :
                _donnee=FichierService.utilisation("/home");
                _icone="image://mem/img/phone";
                break;
            case "sdcard" :
                _donnee=FichierService.utilisation(SdcardLocator.location);
                _icone="image://mem/img/sdcard";
                break;
            case "memoire" :
               _donnee =MemoireService.utilisation();
                _icone="image://mem/img/ram";
                break;
        }
    }

    anchors.fill: parent

    //
    Column {
        anchors{
            fill: parent
            margins: Theme.paddingLarge
        }

        Label {
            text: _donnee ? (_donnee.pourcentage*100).toFixed(0) + "%" : ""
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeHuge
            anchors {
                horizontalCenter: parent.horizontalCenter
            }
        }

        ProgressCircle {
            value: _donnee ? _donnee.pourcentage : 0
            progressColor: Theme.highlightColor
            backgroundColor: Theme.highlightDimmerColor

           // inAlternateCycle: true
            width:parent.width *0.9
            height:parent.width *0.9
            anchors {
                horizontalCenter: parent.horizontalCenter
            }

            Image {
                source: _icone
                anchors{
                    centerIn: parent
                }
            }
        }
    }

    CoverActionList {
        CoverAction {
            iconSource: "image://theme/icon-cover-previous-song"
            onTriggered: {
                switch(affichage) {
                    case "systeme" :
                        affichage = "memoire"
                        break;
                    case "interne" :
                        affichage = "systeme";
                        break;
                    case "sdcard" :
                        affichage="interne";
                        break;
                    case "memoire" :
                        affichage= SdcardLocator.present ? "sdcard" : "interne";
                        break;
                }
            }
        }

        CoverAction {
            iconSource: "image://theme/icon-cover-next-song"
            onTriggered: {
                switch(affichage) {
                    case "systeme" :
                        affichage = "interne"
                        break;
                    case "interne" :
                        affichage= SdcardLocator.present ? "sdcard" : "memoire";
                        break;
                    case "sdcard" :
                        affichage = "memoire";

                        break;
                    case "memoire" :
                        affichage="systeme";
                        break;
                }
            }
        }
    }

    onAffichageChanged: {
        charger_donnee();
    }

    onActiveChanged: {
        if(active) {
            charger_donnee();
        }
    }

    Connections {
        target: app
        onApplicationActiveChanged: {
            if(app.applicationActive) {
                charger_donnee();
            }
        }
    }
}


