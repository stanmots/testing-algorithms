import QtQuick 1.1

import com.testsoft.graphs 1.0

Item {
    id: singleGraphLayout

    property alias mainGraphClassRef: mainGraphClass

    GraphPropertiesPanel {
        id: leftPanel

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: panelsSplitter.left

        GraphSolver {
            id: mainGraphClass
        }
    }

    PanelsSplitter {
        id: panelsSplitter
    }

    AlgorithmTestPanel {
        id: algorithmTestPanel

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: panelsSplitter.right
        anchors.right: parent.right
    }

    onWidthChanged: {
        if(singleGraphLayout.width/2  < Math.abs(panelsSplitter.anchors.horizontalCenterOffset)) {
            var signHelper = 0;
            if(panelsSplitter.anchors.horizontalCenterOffset<0){
                signHelper = -1
            } else {
                signHelper = 1
            }

            panelsSplitter.anchors.horizontalCenterOffset = Math.round(singleGraphLayout.width/2 - 10)*signHelper
        }
    }
}
