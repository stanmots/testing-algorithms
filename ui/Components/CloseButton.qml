import QtQuick 1.1

Rectangle {
    anchors {
        left: parent.left
        leftMargin: 5
        verticalCenter: parent.verticalCenter
    }
    border { width: 1.0; color: "#000033" }
    width: 16
    height: 16
    radius: 32.0
    smooth: true

    Rectangle {
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }

        smooth: true
        color: "white"
        width: parent.width/2
        height: 2.0
        rotation: 45.0
    }

    Rectangle {
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }

        smooth: true
        color: "white"
        width: parent.width/2
        height: 2.0
        rotation: -45.0
    }
}
