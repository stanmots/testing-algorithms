#include "CoveringPairsMethod.h"
#include "CoveringPairHolder.h"
#include "AlgorithmPropertiesSingleton.h"

namespace graphsalgs {

QList<int> findMVCWithCoveringPairsMethod(UndirectedGraphType graph) {

    QList<int> mvc;
    UndirectedGraphType baseGraph = graph;

    AlgorithmPropertiesSingleton::getInstance().getNumbeOfOperationsRef() = 0;

    static const QString kStepsStr = QObject::tr("Step");
    static const QString kChosenVertexStr = QObject::tr("Chosen vertex: ");
    static const QString kPairsWithSameMaxDegreeStr = QObject::tr("Pairs with same max frequency: ");
    static const QString kPairsWithVertexStr = QObject::tr("Pairs with vertex ");
    static const QString kPairsAndDegreeOutputStr = QObject::tr("Pairs of vertices with the corresponding degree: ");

    quint32 stepsCount = 0;

    while(true) {

        /*[1] process hanging vertices */
        graphsops::addHangingVerticesToCover(mvc, graph);

        /*[2] check whether we need to stop the algorithm */
        if( num_edges(graph) == 0 ) {
            return mvc;
        }

        ++stepsCount;
        AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append("\r\n\r\n" + kStepsStr + " " + QString::number(stepsCount) + "\r\n\r\n");

        /*[3] get all pairs properties */
        vertex_iter_t vertexItBegin, vertexItEnd, next, innerNext;
        boost::tie(vertexItBegin, vertexItEnd) = vertices(graph);
        QList<CoveringPairHolder> pairsProperties;

        AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append("\r\n" + kPairsAndDegreeOutputStr + "\r\n");
        AlgorithmPropertiesSingleton::getInstance().getNumbeOfOperationsRef() += num_vertices(graph);

        for (next = vertexItBegin; vertexItBegin != vertexItEnd; vertexItBegin = next) {
            ++next;
            innerNext = next;

            const quint32 baseVertexIndex =  graphsops::getIndexOfVertex(*vertexItBegin, graph);
            const quint32 baseVertexDegree = boost::out_degree(*vertexItBegin, graph);

            if(next != vertexItEnd)
                AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append("\r\n" + kPairsWithVertexStr + QString::number(baseVertexIndex + 1) + ": ");

            for (vertexItBegin = innerNext; vertexItBegin != vertexItEnd; vertexItBegin = innerNext) {
                ++innerNext;

                const quint32 pairedVertexIndex =  graphsops::getIndexOfVertex(*vertexItBegin, graph);
                quint32 pairedVertexDegree = boost::out_degree(*vertexItBegin, graph);

                if(graphsops::areVerticesAdjacent(baseVertexIndex, pairedVertexIndex, graph) == true) {
                    --pairedVertexDegree;
                }

                AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append(QString::number(baseVertexIndex + 1) + " " + QString::number(pairedVertexIndex + 1) + "(" +
                                                                                    QString::number(baseVertexDegree + pairedVertexDegree) + ") ");

                CoveringPairHolder currentHolder(baseVertexDegree,
                                                 pairedVertexDegree,
                                                 baseVertexDegree + pairedVertexDegree,
                                                 std::make_pair(baseVertexIndex, pairedVertexIndex));
                pairsProperties.append(currentHolder);
            }
        }

        AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append("\r\n");


        /*[4] find pairs with maximum degree */
        quint32 maxPairDegree = pairsProperties.at(0).getCommonDegree();
        QList<CoveringPairHolder> maxPairs;
        maxPairs.append(pairsProperties.at(0));

        AlgorithmPropertiesSingleton::getInstance().getNumbeOfOperationsRef() += pairsProperties.size();

        for(int i = 1; i < pairsProperties.size(); ++i) {

            if(maxPairDegree < pairsProperties.at(i).getCommonDegree()) {
                maxPairDegree = pairsProperties.at(i).getCommonDegree();

                maxPairs.clear();
                maxPairs.append(pairsProperties.at(i));
            } else if(maxPairDegree == pairsProperties.at(i).getCommonDegree()) {
                maxPairs.append(pairsProperties.at(i));
            }
        }

        /*[5] choose the correct max pair from the found list */
        CoveringPairHolder maxPair;

        if(maxPairs.size() == 1) {

            maxPair = maxPairs.at(0);

            AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append("\r\n" + kPairsWithSameMaxDegreeStr +
                                                                                QString::number(maxPairs.at(0).getCoverintPair().first + 1) + " " +
                                                                                QString::number(maxPairs.at(0).getCoverintPair().second + 1));

        } else if (maxPairs.size() > 1) {

            AlgorithmPropertiesSingleton::getInstance().getNumbeOfOperationsRef() += maxPairs.size();
            AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append("\r\n" + kPairsWithSameMaxDegreeStr);

            for(int i = 0; i < maxPairs.size(); ++i) {

                AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append(QString::number(maxPairs.at(i).getCoverintPair().first + 1) + "-" +
                                                                                    QString::number(maxPairs.at(i).getCoverintPair().second + 1) + " ");
            }

            //choose firstly pair without adjacent vertices
            for(int i = 0; i < maxPairs.size(); ++i) {
                if(graphsops::areVerticesAdjacent(maxPairs.at(i).getCoverintPair().first,
                                                  maxPairs.at(i).getCoverintPair().second, graph) == false ||
                        i == (maxPairs.size() - 1)) {
                    maxPair = maxPairs.at(i);
                    break;
                }
            }

        } else {
            return mvc;
        }

        /*[6] multiply graph equation by found vertices */
        int chosenVertex;

        if(boost::out_degree(graphsops::getVertexAtIndexFromPropertyMap(maxPair.getCoverintPair().first, baseGraph), baseGraph) >
                boost::out_degree(graphsops::getVertexAtIndexFromPropertyMap(maxPair.getCoverintPair().second, baseGraph), baseGraph)){
            chosenVertex = maxPair.getCoverintPair().first;
        } else {
            chosenVertex = maxPair.getCoverintPair().second;
        }

        QString outputStr = "\r\n\r\n" + kChosenVertexStr;

        mvc.append(chosenVertex);

        outputStr += QString::number(chosenVertex + 1) + " ";

        vertex_desc_t vertexForRemoving;
        vertexForRemoving = graphsops::getVertexAtIndexFromPropertyMap(chosenVertex, graph);

        clear_vertex(vertexForRemoving, graph);
        remove_vertex(vertexForRemoving, graph);


        outputStr += "\r\n";
        AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append(outputStr);
    }

    return mvc;
}

}
