import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    property alias couleur: colorIndicator.color
    property alias valeur: v.text
    property alias titre: t.text
    property alias etat: t.checked
    property alias type:t.enabled

    width: parent.width
    height: t.height + (t.checked ? v.height : 0)
    id:r

    TextSwitch {
        id:t
    }

    BackgroundItem {
        width: t.height
        height: t.height /2
        anchors.right:r.right
        anchors.verticalCenter: t.verticalCenter

        Rectangle {
            id: colorIndicator
            radius:Theme.paddingSmall
            height: parent.height
            width: parent.width
            visible: t.checked || r.type===0
        }
        onClicked: {
            var dialog = pageStack.push("Sailfish.Silica.ColorPickerDialog")
            dialog.accepted.connect(function() {
                colorIndicator.color = dialog.color
            })
        }
    }

    TextField {
        width: parent.width
        anchors.top:t.bottom
        label:  vi18n.get("options.tip")
        placeholderText: vi18n.get("options.pasvide")
        id:v
        visible:t.checked
    }
}
