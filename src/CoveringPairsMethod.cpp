#include "CoveringPairsMethod.h"
#include "CoveringPairHolder.h"
#include "AlgorithmPropertiesSingleton.h"

namespace graphsalgs {

QList<int> findMVCWithCoveringPairsMethod(UndirectedGraphType graph) {

    QList<int> mvc;

    AlgorithmPropertiesSingleton::getInstance().getNumbeOfOperationsRef() = 0;

    static const QString kStepsStr = QObject::tr("Step");
    static const QString kChosenPairStr = QObject::tr("Chosen vertices: ");
    static const QString kPairsWithSameMaxDegreeStr = QObject::tr("Pairs with same max frequency: ");
    static const QString kPairsWithVertexStr = QObject::tr("Pairs with vertex ");
    static const QString kPairsAndDegreeOutputStr = QObject::tr("Pairs of vertices with the corresponding degree: ");
    static const QString kSameDegreePairsAndDegreeOutputStr = QObject::tr("Pairs of vertices with the corresponding degree (from the set with max frequency vertices): ");

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
            std::multiset<int> vertexFrequencies;
            std::set<int> uniqueVertices;

            AlgorithmPropertiesSingleton::getInstance().getNumbeOfOperationsRef() += maxPairs.size();
            AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append("\r\n" + kPairsWithSameMaxDegreeStr);

            for(int i = 0; i < maxPairs.size(); ++i) {

                AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append(QString::number(maxPairs.at(i).getCoverintPair().first + 1) + "-" +
                                                                                    QString::number(maxPairs.at(i).getCoverintPair().second + 1) + " ");

                vertexFrequencies.insert(maxPairs.at(i).getCoverintPair().first);
                vertexFrequencies.insert(maxPairs.at(i).getCoverintPair().second);
                uniqueVertices.insert(maxPairs.at(i).getCoverintPair().first);
                uniqueVertices.insert(maxPairs.at(i).getCoverintPair().second);
            }

            /*[5][1] form pairs of found vertices */
            AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append("\r\n\r\n" + kSameDegreePairsAndDegreeOutputStr + "\r\n");


            std::set<int>::const_iterator uniqueVerticesItBegin = uniqueVertices.begin();
            std::set<int>::const_iterator nextIter, innerNextIter;

            QList<CoveringPairHolder> samePairsProperties;

            for (nextIter = uniqueVerticesItBegin; uniqueVerticesItBegin != uniqueVertices.end(); uniqueVerticesItBegin = nextIter) {
                ++nextIter;
                innerNextIter = nextIter;

                const quint32 baseVertexIndex =  *uniqueVerticesItBegin;
                const quint32 baseVertexFrequency = vertexFrequencies.count(*uniqueVerticesItBegin);

                if(nextIter != uniqueVertices.end())
                    AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append("\r\n" + kPairsWithVertexStr + QString::number(baseVertexIndex + 1) + ": ");


                for (uniqueVerticesItBegin = innerNextIter; uniqueVerticesItBegin != uniqueVertices.end(); uniqueVerticesItBegin = innerNextIter) {
                    ++innerNextIter;

                    const quint32 pairedVertexIndex =  *uniqueVerticesItBegin;
                    const quint32 pairedVertexFrequency = vertexFrequencies.count(*uniqueVerticesItBegin);

                    AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append(QString::number(baseVertexIndex + 1) + " " + QString::number(pairedVertexIndex + 1) + "(" +
                                                                                        QString::number(baseVertexFrequency + pairedVertexFrequency) + ") ");

                    CoveringPairHolder currentHolder(baseVertexFrequency,
                                                     pairedVertexFrequency,
                                baseVertexFrequency + pairedVertexFrequency,
                                                     std::make_pair(baseVertexIndex, pairedVertexIndex));
                    samePairsProperties.append(currentHolder);
                }
            }

            /*[5][2] find pair with same frequency */
            quint32 maxPairFreq = samePairsProperties.at(0).getCommonDegree();
            maxPair = samePairsProperties.at(0);

            QList<CoveringPairHolder> maxSPairs;
            maxSPairs.append(samePairsProperties.at(0));

            AlgorithmPropertiesSingleton::getInstance().getNumbeOfOperationsRef() += samePairsProperties.size();

            for(int i = 1; i < samePairsProperties.size(); ++i) {

                if(maxPairFreq < samePairsProperties.at(i).getCommonDegree()) {
                    maxPairFreq = samePairsProperties.at(i).getCommonDegree();

                    maxSPairs.clear();
                    maxSPairs.append(samePairsProperties.at(i));

                } else if(maxPairFreq == samePairsProperties.at(i).getCommonDegree()) {
                    maxSPairs.append(samePairsProperties.at(i));
                }
            }

            AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append("\r\n\r\n" + kPairsWithSameMaxDegreeStr);

            foreach(const CoveringPairHolder &sp, maxSPairs) {
                AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append(QString::number(sp.getCoverintPair().first + 1) + "-" +
                                                                                    QString::number(sp.getCoverintPair().second + 1) + " ");
            }

            /*[5][3] choose pair */
            if(maxSPairs.size() == 1) {

                maxPair = maxSPairs.at(0);

            } else if (maxSPairs.size() > 1) {

                //choose firstly pair without adjacent vertices
                for(int i = 0; i < maxSPairs.size(); ++i) {
                    if(graphsops::areVerticesAdjacent(maxSPairs.at(i).getCoverintPair().first,
                                                      maxSPairs.at(i).getCoverintPair().second, graph) == false ||
                            i == (maxSPairs.size() - 1)) {
                        maxPair = maxSPairs.at(i);
                        break;
                    }
                }
            }

        } else {
            return mvc;
        }

        /*[6] multiply graph equation by found vertices */
        QList<int> foundVertices;

//        if(graphsops::areVerticesAdjacent(maxPair.getCoverintPair().first,
//                                          maxPair.getCoverintPair().second, graph) == true) {
//                if(maxPair.getFirstVertexDegree() > maxPair.getSecondVertexDegree()){
//                    foundVertices.append(maxPair.getCoverintPair().first);
//                } else {
//                    foundVertices.append(maxPair.getCoverintPair().second);
//                }
//        } else {
//            foundVertices.append(maxPair.getCoverintPair().first);
//            foundVertices.append(maxPair.getCoverintPair().second);
//        }

        if(maxPair.getFirstVertexDegree() > maxPair.getSecondVertexDegree()){
            foundVertices.append(maxPair.getCoverintPair().first);
        } else {
            foundVertices.append(maxPair.getCoverintPair().second);
        }

        QString outputStr = "\r\n\r\n" + kChosenPairStr;

        for(int i = 0; i < foundVertices.size(); ++i) {

            mvc.append(foundVertices.at(i));

            outputStr += QString::number(foundVertices.at(i) + 1) + " ";

            vertex_desc_t vertexForRemoving;
            vertexForRemoving = graphsops::getVertexAtIndexFromPropertyMap(foundVertices.at(i), graph);

            clear_vertex(vertexForRemoving, graph);
            remove_vertex(vertexForRemoving, graph);
        }

        outputStr += "\r\n";
        AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append(outputStr);
    }

    return mvc;
}

}
