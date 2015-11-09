import QtQuick 1.1
import QtDesktop 0.1

Rectangle {
    id: adjMatrixWindowToolBar

    property alias adjMatrixSizeSpinBoxRef: adjMatrixSizeSpinBox
    property alias saveAdjMatrixButtonRef: saveAdjMatrixButton

    height: 40
    anchors {
        top: parent.top
        left: parent.left
        right: parent.right
    }
    color: "#EEEEEE"
    Row {
        spacing: 16.0
        width: 200
        anchors {
            left: parent.left
            leftMargin: 20
            verticalCenter: parent.verticalCenter
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: qsTr("Set an adjancent matrix's size","Set an adjancent matrix's size title") + mainTranslator.emptyString
        }

        Rectangle {
            anchors.verticalCenter: parent.verticalCenter
            width: 70
            height: 25
            color: "white"

            SpinBox {
                id: adjMatrixSizeSpinBox

                anchors.fill: parent
                minimumValue: 0
                maximumValue: 50
                value: 1
            }
        }

        CustomButton {
            id: saveAdjMatrixButton

            enabled: !adjacencyMatrixView.isLoadingAdjMatrix

            customText: qsTr("Save the adjacency matrix","Save the adjacency matrix button") + mainTranslator.emptyString
            tooltip: qsTr("Press this button to save the adjacency matrix whereby you can create a graph.",
                          "Save the adjacency matrix button's tooltip") + mainTranslator.emptyString
            onClicked: {
                hideAllQmlWindows()
                mainTabView.visible = true
                saveAdjMatrixSignal()
            }
        }
    }

    //top line
    Rectangle {
        height: 2.0
        anchors{
            top: parent.top
            left: parent.left
            right: parent.right
        }
        color: "#777777"
    }
}

