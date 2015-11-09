import QtQuick 1.1
import QtDesktop 0.1

Rectangle {
    id: leftPanelRect

    clip: true
    color: "#CCCCCC"

    Connections {
        target: algorithmTestPanel
        onNeedUpdateStorageWithAlgorithmOutput: {

            var algID = getAlgID(algType)

            graphsListVisualModel.model.setProperty(graphsListView.currentIndex, "algorithm" + algID , calculatedResults)
            graphsListVisualModel.model.setProperty(graphsListView.currentIndex, "exactnessOfResult" + algID, algorithmTestPanel.isSolutionExact)
            graphsListVisualModel.model.setProperty(graphsListView.currentIndex, "exactResult" + algID , algorithmTestPanel.exactVertexCount)
            graphsListVisualModel.model.setProperty(graphsListView.currentIndex, "shouldDisplay" + algID, algorithmTestPanel.shouldDisplayAlgResults)

            //need this due to a Qt's bug :(
            var updateModel = graphsListVisualModel.model.get(graphsListView.currentIndex)["shouldDisplay" + algID]
        }

        onNeedUpdateTestPanelWithAlgorithmType: {
            udpateTestPanelText()
        }
    }

    function getAlgID(algType) {
        var algID

        switch(algType) {
        case 0:
            algID = "ShortestPath"
            break
        case 1:
            algID = "GuaranteedPredictionTypeV20"
            break
        case 2:
            algID = "OptType"
            break
        case 3:
            algID = "CTreeType"
            break
        case 4:
            algID = "GuaranteedPredictionType"
            break
        case 5:
            algID = "StdType"
            break
        }

        return algID
    }

    function udpateTestPanelText() {

        algorithmTestPanel.algorithmDetailsAreaRef.text = ""
        algorithmTestPanel.solveAlgorithmButtonRef.enabled = true
        algorithmTestPanel.shouldDisplayAlgResults = false

        var algID = getAlgID(algorithmTestPanel.algComboBoxRef.selectedIndex)

        if(graphsListVisualModel.model.get(graphsListView.currentIndex)["algorithm" + algID] !== undefined
                && graphsListVisualModel.model.get(graphsListView.currentIndex)["algorithm" + algID] !== '') {

            algorithmTestPanel.algorithmDetailsAreaRef.text = graphsListVisualModel.model.get(graphsListView.currentIndex)["algorithm" + algID]
            algorithmTestPanel.solveAlgorithmButtonRef.enabled = false
            algorithmTestPanel.shouldDisplayAlgResults = graphsListVisualModel.model.get(graphsListView.currentIndex)["shouldDisplay" + algID]

            if(graphsListVisualModel.model.get(graphsListView.currentIndex)["exactnessOfResult" + algID] !== undefined) {
                algorithmTestPanel.isSolutionExact = graphsListVisualModel.model.get(graphsListView.currentIndex)["exactnessOfResult" + algID]
            }

            if(graphsListVisualModel.model.get(graphsListView.currentIndex)["exactResult" + algID] !== undefined) {
                algorithmTestPanel.exactVertexCount = graphsListVisualModel.model.get(graphsListView.currentIndex)["exactResult" + algID]
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
