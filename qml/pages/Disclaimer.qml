import QtQuick 2.2
import Sailfish.Silica 1.0

Dialog {

    acceptDestination: Qt.resolvedUrl("../pages/Applications.qml")
    acceptDestinationAction: PageStackAction.Replace


    Column {
        width: parent.width

        DialogHeader {
            title: vi18n.get("disclaimer.titre")
            defaultAcceptText: vi18n.get("disclaimer.ok")
            defaultCancelText: ""

        }

        Text {
            x:Theme.horizontalPageMargin
            width: parent.width - (Theme.horizontalPageMargin *2)
            text: vi18n.get("disclaimer.texte")
            color: Theme.highlightColor
            wrapMode: Text.WordWrap
        }
    }

    Component.onCompleted: serv_config.disclaimer=true;
}
