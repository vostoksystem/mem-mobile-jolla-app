import QtQuick 2.0
import Sailfish.Silica 1.0
import"../composents"

Page {
    id:page
    width: root.width

    SilicaFlickable {
        width: parent.width
        height: parent.height
        anchors.fill: parent
        contentHeight: content.height

        Column {
            id: content
            width: parent.width - Theme.paddingMedium - Theme.paddingMedium
            anchors.left: parent.left
            anchors.margins:Theme.paddingMedium
            spacing: Theme.paddingSmall

            PageHeader {
                title: vi18n.get("menu.apropos")
            }

            Row {
                spacing: Theme.paddingMedium
                height: Theme.itemSizeExtraLarge
                anchors.margins: Theme.paddingMedium
                Image {
                    width: Theme.iconSizeLauncher
                    height:Theme.iconSizeLauncher
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:///img/logo"
                }

                Column {
                    anchors.verticalCenter: parent.verticalCenter

                    Label {
                        text:vi18n.get("apropos.titre")
                        font.pixelSize: Theme.fontSizeHuge
                        font.bold: true
                    }

                    Label {
                        text:vi18n.get("apropos.version").arg(Qt.application.version)
                        font.pixelSize: Theme.fontSizeSmall
                    }
                }
            }

            LabelRubrique {
                text: vi18n.get("apropos.licence")
                position: true
            }

            Label {
                width:parent.width
                wrapMode: Text.WordWrap
                text: vi18n.get("apropos.licence.texte")
            }

            LabelRubrique {
                text: vi18n.get("apropos.copy")
                position: true
            }
            Label {
                width:parent.width
                wrapMode: Text.WordWrap
                text: vi18n.get("apropos.copy.texte").arg("2015-2022")
            }

            BackgroundItem {
                width: parent.width
                height: Theme.itemSizeExtraLarge

                onClicked: Qt.openUrlExternally("http://www.vostoksystem.com")

                Label {

                    width:parent.width
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: "www.vostoksystem.com"
                    font.underline: true
                    font.bold: true
                }
            }
        }
    }
}

