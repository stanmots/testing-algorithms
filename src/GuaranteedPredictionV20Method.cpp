#include "GuaranteedPredictionV20Method.h"
#include "FunctionPropertiesHolder.h"
#include "AlgorithmPropertiesSingleton.h"
#include "GraphsOperationsHelper.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

namespace graphsalgs {

//global minimum vertex cover container
QList<int> GuaranteedPredictionV20Method_MVC;

QList<int> findMVCWithGuaranteedPredictionV20Method(UndirectedGraphType graph) {

    static const QString kStepsStr = QObject::tr("Step");
    static const QString kChosenFunctionStr = QObject::tr("Chosen function: ");

    static quint32 stepsCount = 0;

    ++stepsCount;

    /*[1] check whether we need to stop the algorithm */
    if( num_edges(graph) == 0 ) {
        return GuaranteedPredictionV20Method_MVC;
    }

    AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append("\r\n" + kStepsStr + " " + QString::number(stepsCount) + "\r\n\r\n");
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

            graphsops::deleteAdjacentVertices(vertexForRemoving, tempGraph);

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
                        isFunctionProcessed = true;
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

            QString independendEdgesStr;

            if(independendEdgesCount > 1) {
                gamma += 1 + independendEdgesCount - maxFrequency;

                std::vector<std::pair<int, int> > sortedEdges = graphsops::getSortedEdges(tempGraph);

                for (quint32 i = 0; i < sortedEdges.size(); ++i) {
                    independendEdgesStr.append("(" + QString::number(sortedEdges[i].first + 1) + " v " + QString::number(sortedEdges[i].second + 1) + ")");
                }
            }

            AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append("f" + QString::number(indexOfCurrentVertex + 1) + "= ");

            QList<int> sortedVariables = variablesOutsideParentheses;
            qSort(sortedVariables.begin(), sortedVariables.end());

            for(int i = 0; i < sortedVariables.size(); ++i) {
                AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append(QString::number(sortedVariables[i] + 1) + " ");
            }

            AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append(independendEdgesStr);
            AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append("\r\nGamma= " + QString::number(gamma) + "\r\n\r\n");

            /*[3.4] save current function properties */
            currentFunctionProperties.setGamma(gamma);
            currentFunctionProperties.setParenthesesCount(independendEdgesCount);
            currentFunctionProperties.setVariablesOutsideParentheses(variablesOutsideParentheses);

            std::pair<quint32, FunctionPropertiesHolder> currentFunction = std::make_pair(indexOfCurrentVertex, currentFunctionProperties);
            graphsFunctions.append(currentFunction);
        }
    }

    /*[4] search for the functions with the minimum gamma */
    if(graphsFunctions.size() > 0) {

        quint32 minGamma = graphsFunctions[0].second.getGamma();
        QList<int> indices;
        indices.append(0);

        AlgorithmPropertiesSingleton::getInstance().getNumbeOfOperationsRef() += graphsFunctions.size();

        for(int i = 1; i < graphsFunctions.size(); ++i) {

            if(graphsFunctions[i].second.getGamma() < minGamma) {
                minGamma = graphsFunctions[i].second.getGamma();
                indices.clear();
                indices << i;
            } else if(graphsFunctions[i].second.getGamma() == minGamma) {
                indices << i;
            }
        }

        /*[4.1] check if we already have MVC */
        for(int i = 0; i < indices.size(); ++i) {

            if(graphsFunctions[indices[i]].second.getParenthesesCount() == 0) {

                //found MVC
                GuaranteedPredictionV20Method_MVC.append(graphsFunctions[indices[i]].second.getVariablesOutsideParentheses());

                AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append(kChosenFunctionStr + "f" + QString::number(graphsFunctions[indices[i]].first + 1) + "\r\n");

                stepsCount = 0;

                return GuaranteedPredictionV20Method_MVC;
            }
        }

        /*[4.2] choose random function and remove all covered vertices */
        GuaranteedPredictionV20Method_MVC.append(graphsFunctions[indices[0]].second.getVariablesOutsideParentheses());
        quint32 indexOfMainVertex = graphsFunctions[indices[0]].first;
        vertex_desc_t mainVertex = graphsops::getVertexAtIndexFromPropertyMap(indexOfMainVertex, graph);
        clear_vertex(mainVertex, graph);
        remove_vertex(mainVertex, graph);

        for(int i = 0; i < graphsFunctions[indices[0]].second.getVariablesOutsideParentheses().size(); ++i) {
            vertex_desc_t currentCoveredVertex = graphsops::getVertexAtIndexFromPropertyMap(graphsFunctions[indices[0]].second.getVariablesOutsideParentheses()[i], graph);
            clear_vertex(currentCoveredVertex, graph);
            remove_vertex(currentCoveredVertex, graph);
        }

        AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append(kChosenFunctionStr + "f" + QString::number(graphsFunctions[indices[0]].first + 1) + "\r\n");

        /*[4.3] recursive call */
        return findMVCWithGuaranteedPredictionV20Method(graph);

    } else {
        //empty graph
        return GuaranteedPredictionV20Method_MVC;
    }

    return GuaranteedPredictionV20Method_MVC;
}

}
