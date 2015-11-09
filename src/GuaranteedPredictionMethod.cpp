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

        /*[7] check if the other equations are included in the equation
        with the maximum terms number*/
//        for(quint32 i = 0; i < 3; ++i) {
//            if(i != caseNumber) {
//                for(quint32 j = 0; j < listWithPredictionObjects.at(i)->m_equationSize; ++j) {
//                    std::pair<quint32, quint32> currentPair = listWithPredictionObjects.at(i)->m_equation[j];
//                    std::vector<std::pair<quint32, quint32> >::iterator it = std::find_if(listWithPredictionObjects.at(caseNumber)->m_equation.begin(),
//                                                                                                     listWithPredictionObjects.at(caseNumber)->m_equation.end(),
//                                                                                                     graphsops::MatchPairs(currentPair.first, currentPair.second));

//                    if(it == listWithPredictionObjects.at(caseNumber)->m_equation.end()) {
//                        qDebug()<<"test";
//                        vertex_desc_t firstVertex = graphsops::getVertexAtIndexFromPropertyMap(currentPair.first, graph);
//                        vertex_desc_t secondVertex = graphsops::getVertexAtIndexFromPropertyMap(currentPair.second, graph);
//                        const quint32 firstVDegree = boost::out_degree(firstVertex, graph);
//                        const quint32 secondVDegree = boost::out_degree(secondVertex, graph);
//                        std::vector<quint32*> cVec;
//                        cVec.push_back(&currentPair.first);
//                        cVec.push_back(&currentPair.second);

//                        quint32 vertexInVectorIndex = -1;
//                        if(firstVDegree > secondVDegree) {
//                            vertexInVectorIndex = 0;
//                        } else {
//                            vertexInVectorIndex = 1;
//                        }

//                        if(minimumVertexCoverList.contains(*cVec[vertexInVectorIndex]) == false) {
//                             minimumVertexCoverList.append(*cVec[vertexInVectorIndex]);

//                             switch (vertexInVectorIndex) {
//                             case 0:
//                                 boost::clear_vertex(firstVertex, graph);
//                                 boost::remove_vertex(firstVertex, graph);
//                                 break;
//                             case 1:
//                                 boost::clear_vertex(secondVertex, graph);
//                                 boost::remove_vertex(secondVertex, graph);
//                                 break;
//                             }
//                        }
//                    }
//                }
//            }
//        }

    }

    return QList<int>();
}

}
