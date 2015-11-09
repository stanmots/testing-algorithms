import QtQuick 1.1

Rectangle {

    property alias  adjacencyMatrixViewRef: adjacencyMatrixView
    visible: false

    signal saveAdjMatrixSignal()
    signal closeAdjMatrixSignal()

    width: 600
    height: 500

    AdjacencyMatrixToolBar {
        id: adjacencyMatrixToolBar
    }

    //Matrix
    AdjacencyMatrixView {
        id: adjacencyMatrixView

        anchors {
            top: adjacencyMatrixToolBar.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
    }
}
