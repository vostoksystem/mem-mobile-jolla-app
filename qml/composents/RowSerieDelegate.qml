import QtQuick 2.2
import Sailfish.Silica 1.0
import "../js/fichier.js" as FichierJS

Item {
    height: n.height+r.height+Theme.paddingLarge
    width: parent.width - 2*Theme.horizontalPageMargin
    x : Theme.horizontalPageMargin

    Item {
        id:n
        width: parent.width
        height: Theme.iconSizeSmall

        Label {
            text: nom
            font.bold: true
            color: Theme.highlightColor
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
        }

        Label {
            text: FichierJS.getTaille(valeur *1024)
            color: Theme.highlightColor
            anchors {
                right: parent.right
                rightMargin: Theme.horizontalPageMargin
                verticalCenter: parent.verticalCenter
            }
        }
    }

    Rectangle {
        id:r
        color: couleur
        height: Theme.paddingSmall
        width: 0
        anchors {
            topMargin: Theme.paddingSmall
            left: parent.left
            top: n.bottom
        }

        Behavior on width {
            NumberAnimation { duration: 1500; easing.type: Easing.Linear }
        }

    }

    Component.onCompleted: {
        if(PageStatus.Active) {
            r.width=(width - Theme.paddingSmall) * pourcentage + Theme.paddingSmall;
        }
    }
}
