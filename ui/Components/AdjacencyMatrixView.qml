import QtQuick 1.1
import QtDesktop 0.1

ScrollArea {

    property int k_ADJACENCY_MATRIX_CELL_SIZE: 25
    property alias matrixViewRef: matrixView

    property int adjacencyMatrixSize: adjacencyMatrixToolBar.adjMatrixSizeSpinBoxRef.value
    property bool isLoadingAdjMatrix: false

    property int m_progress: 0

    onAdjacencyMatrixSizeChanged: {

        //start operation
        m_progress = 0
        matrixScrollArea.visible = false
        isLoadingAdjMatrix = true

        mainProgressDialog.setMaximum(Math.abs(adjacencyMatrixSize * adjacencyMatrixSize -
                                               matrixView.rows * matrixView.columns))
        if(mainProgressDialog.maximum > 40) {
            mainProgressDialog.show()
        } else {
            mainProgressDialog.hide()
        }

        matrixView.rows = adjacencyMatrixSize
        matrixView.columns = adjacencyMatrixSize

        isLoadingAdjMatrix = false
        matrixScrollArea.visible = true
    }

    Item {
        id: matrixScrollArea

        width: childrenRect.width
        height: childrenRect.height

        enabled: !isLoadingAdjMatrix

        Connections {
            id: progressConnection

            target: matrixView
            onProgressChanged: {
                if(mainProgressDialog.wasCanceled === false) {
                    m_progress +=1
                    mainProgressDialog.setValue(m_progress)

                    if(m_progress >= mainProgressDialog.maximum) {
                        mainProgressDialog.hide()
                    }
                }
            }
        }

        Row {
            id: adjMatrixTitlesRow

            enabled: false
            spacing: 0
            Rectangle {
                id: baseElement

                enabled: false
                width: k_ADJACENCY_MATRIX_CELL_SIZE
                height: k_ADJACENCY_MATRIX_CELL_SIZE
                color: "#444444"
                border { width: 2.0; color: "#333333"; }
            }

            Repeater {
                model: adjacencyMatrixSize
                Rectangle {
                    enabled: false
                    width: k_ADJACENCY_MATRIX_CELL_SIZE
                    height: k_ADJACENCY_MATRIX_CELL_SIZE
                    color: "#888888"
                    border { width: 1.0; color: "#333333"; }

                    Text {
                        id: adjMatrixRowNumber

                        enabled: false
                        anchors.centerIn: parent
                        text: index+1
                    }
                }
            }
        }

        Column {
            id: adjMatrixTitlesColumn

            enabled: false
            anchors.top: adjMatrixTitlesRow.bottom
            anchors.left: adjMatrixTitlesRow.left
            spacing: 0

            Repeater {
                model: adjacencyMatrixSize
                Rectangle {

                    enabled: false
                    width: k_ADJACENCY_MATRIX_CELL_SIZE
                    height: k_ADJACENCY_MATRIX_CELL_SIZE
                    color: "#888888"
                    border { width: 1.0; color: "#333333"; }
                    Text {
                        id: adjMatrixColumnNumber

                        enabled: false
                        anchors.centerIn: parent
                        text: index+1
                    }
                }
            }
        }

        MatrixView {
            id: matrixView

            anchors.top: adjMatrixTitlesRow.bottom
            anchors.left: adjMatrixTitlesColumn.right

            rows: 1
            columns: 1

            cellWidth: k_ADJACENCY_MATRIX_CELL_SIZE
            cellHeight: k_ADJACENCY_MATRIX_CELL_SIZE

            diagonalCellsEnabled: false
            isSymmetricMatrix: true
            symmetricAnimationEnabled: true
        }
    }
}
