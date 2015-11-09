import QtQuick 1.1
import QtDesktop 0.1

ComboBox {

    property string shortestPathMethodStr : qsTr("Method with searching the shortest path") + mainTranslator.emptyString
    property string guaranteedPredictionMethodStrV20: qsTr("Method with the guaranteed prediction(Approximation type).") + mainTranslator.emptyString
    property string optimalAlgorithmStr: qsTr("Method of the unwarranted prediction") + mainTranslator.emptyString
    property string connectedTreeMethodStr: qsTr("Method based on the connected tree graphs") + mainTranslator.emptyString
    property string guaranteedPredictionMethodStr: qsTr("Optimized method with the heuristic prediction") + mainTranslator.emptyString
    property string standardAlgorithmStr: qsTr("Method of deleting vertices with a max degree") + mainTranslator.emptyString

    model: [ shortestPathMethodStr, guaranteedPredictionMethodStrV20, optimalAlgorithmStr, connectedTreeMethodStr, guaranteedPredictionMethodStr, standardAlgorithmStr]
}
