import QtQuick 2.0
import Sailfish.Silica 1.0
import "../js/couleur.js" as CouleurJS

Item {
    width: parent.width
    height: t.height + (v.visible ? v.height : 0)
    id: delegate

     TextSwitch {
        id:t
        text:vi18n.get("media." + nom)

        Component.onCompleted: checked = actif
        onCheckedChanged: {
            if(!extensionPossible) {
                checked = true;
            }
            actif= checked;
        }

        Binding {
            target: t
            property: "checked"
            value: actif
        }
    }

    BackgroundItem {
        width: t.height
        height: width /2
        anchors.right:delegate.right
        anchors.rightMargin: Theme.paddingMedium
        anchors.verticalCenter: t.verticalCenter

        Rectangle {
            id: colorIndicator
            color: couleur
            radius:Theme.paddingSmall
            height: parent.height
            width: parent.width
            visible: t.checked || !extensionPossible
        }
        onClicked: {
            var dialog = pageStack.push("Sailfish.Silica.ColorPickerDialog");
            dialog.colors=CouleurJS.palette();
            dialog.accepted.connect(function() {
                colorIndicator.color = dialog.color;
                couleur=dialog.color;
            })
        }
    }

    TextField {
        id:v
        width: parent.width
        anchors.top:t.bottom
        label:  vi18n.get("options.tip")
        placeholderText: vi18n.get("options.pasvide")
        text:extension
        visible:t.checked && extensionPossible

        validator: RegExpValidator { regExp: /^[a-z0-9\-_\. ]{1,}$/ }
        inputMethodHints: Qt.ImhNoPredictiveText

        EnterKey.enabled: extension.length > 0
        EnterKey.iconSource: "image://theme/icon-m-enter-accept"
        EnterKey.onClicked: {
            extension=v.text;
            v.focus=false;
        }
    }
}
