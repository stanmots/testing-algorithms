import QtQuick 1.1
import QtDesktop 0.1

Rectangle {
    id: rightPanel

    color: "#AAAAAA"

    property int currentGraphIndex: -1

    property alias algComboBoxRef: algComboBox
    property alias algorithmDetailsAreaRef: algorithmDetailsArea
    property alias solveAlgorithmButtonRef: solveAlgorithmButton
    property alias algorithmResultsIndicatorRef: algorithmResultsIndicator
    property string minimumVertexCoverText: ""

    property bool isSolutionExact: false
    property bool shouldDisplayAlgResults: false
    property int numberOfVerticesInMinimumCover: 0

    property int exactVertexCoverSize: 0
    property variant exactSolution:[]

    signal needUpdateStorageWithAlgorithmOutput (string calculatedResults, int algType)
    signal needUpdateTestPanelWithAlgorithmType ()

    Rectangle {
        id: rightBorderRect
        anchors.fill: parent
        color: "transparent"
        z: 0.5
        border { width: 2.0; color: "#333333"; }
    }
    clip: true

    Item {

        z: 0.4
        anchors.fill: parent


        DropDownComboBox {
            id: algComboBox

            width: 275
            anchors {
                top: parent.top
                topMargin: 30
                horizontalCenter: parent.horizontalCenter
            }

            onSelectedIndexChanged: {
                if(currentGraphIndex != -1) {
                    needUpdateTestPanelWithAlgorithmType()
                }
            }
        }

        Button {
            id: solveAlgorithmButton

            enabled: false
            anchors {
                top: algComboBox.top
                topMargin: 60
                horizontalCenter: parent.horizontalCenter
            }

            width: 275
            height: 50
            tooltip: qsTr("Press this button to find a minimum vertex cover of the selected graph.","Find a minimum vertex cover, tooltip") + mainTranslator.emptyString

            Rectangle {
                id: customButtonRect

                anchors.fill: parent; anchors.margins: -10
                color: solveAlgorithmButton.containsMouse ? "#3399FF": "#33CCFF"
                border.width: solveAlgorithmButton.containsMouse ? 4 : 2
                border.color: solveAlgorithmButton.containsMouse ? "#33FF99" : "#99FFFF"
                radius: 16.0
                states: [
                    State {
                        when: solveAlgorithmButton.pressed
                        PropertyChanges {
                            target: customButtonRect
                            color: "#00FFFF"
                        }
                    }

                ]
                Behavior on color {
                    ColorAnimation { duration: 100 }
                }


                Text {
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        verticalCenter: parent.verticalCenter
                    }

                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("Find a minimum vertex cover","Find a minimum vertex cover, title") + mainTranslator.emptyString
                }

            }

            onClicked: {
                if(currentGraphIndex >= 0) {
                    getAlgorithmOutput()
                }
            }
        }

        Rectangle {
            id: algorithmResultsIndicator
            anchors {
                top: solveAlgorithmButton.bottom
                topMargin: 30
                horizontalCenter: parent.horizontalCenter
            }

            color: isSolutionExact === false ? "#FF3333" : "#00FF99"
            width: algorithmResultsLabel.width+20
            height: algorithmResultsLabel.height+20
            radius: 16

            visible: false

            Text {
                id: algorithmResultsLabel
                anchors.centerIn: parent
                font.bold: true
                font.family: "Verdana, sans-serif"
                text: isSolutionExact === false ? qsTr("Algorithm output is not exact! Correct cover size is: ") + exactVertexCoverSize.toString() + mainTranslator.emptyString:
                                                  qsTr("Congratulations! Algorithm output is exact!") + mainTranslator.emptyString
            }
        }

        TextArea {
            id: algorithmDetailsArea

            clip: false

            readOnly: true
            wrapMode: TextEdit.NoWrap
            anchors {
                top: solveAlgorithmButton.bottom
                topMargin: 70
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }

            Rectangle {
                height: 2.0
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                }
                color: "#222222"
            }

        }
    }

    onShouldDisplayAlgResultsChanged: {
        algorithmResultsIndicator.visible = shouldDisplayAlgResults
    }

    //translation
    property string algTypeStr: qsTr("The algorithm's type is: ")+ mainTranslator.emptyString
    property string algInfoStr: qsTr("Additional information about algorithm steps: ")+ mainTranslator.emptyString
    property string algOutputTitleStr: qsTr("The algorithm output of the current Graph is:","The algorithm output of the current Graph is, first line") + mainTranslator.emptyString
    property string numOfVertInMinCoverStr: qsTr("Number of vertices in the current cover: ") + mainTranslator.emptyString
    property string noVertInCoverStr: qsTr("Can't apply the algorithm! Check your graph settings!") + mainTranslator.emptyString
    property string emptyGraphStr: qsTr("Current graph is empty! Cannot apply algorithm.") + mainTranslator.emptyString
    property string vertInAscendOrderStr: qsTr("Vertices in ascending order: ") + mainTranslator.emptyString
    property string vertInStandardOrder: qsTr("Vertices in order they were added in: ") + mainTranslator.emptyString
    property string exactSolutionStr: qsTr("Exact solution of current graph: ") + mainTranslator.emptyString
    property string exactSolutionLengthStr: qsTr("Number of vertices in the exact solution: ") + mainTranslator.emptyString

    //private functions
    function getAlgorithmOutput() {

        var outputText =  algTypeStr + algComboBox.selectedText + "\r\n"
        outputText +=  algOutputTitleStr + "\r\n"

        var verticesCount = mainGraphClass.getVerticesSizeForCurrentGraph(currentGraphIndex)

        if(verticesCount < 1) {
            outputText += emptyGraphStr
            minimumVertexCoverText = outputText
            algorithmFinished()
            return
        }

        //find exact solution for the current Graph
        var currentExactSolution = mainGraphClass.findExactSolutionForCurrentGraph(currentGraphIndex)
        exactSolution = currentExactSolution
        exactVertexCoverSize = exactSolution.length

        //find the minimum vertex cover
        var minimumVertexCoverList = mainGraphClass.solveAlgorithmForCurrentGraph(algComboBox.selectedIndex,currentGraphIndex)
        outputText += "\r\n" + numOfVertInMinCoverStr

        if(minimumVertexCoverList.length !== 0) {
            numberOfVerticesInMinimumCover = minimumVertexCoverList.length
            outputText += minimumVertexCoverList.length.toString()
            outputText += "\r\n" + vertInStandardOrder
            outputText += convertVerticesInCoverListToText(minimumVertexCoverList)
            minimumVertexCoverList.sort(function(a, b){return a-b});
            outputText += "\r\n" + vertInAscendOrderStr
            outputText += convertVerticesInCoverListToText(minimumVertexCoverList)
            minimumVertexCoverText = outputText

        } else {
            outputText += noVertInCoverStr
            minimumVertexCoverText = outputText
            exactSolution = []
        }

        algorithmFinished()
    }

    function convertVerticesInCoverListToText (vertexCoverList) {
        var coverText = "[ "
        for(var i = 0; i < vertexCoverList.length; ++i) {
            coverText += (vertexCoverList[i] +1).toString()
            if(i == vertexCoverList.length - 1) {
                coverText += " ]"
            } else {
                coverText += ", "
            }
        }

        return coverText;
    }

    function checkIfSolutionExact() {

        if(exactSolution.length === 0) {
            return "empty"
        }

        if(exactSolution.length < numberOfVerticesInMinimumCover) {
            return "approx"
        }

        return "exact"
    }

    function algorithmFinished() {
        var algResults = minimumVertexCoverText

        var solutionType = ""

        switch (checkIfSolutionExact()) {
            case "exact":
                solutionType = "exact"
                exactVertexCoverSize = 0
                isSolutionExact = true
                shouldDisplayAlgResults = true
                break;
            case "approx":
                solutionType = "approx"
                isSolutionExact = false
                shouldDisplayAlgResults = true
                break;
            case "empty":
                solutionType = "empty"
                shouldDisplayAlgResults = false
                break;
        }

        if(exactSolution.length > 0 && solutionType.localeCompare("empty") !== 0) {

            var sortedExactSolution = exactSolution.sort(function(a, b){return a-b});
            var exactSolutionCount = exactSolution.length

            algResults += "\r\n" + "\r\n" + exactSolutionStr
            algResults += convertVerticesInCoverListToText(sortedExactSolution)
            algResults += "\r\n" + exactSolutionLengthStr + exactSolutionCount.toString() + "\r\n" + "\r\n"

            //additional alg info
            algResults += algInfoStr + "\r\n"
            var algInfo = mainGraphClass.getAlgInfo()
            algResults += algInfo

        }

        algorithmDetailsArea.text = algResults

        needUpdateStorageWithAlgorithmOutput(algResults, algComboBox.selectedIndex)

        //reset property values
        solveAlgorithmButton.enabled = false
        numberOfVerticesInMinimumCover = 0
        exactSolution = []
    }
}
