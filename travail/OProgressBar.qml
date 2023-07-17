import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Silica.private 1.0

Item {
    id: progressBar

    property real maximumValue: 1.0
    property real minimumValue: 0.0
    property real value: 0.0
    property real progressValue: Math.max(minimumValue, Math.min(maximumValue, value))
    property bool indeterminate
    property string valueText
    property alias label: labelText.text
    property real leftMargin: Math.round(Screen.width/8)
    property real rightMargin: Math.round(Screen.width/8)

    property bool _hasValueLabel: false
    property alias _timer: progressBarTimer
    property real _grooveWidth: Math.max(0, width - leftMargin - rightMargin)

    height: valueText.length ? Theme.itemSizeExtraLarge : Theme.itemSizeSmall

    onValueTextChanged: {
        if (valueText && !_hasValueLabel) {
            _hasValueLabel = true
            valueIndicatorComponent.createObject(progressBar)
        }
    }

    GlassItem {
        id: background
        // extra painting margins (Theme.paddingMedium on both sides) are needed,
        // because glass item doesn't visibly paint across the full width of the item
        x: progressBar.leftMargin-Theme.paddingMedium
        width: progressBar._grooveWidth + 2*Theme.paddingMedium
        height: Theme.paddingLarge
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Theme.fontSizeSmall
        dimmed: true
        radius: 0.06
        falloffRadius: 0.09
        ratio: 0.0
        color: Theme.secondaryColor
    }

    GlassItem {
        id: highlight
        x: background.x // some margin at each end
        anchors.verticalCenter: background.verticalCenter
        // height added as GlassItem will not display correctly with width < height
        width: indeterminate ? background.width : (progressValue - minimumValue) / (maximumValue - minimumValue) * (background.width-height) + height
        height: Theme.paddingLarge
        visible: indeterminate || value > minimumValue
        dimmed: false
        radius: 0.05
        falloffRadius: 0.14
        dashMargin: Theme.paddingLarge*2
        dashLength: Theme.paddingLarge
        ratio: 0.0
        color: Theme.primaryColor
        dashed: indeterminate

        Timer {
            id: progressBarTimer

            property bool isVisible: progressBar.visible && progressBar.opacity > 0.0

            running: indeterminate && isVisible && Qt.application.active
            repeat: true
            interval: 16
            onTriggered: {
                highlight.dashOffset += 1
                if (running) {
                    // try to hit every 3rd frame
                    restart()
                }
            }
        }
    }

    Text {
        id: labelText
        visible: text.length
        font.pixelSize: Theme.fontSizeSmall
        font.family: Theme.fontFamily
        color: Theme.secondaryColor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: background.verticalCenter
        anchors.topMargin: Theme.paddingSmall + 2
    }

    Component {
        id: valueIndicatorComponent
        Text {
            color: Theme.primaryColor
            x: Math.min(Math.max(Theme.paddingMedium, highlight.x + highlight.width-width/2), parent.width - width - Theme.paddingMedium)
            anchors.bottom: background.verticalCenter
            anchors.bottomMargin: Theme.paddingMedium
            font.pixelSize: Theme.fontSizeLarge
            font.family: Theme.fontFamily
            text: progressBar.valueText
            visible: text !== ""
        }
    }
}
