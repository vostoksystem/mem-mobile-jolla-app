import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id:item
    property real pourcentage:0.0
    property int  actuel
    property int total
    property string titre:vi18n.get("main.statmb")
    property real diviseur:1048576
    property bool lien :true
    property alias icone: img.source
    height: Theme.itemSizeSmall
    width: parent.width

    Image {
        id: img
        width: parent.height
        height: parent.height
    }

    ProgressBar {
        id:progress
    //    value: 0
        label: titre.arg(
                   (pourcentage *100).toFixed(2)
                   ).arg(
                   (actuel / diviseur).toFixed(2)
                   ).arg(
                   (total / diviseur).toFixed(2)
                   )

        width: parent.width - img.width
        anchors {
            left:img.right
            leftMargin: Theme.horizontalPageMargin
            right: parent.right
            rightMargin: Theme.horizontalPageMargin
            verticalCenter: parent.verticalCenter
        }

        Behavior on value {
            NumberAnimation { duration: 1500; easing.type: Easing.Linear }
        }
    }

    onPourcentageChanged:{
        progress.value=item.pourcentage;
    }
}
