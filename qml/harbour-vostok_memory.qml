import QtQuick 2.0
import Sailfish.Silica 1.0
//import "pages"

ApplicationWindow
{
    id:app
    allowedOrientations :Orientation.Portrait
    initialPage:  Qt.resolvedUrl("pages/main.qml")
 //   initialPage:  Qt.resolvedUrl("pages/Applications.qml")
 //   initialPage:  Qt.resolvedUrl("pages/application.qml")
    cover: Qt.resolvedUrl("cover/cmain.qml")
}


