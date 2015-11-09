import QtQuick 1.1

GridView {
    id: matrix

    interactive: false

    property int rows: 0
    property int columns: 0
    property bool isSymmetricMatrix: false
    property bool diagonalCellsEnabled: true
    property bool symmetricAnimationEnabled: false
    property alias matrixListModelRef: matrixListModel

    //used in adjacency matrix
    property color counterpartCellBaseColor: "white"
    property color counterpartCellAnimationColor: "#CC33FF"

    //private properties
    property int m_rows: 0
    property int m_columns: 0
    property int m_progress: matrixListModel.count

    signal progressChanged()

    onM_progressChanged: progressChanged()

    width: cellWidth * columns
    height: cellHeight * rows

    model: ListModel {
        id: matrixListModel

        /*listElement is:
        { "cellValue": 0,"isNeedRunAnimation": false, "isMatrixCellInHoveredRange": false }
        */
    }

    Component {
        id: emptyComponent
        Item {
            enabled: false
            clip: false
            smooth: false
            focus: false
            visible: false
            opacity: 0.0
        }
    }

    Component {
        id: matrixCellComponent

        Rectangle {
            id: matrixCell

            property int rowIndex: index >= 0 ? Math.ceil((index+1)/matrix.columns) : 0
            property int columnIndex: index >= 0 ? (index+1) - (rowIndex-1)*matrix.columns : 0
            property bool counterpartCellAnimationEnabled: isNeedRunAnimation

            width: matrix.cellWidth
            height: matrix.cellHeight

            //optimization properties
            smooth: false
            clip: false
            focus: false

            color: isMatrixCellInHoveredRange ? "#99CCCC": !diagonalCellsEnabled ? rowIndex===columnIndex? "#DDDDDD" : "white" : "white"

            Text {
                id: matrixCellText

                anchors.centerIn: parent

                text: !diagonalCellsEnabled ? rowIndex === columnIndex ? "X" : cellValue : cellValue

                //optimization properties
                smooth: false
                clip: false
                enabled: false
                focus: false
            }

            Rectangle {
                id: borderRect

                anchors {
                    fill: parent
                    margins: 2.0
                }

                color: "transparent"

                //optimization properties
                smooth: false
                clip: false
                enabled: false
                focus: false

                border {
                    width: matrixCellMouseArea.containsMouse ? 3.0 : 0.0
                    color: "#FFCC99"
                    Behavior on color {
                        ColorAnimation { duration: 250; }
                    }
                }
            }

            border {
                width: 1.0
                color: "#333333"
            }

            MouseArea {
                id: matrixCellMouseArea

                anchors.fill: parent
                hoverEnabled: true

                onClicked: {
                    if(diagonalCellsEnabled === false ) {
                        if(rowIndex !== columnIndex) {
                            setReverseCellVaue()
                        }
                    } else {
                        setReverseCellVaue()
                    }
                }

                onContainsMouseChanged: {
                    if(containsMouse === true){
                        colorizeHoveredRange(rowIndex, columnIndex)
                    } else {
                        clearHoveredRange(rowIndex, columnIndex)
                    }
                }
            }

            //private animations
            SequentialAnimation {
                id: counterpartCellAnimation
                ColorAnimation { target: matrixCell; property: "color"; to: matrix.counterpartCellAnimationColor ; duration: 500 }
                ColorAnimation { target: matrixCell; property: "color"; to: matrix.counterpartCellBaseColor; duration: 500 }
            }

            //signal handlers
            onCounterpartCellAnimationEnabledChanged: {
                if(isNeedRunAnimation === true && counterpartCellAnimation.running === false) {
                    counterpartCellAnimation.start()
                }
            }
            //delegate functions
            function setReverseCellVaue() {
                var reverseCellValue = !cellValue ? 1 : 0;
                matrixListModel.setProperty(index, "cellValue", reverseCellValue)

                if(isSymmetricMatrix === true) {

                    //set counterpart cell to the same value as clicked cell
                    var counterpartCellIndex = convertToOneDimIndex(columnIndex, rowIndex, matrix.columns)
                    matrixListModel.setProperty(counterpartCellIndex, "cellValue", reverseCellValue)

                    if(symmetricAnimationEnabled === true) {
                        matrixListModel.setProperty(counterpartCellIndex, "isNeedRunAnimation", true)
                        matrixListModel.setProperty(counterpartCellIndex, "isNeedRunAnimation", false)
                    }
                }
            }
        }
    }

    delegate: matrixCellComponent

    onRowsChanged: {
        redrawRows()
    }

    onColumnsChanged: {
        redrawColumns()
    }

    //private functions
    function redrawRows() {

        if(rows > m_rows) {
            addRows(rows - m_rows)
        } else if ( rows < m_rows) {
            removeRows(m_rows - rows)
        }

        m_rows = rows
    }

    function redrawColumns() {

        if(columns > m_columns) {
            addColumns(columns - m_columns)
        } else if (columns < m_columns) {
            matrix.delegate = emptyComponent
            removeColumns(m_columns - columns)
            matrix.delegate = matrixCellComponent
        }

        m_columns = columns
    }

    function addRows(rowsCount) {
        for(var i = 0; i < rowsCount * matrix.columns; ++i) {
            matrixListModel.append({ "cellValue": 0,"isNeedRunAnimation": false, "isMatrixCellInHoveredRange": false })
        }
    }

    function removeRows(rowsCount) {
        for(var i = 0; i < rowsCount * matrix.m_columns; ++i) {
            matrixListModel.remove(matrixListModel.count - 1)
        }
    }

    function addColumns(columnsCount) {
        for(var ri = 1; ri <= matrix.m_rows; ++ri) {
            for(var ci = 0; ci < columnsCount; ++ci) {
                matrixListModel.insert(ri * columns - columnsCount,{ "cellValue": 0,"isNeedRunAnimation": false, "isMatrixCellInHoveredRange": false })
            }
        }
    }

    function removeColumns(columnsCount) {
        for(var ri = matrix.m_rows; ri >= 1; --ri) {
            for(var ci = 0; ci < columnsCount; ++ci) {
                matrixListModel.remove(ri * m_columns - columnsCount)
            }
        }
    }

    function convertToOneDimIndex(row, column, columnsCount) {
        //matrix cell index starts from 1, but listmodel element - from 0
        return ((row-1)*columnsCount)+(column-1)
    }

    function colorizeHoveredRange(rowRange, columnRange) {

        //colorize columns
        for(var s_row = 1; s_row <= rowRange; ++s_row){
            matrixListModel.setProperty(convertToOneDimIndex(s_row, columnRange, matrix.columns), "isMatrixCellInHoveredRange", true)
        }
        //colozie rows
        for(var s_column = 1; s_column < columnRange; ++s_column){
            matrixListModel.setProperty(convertToOneDimIndex(rowRange, s_column, matrix.columns), "isMatrixCellInHoveredRange", true)
        }
    }

    function clearHoveredRange(rowRange, columnRange) {

        //restore columns' color
        for(var r_row = 1; r_row <= rowRange; ++r_row) {
            matrixListModel.setProperty(convertToOneDimIndex(r_row, columnRange, matrix.columns), "isMatrixCellInHoveredRange", false)
        }

        //restore rows' color
        for(var r_column = 1; r_column < columnRange; ++r_column) {
            matrixListModel.setProperty(convertToOneDimIndex(rowRange, r_column, matrix.columns), "isMatrixCellInHoveredRange", false)
        }
    }
}

