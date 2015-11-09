import QtQuick 1.1

VisualDataModel {
    id: visualDataModel

    signal removingDataModelItem(int r_index)
    //each element must be: { graphProperties: "text", algorithmOutput: "text", exactnessOfResult: bool, exactResult: int }
    model: ListModel {
        id: listModel
    }

    delegate: Rectangle {

        id: visualDelegate
        property int visualIndex: index

        width: 100
        height: 24
        color: ListView.isCurrentItem ? "#FFFFCC" : delegateMouseArea.containsMouse ? "#EEEEEE" :"#DDDDDD"
        radius: 4.0
        clip: true
        smooth: true
        border { width: 2.0; color: "#000033" }
        Text {
            enabled: false
            anchors.centerIn: parent
            text: qsTr("Graph ","Created graph tab name") + mainTranslator.emptyString + (visualIndex+1)
        }

        MouseArea {
            id: delegateMouseArea
            anchors.fill: parent
            hoverEnabled: true

            onClicked: {
                graphsListView.currentIndex = visualIndex
            }
        }

        CloseButton {
            color: closeButtonMouseArea.containsMouse ? "#FF0000" :"#CC0000"

            MouseArea {
                id: closeButtonMouseArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    graphsListView.model.removingDataModelItem(visualIndex)
                    graphsListView.model.model.remove(visualDelegate.visualIndex,1)
                }
            }
        }

        //animations
        ListView.onRemove: SequentialAnimation {
            PropertyAction { target: visualDelegate; property: "ListView.delayRemove"; value: true }
            ParallelAnimation {
                NumberAnimation { target: visualDelegate; property: "width"; to: 0.0; duration: 250; easing.type: Easing.OutInBack }
                NumberAnimation { target: visualDelegate; property: "opacity"; to: 0.0; duration: 250;}
            }

            // Make sure delayRemove is set back to false so that the item can be destroyed
            PropertyAction { target: visualDelegate; property: "ListView.delayRemove"; value: false }
        }
    }
}
