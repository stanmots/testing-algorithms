import QtQuick 1.1
import QtDesktop 0.1

TabWidget {
    z: 0.4
    width: 500
    height: 150

    tabRadius: 0.0

    property bool isAdjMatrixSet: false

    anchors {
        top: parent.top
        topMargin: 25
        horizontalCenter: parent.horizontalCenter
    }

    Tab {

        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }


        Connections {
            target: adjMatrixWindow
            onSaveAdjMatrixSignal: {
                isAdjMatrixSet = true;
                manualGraphTab.createGraphButtonRef.enabled = true;
            }
            onCloseAdjMatrixSignal: {
                isAdjMatrixSet = false;
                manualGraphTab.createGraphButtonRef.enabled = false;
            }

        }

        Connections {
            target: graphsListVisualModel
            onRemovingDataModelItem: mainGraphClass.removeGraph(r_index)
        }

        title: qsTr("Generate a random graph","Random graph tab's title") + mainTranslator.emptyString

        Item {
            anchors.fill: parent
            Column {
                spacing: 30
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }

                Row {
                    spacing: 15
                    anchors.horizontalCenter: parent.horizontalCenter
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Vertices","Set an adjancent matrix's vertices count, title") + mainTranslator.emptyString
                    }

                    SpinBox {
                        id: adjMatrixVerticesSpinBox

                        width: 75
                        height: 25

                        minimumValue: 0
                        maximumValue: 1001
                        value: 2

                        onValueChanged: {
                            if(generateGraphButton.enabled !== true) {
                                generateGraphButton.enabled = true
                            }
                        }

                        onQuickValueChanged: {
                            value = quickValue
                        }
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Graph's Density","Set Graph's Density, title ") + mainTranslator.emptyString
                    }

                    SpinBox {
                        id: adjMatrixDensitySpinBox

                        width: 75
                        height: 25
                        minimumValue: 1
                        maximumValue: 100
                        value: 50

                        onValueChanged: {
                            if(generateGraphButton.enabled !== true) {
                                generateGraphButton.enabled = true
                            }
                        }

                        onQuickValueChanged: {
                            value = quickValue
                        }
                    }
                }

                CustomButton {
                    id: generateGraphButton
                    anchors.horizontalCenter: parent.horizontalCenter
                    tooltip: qsTr("Press this button to generate a graph with vertices count and density you've set.",
                                  "Generate a graph's tooltip") + mainTranslator.emptyString

                    customText: qsTr("Generate a graph","Generate a graph button's title") + mainTranslator.emptyString

                    onClicked: {
                        algorithmTestPanel.solveAlgorithmButtonRef.enabled = true
                        var calculatedEdges = Math.round(adjMatrixVerticesSpinBox.value * (adjMatrixVerticesSpinBox.value - 1) * 0.5 * adjMatrixDensitySpinBox.value/100)
                        mainGraphClass.generateGraph(adjMatrixVerticesSpinBox.value, calculatedEdges)
                        appendGraphToModel(getCurrentGraphIndex())
                        //enabled = false
                    }
                }
            }
        }
    }

    Tab {
        id: manualGraphTab

        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }

        title: qsTr("Set a graph manually","Manual graph tab's title") + mainTranslator.emptyString

        property alias createGraphButtonRef: createGraphButton

        Item {

            property alias createGraphButton: createGraphButton
            anchors.fill: parent
            Column {

                spacing: 20
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }

                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: isAdjMatrixSet === false ? "#FF3333" : "#00FF99"
                    width: matrixStatusLabel.width+20
                    height: matrixStatusLabel.height+10

                    Text {
                        id: matrixStatusLabel
                        anchors.centerIn: parent
                        font.bold: true
                        font.family: "Verdana, sans-serif"
                        text: isAdjMatrixSet === false ? qsTr("An adjacency matrix is not set!","An adjacency matrix is not set, label") + mainTranslator.emptyString :
                                                         qsTr("An adjacency matrix has been set!","An adjacency matrix has been set!, label") + mainTranslator.emptyString
                    }
                }

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 10

                    CustomButton {
                        id: setAdjMatrixButton

                        tooltip: qsTr("Press this button to set an adjacency matrix whereby you can create a graph.",
                                      "Set an adjacency matrix button's tooltip") + mainTranslator.emptyString

                        customText: qsTr("Set an adjacency matrix","Set an adjacency matrix button's title") + mainTranslator.emptyString

                        onClicked: {
                            hideAllQmlWindows()
                            adjMatrixWindow.visible = true
                        }
                    }

                    CustomButton {
                        id: createGraphButton
                        enabled: false
                        tooltip: qsTr("Press this button to create a graph from the adjacency matrix which you've set using appropriate button.",
                                      "Creat a graph button's tooltip") + mainTranslator.emptyString

                        customText: qsTr("Create a graph","Create a graph button's title") + mainTranslator.emptyString

                        onClicked: {
                            createGraphFromAdjMatrix()
                            enabled = false
                        }
                    }
                }
            }
        }
    }
    //private translations
    property string curGraphsProps: qsTr("Current Graph's properties:","Current Graphs' properties:") + mainTranslator.emptyString
    property string vertsStr: qsTr("Vertices: ","Vertices: ") + mainTranslator.emptyString
    property string edgesStr: qsTr("Edges: ","Edges: ") + mainTranslator.emptyString
    property string systemsOfEquationsStr: qsTr("Systems of the graph's equations: ","Systems of the graph's equations") + mainTranslator.emptyString
    property string equationStr: qsTr("Equation ", "Equation ") + mainTranslator.emptyString
    property string listOfEdgesStr: qsTr("List of graph's edges: ","List of graph's edges") + mainTranslator.emptyString
    property string oneEdgeStr: qsTr("Edge ", "Edge ") + mainTranslator.emptyString
    property string mainEquationStr: qsTr("Main equation: ", "main equation of the graph ") + mainTranslator.emptyString

    //private functions
    function createGraphFromAdjMatrix() {
        var currentGraphIndex = getCurrentGraphIndex()
        mainGraphClass.createGraph(adjMatrixWindow.adjacencyMatrixViewRef.adjacencyMatrixSize)

        //fill created graph with edges
        for (var fe_row = 1; fe_row < adjMatrixWindow.adjacencyMatrixViewRef.adjacencyMatrixSize; ++fe_row ) {
            for(var fe_column = fe_row+1; fe_column <= adjMatrixWindow.adjacencyMatrixViewRef.adjacencyMatrixSize; ++fe_column) {
                if(adjMatrixWindow.adjacencyMatrixViewRef.matrixViewRef.matrixListModelRef.get((((fe_row-1)*adjMatrixWindow.adjacencyMatrixViewRef.adjacencyMatrixSize)-1)+fe_column).cellValue === 1) {
                    mainGraphClass.addEdgeToCurrentGraph(fe_row-1, fe_column-1, currentGraphIndex)
                }
            }
        }
        appendGraphToModel(currentGraphIndex)
    }

    function getCurrentGraphIndex() {
        var cg_index

        if(graphsListView.currentIndex === -1) {
            cg_index = 0
        } else {
            cg_index = graphsListView.currentIndex + 1
        }

        return cg_index;
    }

    function appendGraphToModel(graphIndex) {
        //append graph to the list model
        graphsListVisualModel.model.append({ graphProperties: getGraphPropertiesText(graphIndex)})

        graphsListView.currentIndex = graphsListVisualModel.model.count-1
    }

    function getGraphPropertiesText (currentGraphIndex) {

        //first line
        var baseText = curGraphsProps

        //second line
        var verticesCount = mainGraphClass.getVerticesSizeForCurrentGraph(currentGraphIndex)
        var edgesCount = mainGraphClass.getEdgesSizeForCurrentGraph(currentGraphIndex)
        baseText += "\r\n" + vertsStr + verticesCount.toString() + "; "
        baseText +=  edgesStr + edgesCount.toString() + "\r\n"

        //third line
        baseText += "\r\n" + systemsOfEquationsStr

        //equations field
        for (var ef_row = 0; ef_row < verticesCount; ++ef_row ) {
            var adjacentVerticesList = mainGraphClass.getInitialAdjacentVertices(ef_row,currentGraphIndex)
            baseText += "\r\n" + equationStr + (ef_row+1).toString() + ": "
            //equation ending
            if(adjacentVerticesList.length <= 0) {
                baseText += " N/A"
            } else {
                adjacentVerticesList.sort(function(a, b){return a-b});
                for(var ef_column = 0; ef_column < adjacentVerticesList.length; ++ef_column) {
                    baseText += "(" + (ef_row+1).toString() + " v " + (adjacentVerticesList[ef_column] +1).toString() + ")"
                    if(ef_column !== adjacentVerticesList.length - 1) {
                        baseText += " + "
                    }
                }
            }
        }

        //print graphs's main equation and graphs's edges
        var edgesListOfCurrentGraph = mainGraphClass.getEdgesListForCurrentGraph(currentGraphIndex)

        var mainEquationText = "\r\n\r\n" + mainEquationStr + "\r\n\F(G) = "
        var graphsEdgesText = "\r\n\r\n" + listOfEdgesStr
        var currentEdge = ""

        for (var c_edge = 0, edge_number = 1; c_edge < edgesListOfCurrentGraph.length; c_edge += 2, ++edge_number ) {

            graphsEdgesText += "\r\n" + oneEdgeStr + edge_number.toString() + ": "

            currentEdge = "(" + (edgesListOfCurrentGraph[c_edge]+1).toString() + " v " + (edgesListOfCurrentGraph[c_edge+1] +1).toString() + ")"

            graphsEdgesText += currentEdge
            mainEquationText += currentEdge
        }

        baseText += mainEquationText + graphsEdgesText

        return baseText
    }
}
