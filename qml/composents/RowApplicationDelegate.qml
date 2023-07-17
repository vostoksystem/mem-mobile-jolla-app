import QtQuick 2.0
import Sailfish.Silica 1.0
import "../js/fichier.js" as FichierJS

ListItem {
    id:item
    width: parent.width
    contentHeight:Theme.itemSizeLarge

    property int _total:0
    property int _application:0
    property int _config:0
    property int _donnee:0
    property int _cache:0


    Image {
        id:ico
        // garde la même tailel que pour les processus
        height: parent.height - (Theme.fontSizeSmall * 1.2)
        width: height
        x:Theme.horizontalPageMargin
        source: icon
        anchors {
            verticalCenter: parent.verticalCenter
        }
    }

    Item {
        height: parent.height
        width: parent.width - ico.width - Theme.horizontalPageMargin
        x:Theme.horizontalPageMargin

        anchors {
            left: ico.right
        }

        Text {
            id:titre
            height: Theme.fontSizeMedium
            width: parent.width
           // font.pixelSize: Theme.fontSizeSmall
            font.bold: true
            text: name
            clip: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            anchors {
                right: parent.right
                rightMargin: Theme.horizontalPageMargin
            }

            color: item.highlighted ? Theme.highlightColor : Theme.primaryColor
        }

        Text {
            id:total_text
            height: (parent.height - titre.height)/2
            width: parent.width
            visible: _total !== 0
            font.pixelSize: Theme.fontSizeExtraSmall
            font.bold: true
            text: vi18n.get("applications.total").arg(FichierJS.getTaille(_total))
            clip: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            anchors {
                right: parent.right
                top:titre.bottom
                rightMargin: Theme.horizontalPageMargin
            }

            color: item.highlighted ? Theme.highlightColor : Theme.secondaryColor
        }

        Text {
            height: (parent.height - titre.height)/2
            width: parent.width
            visible: _total !== 0
            font.pixelSize: Theme.fontSizeExtraSmall
            font.bold: true
            text: vi18n.get("applications.stat").arg(
                      _application < 1024 ? 0 : FichierJS.getTaille(_application)).arg(
                      _donnee + _config < 1024 ? 0 : FichierJS.getTaille(_donnee+_config)).arg(
                      _cache < 1024 ? 0 : FichierJS.getTaille(_cache))
            clip: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            anchors {
                right: parent.right
                top:total_text.bottom
                rightMargin: Theme.horizontalPageMargin
            }

            color: item.highlighted ? Theme.highlightColor : Theme.secondaryColor
        }
    }

    onClicked:pageStack.push(Qt.resolvedUrl("../pages/application.qml"), {app_id:did})

    Component.onCompleted: {
        var taille  = CalculApplication.getTaille(did);
        if(taille === undefined) {
            return;
        }

        _application = taille.application;
        _config = taille.config;
        _donnee = taille.donnee;
        _cache = taille.cache;
        _total = _application + _donnee + _cache;

    }

    Connections {
        target: CalculApplication

        onCalculDisponible : {

            if( taille === undefined) {
                return;
            }

            if( did !== id_desktop ) {
                return;
            }

            _application = taille.application;
            _config = taille.config;
            _donnee = taille.donnee;
            _cache = taille.cache;
            _total = _application + _config + _donnee + _cache;
        }
    }
}
