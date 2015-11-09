import QtQuick 1.1

Item {
     id: tabWidget

     // Setting the default property to stack.children means any child items
     // of the TabWidget are actually added to the 'stack' item's children.
     // See the "Property Binding"
     // documentation for details on default properties.
     default property alias content: stack.children

     property int current: 0
     property color currentTabColor: "#777777"
     property color neighbourTabsColor: "#EEEEEE"
     property int tabRadius: 12
     property bool isNeedHoverEnabled: true
     property int tabWidth: 250
     property int tabHeight: 24

     onCurrentChanged: setOpacities()
     Component.onCompleted: setOpacities()

     function setOpacities() {
         for (var i = 0; i < stack.children.length; ++i) {
             stack.children[i].opacity = (i == current ? 1 : 0)
         }
     }

     Row {
         id: header
         anchors.horizontalCenter: parent.horizontalCenter

         Repeater {
             model: stack.children.length
             delegate: Rectangle {
                 id: tabDelegate

                 width: tabWidth; height: tabHeight
                 radius: tabRadius
                 color: tabWidget.current == index ? currentTabColor : neighbourTabsColor
                 border { width: 2.0; color: tabMouseArea.containsMouse ? "#00CC99" : "#0099CC";
                     Behavior on color {
                         ColorAnimation { duration: 250; }
                     }
                 }

                 Text {
                     horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter
                     anchors.fill: parent
                     text: stack.children[index].title
                     elide: Text.ElideRight
                     font.bold: tabWidget.current == index
                     color: tabWidget.current == index ? "white" : "black"
                 }
                 MouseArea {
                     id: tabMouseArea
                     anchors.fill: parent
                     hoverEnabled: isNeedHoverEnabled
                     onClicked: tabWidget.current = index
                 }

                 Behavior on color {
                     ColorAnimation { duration: 250; }
                 }
             }
         }
     }

     Item {
         id: stack
         width: tabWidget.width
         anchors.top: header.bottom; anchors.bottom: tabWidget.bottom
     }
 }
