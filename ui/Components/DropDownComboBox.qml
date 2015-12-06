import QtQuick 1.1
import QtDesktop 0.1

ComboBox {

    property string coveringMethodStr : qsTr("Method based on the covering pairs searching") + mainTranslator.emptyString
    property string combinedMethodStr : qsTr("Combined Method") + mainTranslator.emptyString
    property string shortestPathMethodStr : qsTr("Method with searching the shortest path") + mainTranslator.emptyString
    property string guaranteedPredictionMethodStrV20: qsTr("Method with the guaranteed prediction(Approximation type).") + mainTranslator.emptyString
    property string optimalAlgorithmStr: qsTr("Optimized method with the heuristic prediction") + mainTranslator.emptyString
    property string connectedTreeMethodStr: qsTr("Method based on the connected tree graphs") + mainTranslator.emptyString
    property string guaranteedPredictionMethodStr: qsTr("Method with guaranteed predictions") + mainTranslator.emptyString
    property string greedyAlgorithmStr: qsTr("Greedy algorithm (deleting vertices with the max degree)") + mainTranslator.emptyString

    model: [ coveringMethodStr, combinedMethodStr, shortestPathMethodStr, guaranteedPredictionMethodStrV20, optimalAlgorithmStr, connectedTreeMethodStr, guaranteedPredictionMethodStr, greedyAlgorithmStr]
}
