import QtQuick 1.1
import QtDesktop 0.1

Rectangle {
    id: leftPanelRect

    clip: true
    color: "#CCCCCC"

    Connections {
        target: algorithmTestPanel
        onNeedUpdateStorageWithAlgorithmOutput: {

            graphsListVisualModel.model.setProperty(graphsListView.currentIndex, "algorithm" + algType , calculatedResults)
            graphsListVisualModel.model.setProperty(graphsListView.currentIndex, "exactnessOfResult" + algType, algorithmTestPanel.isSolutionExact)
            graphsListVisualModel.model.setProperty(graphsListView.currentIndex, "exactResult" + algType, algorithmTestPanel.exactVertexCoverSize)
            graphsListVisualModel.model.setProperty(graphsListView.currentIndex, "shouldDisplay" + algType, algorithmTestPanel.shouldDisplayAlgResults)

            //need this due to a Qt's bug :(
            var updateModel = graphsListVisualModel.model.get(graphsListView.currentIndex)["shouldDisplay" + algType]
        }

        onNeedUpdateTestPanelWithAlgorithmType: {
            udpateTestPanelText()
        }
    }

    function udpateTestPanelText() {

        algorithmTestPanel.algorithmDetailsAreaRef.text = ""
        algorithmTestPanel.solveAlgorithmButtonRef.enabled = true
        algorithmTestPanel.shouldDisplayAlgResults = false

        var algUniqueKey = algorithmTestPanel.algComboBoxRef.selectedIndex

        if(graphsListVisualModel.model.get(graphsListView.currentIndex)["algorithm" + algUniqueKey] !== undefined
                && graphsListVisualModel.model.get(graphsListView.currentIndex)["algorithm" + algUniqueKey] !== '') {

            algorithmTestPanel.algorithmDetailsAreaRef.text = graphsListVisualModel.model.get(graphsListView.currentIndex)["algorithm" + algUniqueKey]
            algorithmTestPanel.solveAlgorithmButtonRef.enabled = false
            algorithmTestPanel.shouldDisplayAlgResults = graphsListVisualModel.model.get(graphsListView.currentIndex)["shouldDisplay" + algUniqueKey]

            if(graphsListVisualModel.model.get(graphsListView.currentIndex)["exactnessOfResult" + algUniqueKey] !== undefined) {
                algorithmTestPanel.isSolutionExact = graphsListVisualModel.model.get(graphsListView.currentIndex)["exactnessOfResult" + algUniqueKey]
            }

            if(graphsListVisualModel.model.get(graphsListView.currentIndex)["exactResult" + algUniqueKey] !== undefined) {
                algorithmTestPanel.exactVertexCoverSize = graphsListVisualModel.model.get(graphsListView.currentIndex)["exactResult" + algUniqueKey]
            }
        }
    }

    GraphsListVisualModel {
        id: graphsListVisualModel
    }

    Rectangle {
        id: leftBorderRect
        anchors.fill: parent
        color: "transparent"
        z: 0.5
        border { width: 2.0; color: "#333333"; }
    }

    CreateGraphTabView {
        id: createGraphTabView
        z: 0.4
    }
    Rectangle {
        id: graphsListViewHolder

        height: 25
        anchors {
            top: createGraphTabView.bottom
            topMargin: 10
            left: parent.left
            right: parent.right
        }
        color: "gray"

        Rectangle {
            height: 2.0
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            color: "#222222"
        }

        ListView {
            id: graphsListView

            height: parent.height
            highlightFollowsCurrentItem: true
            anchors {
                left: parent.left
                right: parent.right
            }
            spacing: 5
            snapMode: ListView.SnapToItem
            orientation: ListView.Horizontal
            model: graphsListVisualModel

            onCurrentIndexChanged: {
                algorithmTestPanel.currentGraphIndex = currentIndex;
                if(currentIndex !== -1) {
                    graphDetailsArea.text = graphsListVisualModel.model.get(currentIndex).graphProperties                    

                    udpateTestPanelText()

                } else {
                    graphDetailsArea.text = ""
                    algorithmTestPanel.algorithmDetailsAreaRef.text = ""
                    algorithmTestPanel.solveAlgorithmButtonRef.enabled = false
                    algorithmTestPanel.shouldDisplayAlgResults = false
                }
            }
        }
    }

    TextArea {
        id: graphDetailsArea
        clip: false

        readOnly: true
        wrapMode: TextEdit.NoWrap
        anchors {
            top: graphsListViewHolder.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
    }
}
