import QtQuick 1.1
import QtDesktop 0.1

Rectangle {
    color: "#DDDDDD"

    Rectangle {
        id: background
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: 175
        color: "#999999"

        Rectangle {
            height: 2.0
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
            color: "black"
        }
    }


    ScrollArea {
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: 200

        Item {
            id: configureTestingPanel

            height: 175
            width: childrenRect.width

            Text {
                id: testingTitle
                anchors {
                    top: parent.top
                    topMargin: 10
                    left: parent.left
                    leftMargin: 20
                }

                font.bold: true
                font.pointSize: 10

                text: qsTr("Set the testing parameters: ") + mainTranslator.emptyString
            }

            Rectangle {
                id: testingParameters

                anchors{
                    top: testingTitle.bottom
                    topMargin: 10
                    left: parent.left
                    leftMargin: 10
                }
                color: "#FFFFCC"
                width: 500
                height: 125

                //vertices' parameters
                Text {
                    id: verticesTitle
                    anchors {
                        top: parent.top
                        topMargin: 5
                        left: parent.left
                        leftMargin: 20
                    }
                    font.bold: true
                    text: qsTr("Vertices' parameters: ") + mainTranslator.emptyString
                }

                Row {
                    id: verticesRow

                    spacing: 70

                    anchors {
                        top: verticesTitle.top
                        topMargin: 15
                        left: parent.left
                        leftMargin: 50
                    }

                    Column {
                        spacing: 5
                        anchors {
                            verticalCenter: parent.verticalCenter
                        }

                        Text {
                            id: startVertValueTitle

                            text: qsTr("Start value") + mainTranslator.emptyString
                        }

                        SpinBox {
                            id: startVertValueSpinBox

                            width: 75
                            height: 22
                            minimumValue: 1
                            maximumValue: 1001
                            value: 10
                        }
                    }

                    Column {
                        spacing: 5
                        anchors {
                            verticalCenter: parent.verticalCenter
                        }

                        Text {
                            id: endVertValueTitle
                            text: qsTr("End value") + mainTranslator.emptyString
                        }

                        SpinBox {
                            id: endVertValueSpinBox

                            width: 75
                            height: 22
                            minimumValue: 1
                            maximumValue: 1001
                            value: 50
                        }
                    }

                    Column {
                        spacing: 5
                        anchors {
                            verticalCenter: parent.verticalCenter
                        }

                        Text {
                            id: stepVertValueTitle
                            text: qsTr("Step value") + mainTranslator.emptyString
                        }

                        SpinBox {
                            id: stepVertValueSpinBox

                            width: 75
                            height: 22
                            minimumValue: 1
                            maximumValue: 1001
                            value: 10
                        }
                    }
                }

                //density parameters
                Text {
                    id: densityTitle
                    anchors {
                        top: verticesRow.bottom
                        topMargin: 5
                        left: parent.left
                        leftMargin: 20
                    }

                    font.bold: true
                    text: qsTr("Density parameters: ") + mainTranslator.emptyString
                }

                Row {
                    spacing: 70
                    anchors {
                        top: densityTitle.top
                        topMargin: 15
                        left: parent.left
                        leftMargin: 50
                    }

                    Column {
                        spacing: 5
                        anchors {
                            verticalCenter: parent.verticalCenter
                        }

                        Text {
                            id: startDensityValueTitle
                            text: qsTr("Start value") + mainTranslator.emptyString
                        }

                        SpinBox {
                            id: startDensityValueSpinBox

                            width: 75
                            height: 22
                            minimumValue: 1
                            maximumValue: 100
                            value: 10
                        }
                    }

                    Column {
                        spacing: 5
                        anchors {
                            verticalCenter: parent.verticalCenter
                        }

                        Text {
                            id: endDensityValueTitle
                            text: qsTr("End value") + mainTranslator.emptyString
                        }

                        SpinBox {
                            id: endDensityValueSpinBox

                            width: 75
                            height: 22
                            minimumValue: 1
                            maximumValue: 100
                            value: 70
                        }
                    }

                    Column {
                        spacing: 5
                        anchors {
                            verticalCenter: parent.verticalCenter
                        }

                        Text {
                            id: stepDensityValueTitle
                            text: qsTr("Step value") + mainTranslator.emptyString
                        }

                        SpinBox {
                            id: stepDensityValueSpinBox

                            width: 75
                            height: 22
                            minimumValue: 1
                            maximumValue: 100
                            value: 10
                        }
                    }
                }
            }

            //tests count
            Text {
                id: testsCountTitle
                anchors {
                    top: parent.top
                    topMargin: 10
                    left: testingParameters.right
                    leftMargin: 50
                }

                font.bold: true
                font.pointSize: 10

                text: qsTr("Set tests count: ") + mainTranslator.emptyString
            }

            Rectangle {
                id: testsCountPanel

                anchors{
                    top: testsCountTitle.bottom
                    topMargin: 10
                    horizontalCenter: testsCountTitle.horizontalCenter
                }
                color: "#FFFFCC"
                width: 100
                height: 125

                Text {
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: testsCountSpinBox.top
                        bottomMargin: 10
                    }
                    font.bold: true
                    text: qsTr("Count: ") + mainTranslator.emptyString
                }

                SpinBox {
                    id: testsCountSpinBox
                    anchors {
                        verticalCenter: parent.verticalCenter
                        horizontalCenter: parent.horizontalCenter
                    }

                    width: 75
                    height: 30
                    minimumValue: 1
                    maximumValue: 5001
                    value: 5
                }
            }

            //choose algorithm type
            Text {
                id: algTypeTitle
                anchors {
                    top: parent.top
                    topMargin: 10
                    left: testsCountPanel.right
                    leftMargin: 150
                }

                font.bold: true
                font.pointSize: 10

                text: qsTr("Choose algorigthm type: ") + mainTranslator.emptyString
            }

            DropDownComboBox {
                id: algType
                width: 300
                anchors {
                    top: testsCountPanel.top
                    horizontalCenter:  algTypeTitle.horizontalCenter
                }
            }

            Button {
                id: startTestButton
                anchors {
                    top: algType.bottom
                    topMargin: 30
                    horizontalCenter: algType.horizontalCenter
                }

                width: 200
                height: 50
                tooltip: qsTr("Press this one to start testing") + mainTranslator.emptyString

                text: qsTr("Start testing") + mainTranslator.emptyString


                onClicked: {
                    enabled = false
                    configureTestingPanel.enabled = false
                    getTestingOutput()
                }
            }
        }
    }

    TextArea {
        id: graphDetailsArea
        clip: false

        font.pointSize: 15
        readOnly: true
        wrapMode: TextEdit.NoWrap
        anchors {
            top: background.bottom
            topMargin: 25
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            bottomMargin: 20
        }
    }

    property string testOutputTitle: qsTr("Testing graphs with the current parameters: ") + mainTranslator.emptyString
    property string errorsFrequency: qsTr("Frequency of the algorithm's errors: ") + mainTranslator.emptyString
    property string numberOfOperationsStr: qsTr("Number of operations: ") + mainTranslator.emptyString
    property string relativeErrorsStr: qsTr("Relative Errors: ") + mainTranslator.emptyString
    property string exacutionProbabilityStr: qsTr("Exacution Probability: ") + mainTranslator.emptyString
    property string exacutionTimeStr: qsTr("Exacution Time, ms: ") + mainTranslator.emptyString
    property string exacutionProbabilityWithAllowedTimeStr: qsTr("Exacution Probability with allowed time (300 ms): ") + mainTranslator.emptyString

    //private functions
    function repeat(str, count) {
        if (count < 1) return "_";
        var result = "";
        while (count > 1) {
            if (count & 1) result += str;
            count >>= 1
            str += str;
        }
        return result + str;
    }

    function getTestingOutput() {

        var MAX_DIGITS_NUMBER_FREQ = 7
        var MAX_DIGITS_NUMBER_NUMOPS = 10
        var MAX_DIGITS_NUMBER_OTHERS = 12

        var ALLOWED_TIME = 300

        var ALIGNMENT_SYMBOL = "_"

        graphDetailsArea.text = ""

        var testOutput = testOutputTitle + "\r\n\r\n" + errorsFrequency + "\r\n"
        var numberOfOperationsOutput = "\r\n\r\n" + numberOfOperationsStr + "\r\n"
        var relativeErrorsOutput = "\r\n\r\n" + relativeErrorsStr + "\r\n"
        var exacutionProbabilityOutput = "\r\n\r\n" + exacutionProbabilityStr + "\r\n"
        var exacutionTimeOutput = "\r\n\r\n" + exacutionTimeStr + "\r\n"
        var exacutionProbabilityWithAllowedTimeOutput = "\r\n\r\n" + exacutionProbabilityWithAllowedTimeStr + "\r\n"

        var firstRowDensityAxisFreq = "0" + repeat(ALIGNMENT_SYMBOL, MAX_DIGITS_NUMBER_FREQ-1)
        var firstRowDensityAxisNumops = "0" + repeat(ALIGNMENT_SYMBOL, MAX_DIGITS_NUMBER_NUMOPS-1)
        var firstRowDensityAxisOthers = "0" + repeat(ALIGNMENT_SYMBOL, MAX_DIGITS_NUMBER_OTHERS-1)


        //fill first output row
        for(var rd = startDensityValueSpinBox.value; rd <= endDensityValueSpinBox.value; rd += stepDensityValueSpinBox.value) {
            firstRowDensityAxisFreq += rd
            firstRowDensityAxisNumops += rd
            firstRowDensityAxisOthers += rd

            if(rd + stepDensityValueSpinBox.value <= endDensityValueSpinBox.value) {

                firstRowDensityAxisFreq +=  repeat(ALIGNMENT_SYMBOL, MAX_DIGITS_NUMBER_FREQ - rd.toString().length)
                firstRowDensityAxisNumops +=  repeat(ALIGNMENT_SYMBOL, MAX_DIGITS_NUMBER_NUMOPS - rd.toString().length)
                firstRowDensityAxisOthers +=  repeat(ALIGNMENT_SYMBOL, MAX_DIGITS_NUMBER_OTHERS - rd.toString().length)
            }
        }

        testOutput += firstRowDensityAxisFreq
        numberOfOperationsOutput += firstRowDensityAxisNumops
        relativeErrorsOutput += firstRowDensityAxisOthers
        exacutionProbabilityOutput += firstRowDensityAxisOthers
        exacutionTimeOutput += firstRowDensityAxisOthers
        exacutionProbabilityWithAllowedTimeOutput += firstRowDensityAxisOthers

        var maxValue = (Math.floor((endVertValueSpinBox.value - startVertValueSpinBox.value)/stepVertValueSpinBox.value)+1) *
                (Math.floor((endDensityValueSpinBox.value - startDensityValueSpinBox.value)/stepDensityValueSpinBox.value)+1) * testsCountSpinBox.value

        mainProgressDialog.setMaximum(maxValue)
        mainProgressDialog.show()
        var progress = 0
        mainProgressDialog.setValue(progress)

        for(var v = startVertValueSpinBox.value; v <= endVertValueSpinBox.value; v += stepVertValueSpinBox.value) {

            testOutput += "\r\n" + v + repeat(ALIGNMENT_SYMBOL, MAX_DIGITS_NUMBER_FREQ - v.toString().length)
            numberOfOperationsOutput += "\r\n" + v + repeat(ALIGNMENT_SYMBOL, MAX_DIGITS_NUMBER_NUMOPS - v.toString().length)
            relativeErrorsOutput += "\r\n" + v + repeat(ALIGNMENT_SYMBOL, MAX_DIGITS_NUMBER_OTHERS - v.toString().length)
            exacutionProbabilityOutput += "\r\n" + v + repeat(ALIGNMENT_SYMBOL, MAX_DIGITS_NUMBER_OTHERS - v.toString().length)
            exacutionTimeOutput += "\r\n" + v + repeat(ALIGNMENT_SYMBOL, MAX_DIGITS_NUMBER_OTHERS - v.toString().length)
            exacutionProbabilityWithAllowedTimeOutput += "\r\n" + v + repeat(ALIGNMENT_SYMBOL, MAX_DIGITS_NUMBER_OTHERS - v.toString().length)

            for(var d = startDensityValueSpinBox.value; d <= endDensityValueSpinBox.value; d += stepDensityValueSpinBox.value) {

                var errorsCount = 0
                var numberOfOperations = 0
                var relativeErrors = 0
                var exacutionProbability = 0
                var exacutionProbabilityWithAllowedTime = 0

                var startTime = 0
                var endTime = 0

                for(var t = 0; t < testsCountSpinBox.value; ++t) {

                    ++progress
                    mainProgressDialog.setValue(progress)

                    //[1] Generate a graph with the current vertices count and density
                    var calculatedEdges = Math.round(v * (v - 1) * 0.5 * d/100)
                    singleGraphTab.mainGraphClassRef.generateGraph(v, calculatedEdges)


                    //[2] Find the minimum vertex cover
                    var gIndex = singleGraphTab.mainGraphClassRef.getCurrentGraphsCount() - 1

                    if(startTime === 0) startTime = new Date().getTime()
                    var minimumVertexCoverList = singleGraphTab.mainGraphClassRef.solveAlgorithmForCurrentGraph(algType.selectedIndex, gIndex)
                    if(endTime === 0) endTime = new Date().getTime() - startTime

                    //[3] Find all minimum vertex covers
                    singleGraphTab.mainGraphClassRef.getAllVertexCoversOfCurrentGraph(gIndex, false)

                    //[4] Check if the algorithm's output is exact
                    var arrayOfCoversLengths = singleGraphTab.mainGraphClassRef.getArrayOfCoversLengths()
                    for(var i = 0; i < arrayOfCoversLengths.length; ++i) {
                        if(arrayOfCoversLengths[i] < minimumVertexCoverList.length) {
                            //error is found
                            ++errorsCount
                            if(relativeErrors === 0) relativeErrors = +(Math.abs(minimumVertexCoverList.length - arrayOfCoversLengths[i])/arrayOfCoversLengths[i]).toFixed(MAX_DIGITS_NUMBER_OTHERS-2)
                            break
                        }
                    }
                    //[4.1] Get number of operations
                    numberOfOperations += singleGraphTab.mainGraphClassRef.getNumberOfOperations()

                    //[5] Remove tested graph
                    singleGraphTab.mainGraphClassRef.removeGraph(gIndex)
                }

                testOutput += errorsCount

                numberOfOperationsOutput += numberOfOperations

                relativeErrorsOutput += relativeErrors

                exacutionProbability = +(errorsCount/testsCountSpinBox.value).toFixed(MAX_DIGITS_NUMBER_OTHERS-2)
                exacutionProbabilityOutput += exacutionProbability

                exacutionTimeOutput += +(endTime).toFixed(MAX_DIGITS_NUMBER_OTHERS)

                exacutionProbabilityWithAllowedTime += +(1 - Math.pow(Math.E, -(ALLOWED_TIME/endTime))).toFixed(MAX_DIGITS_NUMBER_OTHERS-2)
                exacutionProbabilityWithAllowedTimeOutput += exacutionProbabilityWithAllowedTime

                if(d + stepDensityValueSpinBox.value <= endDensityValueSpinBox.value) {

                    testOutput += repeat(ALIGNMENT_SYMBOL, MAX_DIGITS_NUMBER_FREQ - errorsCount.toString().length)

                    numberOfOperationsOutput += repeat(ALIGNMENT_SYMBOL, MAX_DIGITS_NUMBER_NUMOPS - numberOfOperations.toString().length)
                    relativeErrorsOutput += repeat(ALIGNMENT_SYMBOL, MAX_DIGITS_NUMBER_OTHERS - relativeErrors.toString().length)
                    exacutionProbabilityOutput += repeat(ALIGNMENT_SYMBOL, MAX_DIGITS_NUMBER_OTHERS - exacutionProbability.toString().length)
                    exacutionTimeOutput += repeat(ALIGNMENT_SYMBOL, MAX_DIGITS_NUMBER_OTHERS - endTime.toString().length)
                    exacutionProbabilityWithAllowedTimeOutput += repeat(ALIGNMENT_SYMBOL, MAX_DIGITS_NUMBER_OTHERS - exacutionProbabilityWithAllowedTime.toString().length)

                }
            }
        }


        testOutput += "\r\n\r\n" + numberOfOperationsOutput + "\r\n\r\n" +
                relativeErrorsOutput + "\r\n\r\n" +
                exacutionProbabilityOutput + "\r\n\r\n" +
                exacutionTimeOutput + "\r\n\r\n" +
                exacutionProbabilityWithAllowedTimeOutput

        mainProgressDialog.hide()
        graphDetailsArea.text = testOutput

        configureTestingPanel.enabled = true
        startTestButton.enabled = true
    }
}
