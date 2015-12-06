#include "ShortestPathMethod.h"
#include "FunctionPropertiesHolder.h"
#include "AlgorithmPropertiesSingleton.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

namespace graphsalgs {

QList<int> getMinPathsIndices(const QList<QSet<int> > paths);

QList<int> findMVCWithShortestPathMethod(UndirectedGraphType graph) {

    QList<int> MVC;

    /*[1] check whether we need to stop the algorithm */
    if( num_edges(graph) == 0 ) {
        return MVC;
    }

    const QString kChosenFunctionStr = QObject::tr("Chosen function: ");

    AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append("\r\n");
    AlgorithmPropertiesSingleton::getInstance().getNumbeOfOperationsRef() = 0;

    /*[2] declare the required variables */
    vertex_iter_t vertexItBegin, vertexItEnd, next;

    QList<std::pair<quint32, FunctionPropertiesHolder> > graphsFunctions;

    /*[3] start analysis of each vertex of the graph */
    boost::tie(vertexItBegin, vertexItEnd) = vertices(graph);

    for (next = vertexItBegin; vertexItBegin != vertexItEnd; vertexItBegin = next) {
        ++next;
        FunctionPropertiesHolder currentFunctionProperties;

        /*[3.1] search for all the adjacent verteces,
        they represent the variables outside parenthesis of the current graph's function
        */
        QList<int> variablesOutsideParentheses = graphsops::getAdjacentVertices(*vertexItBegin, graph).toList();

        int indexOfCurrentVertex = graphsops::getIndexOfVertex(*vertexItBegin, graph);

        if(variablesOutsideParentheses.size() > 0) {

            bool isFunctionProcessed = false;
            UndirectedGraphType tempGraph = graph;

            //with multiset we can quicly get a total number of occurrences of a vertex
            //Qt doesn't have multiset container, so we use std version
            std::multiset<quint32> independentEdges;
            quint32 independendEdgesCount = 0;
            quint32 maxFrequency = 0;
            quint32 indexOfVertexWithMaxFrequency = 0;

            vertex_desc_t vertexForRemoving;
            vertexForRemoving = graphsops::getVertexAtIndexFromPropertyMap(indexOfCurrentVertex, tempGraph);

            graphsops::clearAdjacentVertices(vertexForRemoving, tempGraph);

            while(isFunctionProcessed == false) {

                independendEdgesCount = 0;
                independentEdges.clear();

                AlgorithmPropertiesSingleton::getInstance().getNumbeOfOperationsRef() += num_edges(tempGraph);
                graphsops::addHangingVerticesToCover(variablesOutsideParentheses, tempGraph);

                /*[3.2] search for independent edges that represent parenthesis */
                graphsops::findIndependentEdges(independentEdges, independendEdgesCount, tempGraph);

                maxFrequency = 0;
                indexOfVertexWithMaxFrequency = 0;

                if(independendEdgesCount == 0) {
                    isFunctionProcessed = true;
                } else if(independendEdgesCount == 1) {
                    indexOfVertexWithMaxFrequency = *(independentEdges.begin());
                    variablesOutsideParentheses.append(indexOfVertexWithMaxFrequency);
                    independendEdgesCount = 0;
                    isFunctionProcessed = true;
                } else if(independendEdgesCount > 1) {

                    /*[3.2.1] search for vertex with maximum frequency */
                    quint32 sameVerticesCount = 0;
                    QSet<quint32> uniqueValues;

                    AlgorithmPropertiesSingleton::getInstance().getNumbeOfOperationsRef() +=independendEdgesCount;

                    for(std::multiset<quint32>::iterator it = independentEdges.begin(); it != independentEdges.end(); ++it) {
                        if(uniqueValues.contains(*it) == false) {

                            uniqueValues.insert(*it);

                            if(independentEdges.count(*it) > maxFrequency) {
                                maxFrequency = independentEdges.count(*it);
                                indexOfVertexWithMaxFrequency = *it;
                                sameVerticesCount = 0;
                            } else if(independentEdges.count(*it) == maxFrequency) {
                                ++sameVerticesCount;
                            }
                        }
                    }

                    if(sameVerticesCount > 0) {
                        /*[3.2.2] search for shortest path */
                        std::vector<std::pair<int, int> > sortedEdges = graphsops::getSortedEdges(tempGraph);
                        AlgorithmPropertiesSingleton::getInstance().getNumbeOfOperationsRef() +=sortedEdges.size();

                        QList<QSet<int> > currentFirstStage, currentSecondStage;

                        QSet<int> initialFirstStageSet;
                        initialFirstStageSet.insert(sortedEdges[0].first);
                        QSet<int> initialSecondStageSet;
                        initialSecondStageSet.insert(sortedEdges[0].second);

                        currentFirstStage.append(initialFirstStageSet);
                        currentSecondStage.append(initialSecondStageSet);

                        for (quint32 i = 1; i < sortedEdges.size(); ++i) {

                            QList<QSet<int> > newFirstStage, newSecondStage;

                            /*[3.2.2.1] get minimum indices with paths from stages */
                            QList<int> firstStageMinPathsIndices = getMinPathsIndices(currentFirstStage);
                            QList<int> secondStageMinPathsIndices = getMinPathsIndices(currentSecondStage);


                            /*[3.2.2.2] form the new paths for the new stages */
                            for(int x = 0; x < firstStageMinPathsIndices.size(); ++x) {
                                QSet<int> firstStageSet1;
                                QSet<int> secondStageSet1;

                                //first stage vertex + previous first stage min path
                                firstStageSet1.insert(sortedEdges[i].first);
                                firstStageSet1.unite(currentFirstStage[firstStageMinPathsIndices[x]]);

                                //second stage vertex + previous first stage min path
                                secondStageSet1.insert(sortedEdges[i].second);
                                secondStageSet1.unite(currentFirstStage[firstStageMinPathsIndices[x]]);

                                newFirstStage.append(firstStageSet1);
                                newSecondStage.append(secondStageSet1);
                            }

                            for(int x = 0; x < secondStageMinPathsIndices.size(); ++x) {
                                QSet<int> firstStageSet2;
                                QSet<int> secondStageSet2;

                                //first stage vertex + previous second stage min path
                                firstStageSet2.insert(sortedEdges[i].first);
                                firstStageSet2.unite(currentSecondStage[secondStageMinPathsIndices[x]]);

                                //second stage vertex + previous second stage min path
                                secondStageSet2.insert(sortedEdges[i].second);
                                secondStageSet2.unite(currentSecondStage[secondStageMinPathsIndices[x]]);

                                newFirstStage.append(firstStageSet2);
                                newSecondStage.append(secondStageSet2);
                            }

                            currentFirstStage = newFirstStage;
                            currentSecondStage = newSecondStage;
                        }

                        /*[3.2.2.3] find the path with the minimum size */
                        QList<int> finalFirstStageMinPathsIndices = getMinPathsIndices(currentFirstStage);
                        QList<int> finalSecondStageMinPathsIndices = getMinPathsIndices(currentSecondStage);

                        QSet<int> minPath;

                        //choose first min index (0)
                        if(currentFirstStage[finalFirstStageMinPathsIndices[0]].size() > currentSecondStage[finalSecondStageMinPathsIndices[0]].size()) {
                            minPath = currentSecondStage[finalSecondStageMinPathsIndices[0]];
                        } else {
                            minPath = currentFirstStage[finalFirstStageMinPathsIndices[0]];
                        }

                        //remove min group
                        foreach (const int &value, minPath) {
                            variablesOutsideParentheses.append(value);

                            vertex_desc_t vertexFromShortestPath;
                            vertexFromShortestPath = graphsops::getVertexAtIndexFromPropertyMap(value, tempGraph);

                            clear_vertex(vertexFromShortestPath, tempGraph);
                            remove_vertex(vertexFromShortestPath, tempGraph);
                        }

                    } else {

                        variablesOutsideParentheses.append(indexOfVertexWithMaxFrequency);

                        vertex_desc_t maxFreqVertexForRemoving;
                        maxFreqVertexForRemoving = graphsops::getVertexAtIndexFromPropertyMap(indexOfVertexWithMaxFrequency, tempGraph);

                        clear_vertex(maxFreqVertexForRemoving, tempGraph);
                        remove_vertex(maxFreqVertexForRemoving, tempGraph);
                    }
                }
            }

            /*[3.3] calculating gamma */
            quint32 gamma = variablesOutsideParentheses.size();

            AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append("f" + QString::number(indexOfCurrentVertex + 1) + "= ");

            QList<int> sortedVariables = variablesOutsideParentheses;
            qSort(sortedVariables.begin(), sortedVariables.end());

            for(int i = 0; i < sortedVariables.size(); ++i) {
                AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append(QString::number(sortedVariables[i] + 1) + " ");
            }

            AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append("\r\nGamma= " + QString::number(gamma) + "\r\n\r\n");

            /*[3.4] save current function properties */
            currentFunctionProperties.setGamma(gamma);
            currentFunctionProperties.setVariablesOutsideParentheses(variablesOutsideParentheses);

            std::pair<quint32, FunctionPropertiesHolder> currentFunction = std::make_pair(indexOfCurrentVertex, currentFunctionProperties);
            graphsFunctions.append(currentFunction);
        }
    }

    /*[4] search for the functions with the minimum gamma */
    quint32 minGamma = graphsFunctions[0].second.getGamma();
    quint32 minIndex = 0;

    AlgorithmPropertiesSingleton::getInstance().getNumbeOfOperationsRef() += graphsFunctions.size();

    for(int i = 1; i < graphsFunctions.size(); ++i) {

        if(graphsFunctions[i].second.getGamma() < minGamma) {
            minGamma = graphsFunctions[i].second.getGamma();
            minIndex = i;
        }
    }

    MVC.append(graphsFunctions[minIndex].second.getVariablesOutsideParentheses());

    AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append(kChosenFunctionStr + "f" + QString::number(graphsFunctions[minIndex].first + 1) + "\r\n");

    return MVC;
}

QList<int> getMinPathsIndices(const QList<QSet<int> > paths) {

    int minPath = paths[0].size();
    QList<int> minPathsIndices; minPathsIndices << 0;

    for (int i = 1; i < paths.size(); ++i) {
        if(paths[i].size() < minPath) {
            minPath = paths[i].size();

            minPathsIndices.clear();
            minPathsIndices << i;

        } else if(paths[i].size() == minPath) {

            minPathsIndices << i;
        }
    }

    return minPathsIndices;
}

}
