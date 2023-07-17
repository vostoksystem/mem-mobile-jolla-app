import QtQuick 2.0
import Sailfish.Silica 1.0
import"../composents"

Page {
    id:page
    width: root.width

    function rechargerDefault() {
        serv_config.parDefaut();
    }

    SilicaFlickable {
        width: parent.width
        height: parent.height
        contentHeight: c.height
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: vi18n.get("options.default")
                onClicked: rechargerDefault()
            }
        }

        VerticalScrollDecorator {}

        Column {
            width: parent.width - (Theme.horizontalPageMargin*2)
            anchors.left: parent.left
            anchors.margins:Theme.horizontalPageMargin
            spacing: Theme.paddingSmall
            id:c

            PageHeader {
                title: vi18n.get("options.titre")
                id:ph
            }

            LabelRubrique {
                text: vi18n.get("options.stat")
                position: true
            }

            TextSwitch {
                id:android
                text: vi18n.get("options.stat.android")
                description:  vi18n.get("options.stat.android.info")

                Component.onCompleted: checked = serv_config.androidScan
                onCheckedChanged: {
                    serv_config.androidScan=checked;
                }

                Binding {
                    target:  android
                    property: "checked"
                    value: serv_config.androidScan
                }
            }

            LabelRubrique {
                text: vi18n.get("options.media")
                position: true
            }
            Repeater {
                model: serv_config.listeMime
                delegate:  MimeTypeDelegate{ }
            }

            LabelRubrique {
                text: vi18n.get("options.memoire")
                position: true
            }
            TextSwitch {
                id:processus
                text: vi18n.get("options.memoire.tous")
                description:  vi18n.get("options.memoire.tousdesc")
                Component.onCompleted: checked = serv_config.processusSysteme
                onCheckedChanged: {
                    serv_config.processusSysteme=checked;
                }

                Binding {
                    target: processus
                    property: "checked"
                    value: serv_config.processusSysteme
                }
            }
            Slider {
                id:taille
                label: vi18n.get("options.memoire.palier")
                minimumValue: 0
                maximumValue: 50
                stepSize: 10
                valueText: sliderValue + 'Mb'
                width: parent.width
                Component.onCompleted: value=serv_config.tailleMinProcessus

                onValueChanged: {
                    if(serv_config.tailleMinProcessus===value) {
                        return;
                    }
                    serv_config.tailleMinProcessus=value;
                }

                Binding {
                    target: taille
                    property: "value"
                    value: serv_config.tailleMinProcessus
                }
            }
        }
    }
}
