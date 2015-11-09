import QtQuick 1.1
import QtDesktop 0.1

Rectangle {
    id: splitter

    color: splitterDragArea.containsMouse === true ? "#00FFCC" : "#0099FF"
    width: 10

    property int previousXPos: 0

    anchors {
        top: parent.top
        bottom: parent.bottom
        horizontalCenter: splitterDragArea.drag.active === false ? parent.horizontalCenter : undefined
    }

    //cosmetics
    Rectangle {
        id: bottomLine
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        height: 2.0
        color: "#333333"
    }

    Rectangle {
        id: topLine
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        height: 2.0
        color: "#333333"
    }

    Behavior on color {
        ColorAnimation { duration: 250; }
    }

    MouseArea {
        id: splitterDragArea

        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true
        drag.axis: Drag.XAxis
        drag.maximumX: singleGraphLayout.width - splitter.width
        drag.maximumY: singleGraphLayout.height - splitter.height
        drag.minimumX: singleGraphLayout.x
        drag.minimumY: singleGraphLayout.y
        drag.target: splitter

        onClicked: {
            if(mouse.button === Qt.RightButton)
                splitterMenu.showPopup(splitter.x, mouse.y)
        }

        drag.onActiveChanged: {
            if (drag.active === true) {
                //drag started
                splitter.previousXPos = splitter.x;
            } else {
                //drag finished
               splitter.anchors.horizontalCenterOffset += splitter.x - splitter.previousXPos;
            }
        }
    }

    ContextMenu {
        id: splitterMenu

        MenuItem {
            text: "50/50"
            onTriggered: splitter.anchors.horizontalCenterOffset = 0

        }
    }
}
