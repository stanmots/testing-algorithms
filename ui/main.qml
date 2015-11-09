import QtQuick 1.1

import "qrc:///ui/Components" as GUI

Rectangle {

    id: mainWindow

    implicitHeight: 500
    implicitWidth: 500
    visible: true
    color: "lightgray"

    //main window components
    Item {
        id: mainWindowsList

        anchors.fill: parent

        GUI.MainTabView {
            id: mainTabView
            anchors.fill: parent
        }

        GUI.AdjacencyMatrixWindow {
            id: adjMatrixWindow
            anchors {
                top: parent.top
                topMargin: 20
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
        }
    }

    //translations
    property string progressDialogLabelText: qsTr("Progress status of the current operation...")

    Component.onCompleted: {
        mainProgressDialog.setLabelText(progressDialogLabelText)
    }

    function hideAllQmlWindows() {
        for (var i = 0; i < mainWindowsList.children.length; ++i) {
            mainWindowsList.children[i].visible = false
        }
    }
}
