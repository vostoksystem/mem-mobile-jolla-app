import QtQuick 2.0
import Sailfish.Silica 1.0
import "../js/fichier.js" as FichierJS

ListItem {   
    id:item
    width: parent.width
    contentHeight:Theme.itemSizeLarge

    opacity: opacite

    property real pourcentage : 0.0

    ListView.onRemove: animateRemoval(item)

    menu: ContextMenu {
        MenuItem {
            enabled: utilisateur === "nemo" && tuer === false
            text: vi18n.get(
                      type === "ANDROID" ?
                          "memoire.arreter.android" :  "memoire.arreter"
                      )
            onClicked: item.ListView.view.tuerProcess(name, pid)
        }

        onActiveChanged: item.ListView.view.actionMenu(active)
    }

    //-----------------------------------------
    function lancementmenu() {
        ListView.view.azerty();
    }

    Text {
        id:titre
        height: Theme.fontSizeSmall * 1.2
        width: parent.width
        font.pixelSize: Theme.fontSizeSmall
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

    Item {
        height: parent.height - titre.height
        width: parent.width - Theme.horizontalPageMargin
        x:Theme.horizontalPageMargin

        anchors {
            left: parent.left
            top:titre.bottom
        }

        Image {
            id:ico
            height: parent.height
            width: height
            x:Theme.horizontalPageMargin
            source: tuer ? "image://theme/icon-m-timer" : icon
            anchors {
                verticalCenter: parent.verticalCenter
            }
        }

        Text {
            id:valeur
            width: Theme.iconSizeLarge
            height: parent.height
            font.pixelSize: Theme.fontSizeMedium
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: vi18n.get("memoire.pourcentage").arg((pourcentage * 100.0).toFixed(1))
            anchors {
                left: ico.right
                verticalCenter: parent.verticalCenter
            }
            color: parent.highlighted ? Theme.highlightColor : Theme.primaryColor
        }

        Item {
            id:gmem
            width: parent.width - valeur.width - ico.width - Theme.horizontalPageMargin
            height: parent.height / 2
            anchors {
                left: valeur.right
                top:parent.top
            }

            property int tailleJauge: width - height - Theme.itemSizeMedium - (Theme.horizontalPageMargin * 2)

            Rectangle {
                width: gmem.tailleJauge  * fullMemoryPercent
                height:parent.height
                color: Theme.highlightColor

                Rectangle {
                    width: gmem.tailleJauge  * memoryPercent
                    height:parent.height
                    color: Theme.highlightBackgroundColor
                }
            }

            Image {
                height: parent.height
                width: height
                x:Theme.horizontalPageMargin
                source: "image://mem/img/ram"
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: gtext.left
                }
            }


            Text {
                id : gtext
                font.pixelSize: Theme.fontSizeExtraSmall
                text:  vi18n.get("memoire.memoire").arg(FichierJS.getTaille(fullMemory > 0 ? fullMemory : memory))
                width: Theme.itemSizeMedium
                horizontalAlignment: Text.AlignRight
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                }

                color: Theme.highlightColor
            }
        }

        Item {
            width: gmem.width
            height: gmem.height
            anchors {
                left: gmem.left
                top: gmem.bottom
            }

            Rectangle {
                width: gmem.tailleJauge * cpu
                height:parent.height
                color: Theme.secondaryColor
            }

            Image {
                height: parent.height
                width: height
                x: Theme.horizontalPageMargin
                source: "image://mem/img/cpu"
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: ctext.left
                }
            }

            Text {
                id: ctext
                font.pixelSize: Theme.fontSizeExtraSmall
                text: vi18n.get("memoire.cpu").arg((cpu * 100.0).toFixed(1))
                width: Theme.itemSizeMedium
                horizontalAlignment: Text.AlignRight
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                }

                color: Theme.secondaryHighlightColor
            }
        }
    }

    onClicked: openMenu()

    Component.onCompleted: {

        if(percentType == "cpu") {
            pourcentage = cpu
            return;
        }

        // affiche memoire
        pourcentage = fullMemoryPercent === 0.0 ? memoryPercent : fullMemoryPercent
    }
}

