import QtQuick 1.1
import QtDesktop 0.1

Button {
    property alias customText: buttonText.text
    width: buttonText.width + 10.0
    Text {
        id: buttonText
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }
        color: parent.enabled ? "black" : "gray"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
