#include "GuaranteedPredictionMethod.h"


#ifdef QT_DEBUG
#include <QDebug>
#endif

namespace graphsalgs {

QList<int> findMVCWithGuaranteedPredictionMethod(UndirectedGraphType graph) {

    QList<int> minimumVertexCoverList;

    while(true) {

        /*[1] check and add all hanging vertices to the cover */
        graphsops::addHangingVerticesToCover(minimumVertexCoverList, graph);

        /*[1.1] check if we have already the minimum vertex cover */
        if( num_edges(graph) == 0 ) {
            return minimumVertexCoverList;
        }

        /*[2] find a pair of the vertices with the max common degree */
        std::pair<quint32, quint32> testPair = graphsops::getPairOfVerticesWithMaxDegree(graph);

        /*[3] get the prediction graph of the case where: x1=0; x2=0 */
        PredictionStorage firstCaseStorage =  graphsops::getZeroPairCase(testPair, graph);
        /*[4] get the prediction graph of the case where: x1=0; x2=1 */
        PredictionStorage secondCaseStorage =  graphsops::getUnityPairCase(testPair.first, testPair.second, graph);
        /*[5] get the prediction graph of the case where: x1=1; x2=0 */
        PredictionStorage thirdCaseStorage =  graphsops::getUnityPairCase(testPair.second, testPair.first, graph);

        /*[6] find the equation with the maximum terms number */
        QList<PredictionStorage*> listWithPredictionObjects;
        listWithPredictionObjects << &firstCaseStorage << &secondCaseStorage << &thirdCaseStorage;

        quint32 maxTermsNumber = 0, caseNumber = 0;
        for(quint32 i = 0; i < 3; ++i) {
            const quint32 currentEquationSize = listWithPredictionObjects.at(i)->m_equationSize;
            if(maxTermsNumber < currentEquationSize) {
                maxTermsNumber = currentEquationSize;
                caseNumber = i;
            }
        }

        if(maxTermsNumber == 0) {

            //found the minimum vertex cover
            quint32 finalCaseNumber = 0;
            quint32 minPartialCoverSize = listWithPredictionObjects.at(0)->m_partialVertexCover.size();
            for(quint32 i = 1; i < 3; ++i) {
                if((quint32)listWithPredictionObjects.at(i)->m_partialVertexCover.size() < minPartialCoverSize) {
                    minPartialCoverSize = listWithPredictionObjects.at(i)->m_partialVertexCover.size();
                    finalCaseNumber = i;
                }
            }
            minimumVertexCoverList.append(listWithPredictionObjects.at(finalCaseNumber)->m_partialVertexCover);
            return minimumVertexCoverList;

        }

        minimumVertexCoverList.append(listWithPredictionObjects.at(caseNumber)->m_partialVertexCover);
        graph = listWithPredictionObjects.at(caseNumber)->m_graph;
    }

    return QList<int>();
}

}
