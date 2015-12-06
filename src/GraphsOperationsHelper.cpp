#include "GraphsOperationsHelper.h"
#include "AlgorithmPropertiesSingleton.h"

#include <QString>

namespace graphsops {

std::vector<std::pair<int, int> > getSortedEdges(const UndirectedGraphType& graph) {

    edge_iter_t edgeIterBegin, edgeIterEnd;

    tie(edgeIterBegin, edgeIterEnd) = edges(graph);

    std::vector<std::pair<int, int> > sortedEdges;

    for (; edgeIterBegin != edgeIterEnd; ++edgeIterBegin)  {
        const vertex_desc_t targetVertex = target(*edgeIterBegin, graph);
        const quint32 targetVertexIndex = graphsops::getIndexOfVertex(targetVertex, graph);

        const vertex_desc_t sourceVertex = source(*edgeIterBegin, graph);
        const quint32 sourceVertexIndex = graphsops::getIndexOfVertex(sourceVertex, graph);

        std::pair<int, int> edge;
        if(sourceVertexIndex < targetVertexIndex) {
            edge = std::make_pair(sourceVertexIndex, targetVertexIndex);
        } else if(sourceVertexIndex > targetVertexIndex) {
            edge = std::make_pair(targetVertexIndex, sourceVertexIndex);
        }

        sortedEdges.push_back(edge);
    }

    std::sort(sortedEdges.begin(), sortedEdges.end());

    return sortedEdges;
}
bool DoesEachVertexHaveSameDegree (const UndirectedGraphType& g) {

    vertex_iter_t vertexItBeginH, vertexItEndH, next;
    boost::tie(vertexItBeginH, vertexItEndH) = boost::vertices(g);
    bool isHPathFound = true;

    if(vertexItBeginH != vertexItEndH ) {
        const quint32 baseDegree = boost::out_degree(*vertexItBeginH, g);
        ++vertexItBeginH;

        for (next = vertexItBeginH; vertexItBeginH != vertexItEndH; vertexItBeginH = next) {
            ++next;

            const quint32 currentDegree = boost::out_degree(*vertexItBeginH, g);

            if(currentDegree != baseDegree) {
                isHPathFound = false;
                break;
            }
        }
    } else {
        isHPathFound = false;
    }

    return  isHPathFound;
}

quint32 removeVertexWithMaxDegree(UndirectedGraphType &graph) {

    quint32 indexOfVertexWithMaxFrequency = graphsops::getMaxDegreeVertexIndex(graph);
    vertex_desc_t maxFreqVertexForRemoving;
    maxFreqVertexForRemoving = graphsops::getVertexAtIndexFromPropertyMap(indexOfVertexWithMaxFrequency, graph);

    clear_vertex(maxFreqVertexForRemoving, graph);
    remove_vertex(maxFreqVertexForRemoving, graph);

    return indexOfVertexWithMaxFrequency;
}

void deleteVerticesWithoutEdges (UndirectedGraphType& graph) {

    vertex_iter_t vertexItBegin, vertexItEnd, next;
    boost::tie(vertexItBegin, vertexItEnd) = vertices(graph);

    for (next = vertexItBegin; vertexItBegin != vertexItEnd; vertexItBegin = next) {
        ++next;

        const quint32 currentDegree = boost::out_degree(*vertexItBegin, graph);
        if(currentDegree == 0) {
            remove_vertex(*vertexItBegin, graph);
        }
    }

}

void clearAdjacentVertices (const vertex_desc_t& vertex, UndirectedGraphType& graph) {
    adj_iter_t adjItBegin, adjItEnd, next;

    tie(adjItBegin, adjItEnd) = adjacent_vertices(vertex, graph);

    for (next = adjItBegin; adjItBegin != adjItEnd; adjItBegin = next) {
        ++next;
        clear_vertex(*adjItBegin, graph);
        //remove_vertex(*adjItBegin, graph);
    }
}

void addHangingVerticesToCover (QList<int>& vertexCoverList, UndirectedGraphType& graph) {

    if( num_edges(graph) == 0 ) {
        return;
    }

    static const QString kHangingVerticesStr = QObject::tr("Found hanging vertex: ");
    bool isNeedAnotherPass = true;
    vertex_iter_t vertexItBegin, vertexItEnd, next;
    adj_iter_t adjItBegin, adjItEnd;

    while (isNeedAnotherPass) {

        isNeedAnotherPass = false;

        boost::tie(vertexItBegin, vertexItEnd) = vertices(graph);

        for (next = vertexItBegin; vertexItBegin != vertexItEnd; vertexItBegin = next) {
            ++next;
            const quint32 vertexDegree = out_degree(*vertexItBegin, graph);

            switch(vertexDegree) {
            case NO_INCIDENT_EDGES:
                remove_vertex(*vertexItBegin, graph);
                break;
            case ONE_INCIDENT_EDGE:
                isNeedAnotherPass = true;
                tie(adjItBegin, adjItEnd) = adjacent_vertices(*vertexItBegin, graph);
                vertexCoverList.append(getIndexOfVertex(*adjItBegin, graph));

                AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().append("\r\n" + kHangingVerticesStr + QString::number(getIndexOfVertex(*vertexItBegin, graph) + 1));

                clear_vertex(*adjItBegin, graph);
                remove_vertex(*vertexItBegin, graph);
                break;
            }
        }
    }

}

std::pair<quint32, quint32> getPairOfVerticesWithMaxDegree(const UndirectedGraphType& graph) {

    vertex_iter_t vertexItBegin, vertexItEnd, next;
    boost::tie(vertexItBegin, vertexItEnd) = vertices(graph);
    quint32 maxDegree = 0;
    std::pair<quint32, quint32> maxPair = std::make_pair(-1, -1);
    std::vector<std::pair<quint32, quint32> > processedVertices;

    for (next = vertexItBegin; vertexItBegin != vertexItEnd; vertexItBegin = next) {
        ++next;

        const quint32 baseVertexIndex = getIndexOfVertex(*vertexItBegin, graph);
        const quint32 baseVertexDegree = out_degree(*vertexItBegin, graph);
        adj_iter_t adjVerItBegin, adjVerItEnd;

        tie(adjVerItBegin, adjVerItEnd) = boost::adjacent_vertices(*vertexItBegin, graph);
        for (; adjVerItBegin != adjVerItEnd; ++adjVerItBegin)  {
            const quint32 adjVertexIndex = getIndexOfVertex(*adjVerItBegin, graph);

            if(std::find_if(processedVertices.begin(),processedVertices.end(), graphsops::MatchPairs(baseVertexIndex, adjVertexIndex))
                    == processedVertices.end()) {

                std::pair<quint32, quint32> currentPair = std::make_pair(baseVertexIndex, adjVertexIndex);
                processedVertices.push_back(currentPair);

                const quint32 adjVertexDegree = out_degree(*adjVerItBegin, graph);
                quint32 commonDegree = adjVertexDegree + baseVertexDegree;

                if( commonDegree > maxDegree) {
                    maxDegree = commonDegree;
                    maxPair = std::make_pair(baseVertexIndex, adjVertexIndex);
                }
            }
        }
    }

    return maxPair;
}

std::vector<std::pair<quint32, quint32> > getEquationOfGraph(const UndirectedGraphType& graph) {
    vertex_iter_t vertexItBegin, vertexItEnd, next;
    boost::tie(vertexItBegin, vertexItEnd) = vertices(graph);
    std::vector<std::pair<quint32, quint32> > processedVertices;

    for (next = vertexItBegin; vertexItBegin != vertexItEnd; vertexItBegin = next) {
        ++next;

        const quint32 baseVertexIndex =getIndexOfVertex(*vertexItBegin, graph);

        adj_iter_t adjVerItBegin, adjVerItEnd;
        tie(adjVerItBegin, adjVerItEnd) = boost::adjacent_vertices(*vertexItBegin, graph);

        for (; adjVerItBegin != adjVerItEnd; ++adjVerItBegin)  {
            const quint32 adjVertexIndex = getIndexOfVertex(*adjVerItBegin, graph);

            if(std::find_if(processedVertices.begin(),processedVertices.end(), graphsops::MatchPairs(baseVertexIndex, adjVertexIndex))
                    == processedVertices.end()) {
                std::pair<quint32, quint32> currentPair = std::make_pair(baseVertexIndex, adjVertexIndex);
                processedVertices.push_back(currentPair);
            }
        }
    }

    return processedVertices;
}

PredictionStorage getZeroPairCase(std::pair<quint32, quint32> testPair, UndirectedGraphType graph) {

    //clear two vertices in the test pair
    vertex_desc_t firstVertex = getVertexAtIndexFromPropertyMap(testPair.first, graph);
    clear_vertex(firstVertex, graph);
    remove_vertex(firstVertex, graph);

    vertex_desc_t secondVertex = getVertexAtIndexFromPropertyMap(testPair.second, graph);
    clear_vertex(secondVertex, graph);
    remove_vertex(secondVertex, graph);

    //save properties of the current graph
    QList<int> partialVertexCover;
    partialVertexCover << testPair.first <<testPair.second;

    graphsops::addHangingVerticesToCover(partialVertexCover, graph);

    std::vector<std::pair<quint32, quint32> > equationOfGraph = getEquationOfGraph(graph);
    PredictionStorage currentStorage(equationOfGraph, equationOfGraph.size(), partialVertexCover, graph);

    return currentStorage;
}

PredictionStorage getUnityPairCase(const quint32 zeroVertex, const quint32 unityVertex, UndirectedGraphType graph) {
    //process zero vertex
    QSet<int> partialVertexCover;
    partialVertexCover << zeroVertex;

    vertex_desc_t vertexForRemoving = getVertexAtIndexFromPropertyMap(zeroVertex, graph);
    clear_vertex(vertexForRemoving, graph);
    remove_vertex(vertexForRemoving, graph);

    //process unity vertex
    vertex_desc_t predictionVertex = getVertexAtIndexFromPropertyMap(unityVertex, graph);

    adj_iter_t adjVerItBegin, adjVerItEnd, next;
    tie(adjVerItBegin, adjVerItEnd) = boost::adjacent_vertices(predictionVertex, graph);

    for (next = adjVerItBegin; adjVerItBegin != adjVerItEnd; adjVerItBegin = next)  {
        ++next;

        const quint32 adjVertexIndex = getIndexOfVertex(*adjVerItBegin, graph);
        partialVertexCover << adjVertexIndex;

        clear_vertex(*adjVerItBegin, graph);
    }

    //remove unity vertex at the end
    clear_vertex(predictionVertex, graph);
    remove_vertex(predictionVertex, graph);

    deleteVerticesWithoutEdges(graph);

    //save properties of the current graph
    QList<int> partialVertexCoverList = partialVertexCover.toList();

    graphsops::addHangingVerticesToCover(partialVertexCoverList, graph);

    std::vector<std::pair<quint32, quint32> > equationOfGraph = getEquationOfGraph(graph);
    PredictionStorage currentStorage(equationOfGraph, equationOfGraph.size(), partialVertexCoverList, graph);

    return currentStorage;
}

}
