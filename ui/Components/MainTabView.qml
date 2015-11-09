import QtQuick 1.1

Item {
    visible: true

    TabWidget {
        id: tabsCollection

        anchors.fill: parent

        SingleGraphPage {
            id: singleGraphTab

            anchors.fill: parent
            property string title: qsTr("Graphs Analysis", "SingleGraph tab title") + mainTranslator.emptyString
        }

        DeepTestingPage {
            id: deepTestingTab

            anchors.fill: parent
            property string title: qsTr("Deep Testing", "Deep testing tab title") + mainTranslator.emptyString
        }
    }

    Rectangle {
        anchors {
            left: parent.left
            right: parent.right
            top: tabsCollection.top
            topMargin: 24
        }

        height: 2.0
        color: "black"
    }
}



