import QtQuick 2.2
import Sailfish.Silica 1.0

Dialog {

    property string nom_app

    Column {
        width: parent.width

        DialogHeader {
            title: vi18n.get("app.restaure.titre")
        }

        Text {
            x:Theme.horizontalPageMargin
            width: parent.width
            text: vi18n.get("app.restaure.desc").arg(nom_app)
            color: Theme.highlightColor
            wrapMode: Text.WordWrap
        }
    }

}
