#ifndef GRAPHSOPERATIONSHELPER_H
#define GRAPHSOPERATIONSHELPER_H

#include <QSet>
#include <QList>
#include <QtDebug>

#include "GraphsTypesHelper.h"
#include "PredictionStorage.h"

#include <QObject>
#include <QString>

namespace graphsops {

struct MatchPairs
{
    MatchPairs(const quint32 f, const quint32 s) : firstValue(f),
        secondValue(s) {}

    bool operator()(const std::pair<quint32, quint32>& p) {
        return ((p.first == firstValue && p.second == secondValue) ||
                (p.first == secondValue && p.second == firstValue));
    }

    const quint32 firstValue;
    const quint32 secondValue;
};

struct SortEdgesPredicate
{
    bool operator()(const std::pair<int, int>& left,
                    const std::pair<int, int>& right) {
        return ((left.first < right.first) ||
                (left.first == right.first && left.second < right.second));
    }
};

bool DoesEachVertexHaveSameDegree (const UndirectedGraphType& graph);
void deleteVerticesWithoutEdges (UndirectedGraphType& graph);
void clearAdjacentVertices(const vertex_desc_t& vertex, UndirectedGraphType& graph);
void addHangingVerticesToCover (QList<int>& vertexCoverList, UndirectedGraphType& graph);
quint32 removeVertexWithMaxDegree(UndirectedGraphType& graph);
std::pair<quint32, quint32> getPairOfVerticesWithMaxDegree(const UndirectedGraphType& graph);
PredictionStorage getZeroPairCase(std::pair<quint32, quint32> testPair, UndirectedGraphType graph);
PredictionStorage getUnityPairCase(const quint32 zeroVertex, const quint32 unityVertex, UndirectedGraphType graph);
std::vector<std::pair<quint32, quint32> > getEquationOfGraph(const UndirectedGraphType& graph);
std::vector<std::pair<int, int> > getSortedEdges(const UndirectedGraphType& graph);

inline quint32 getIndexOfVertex(const vertex_desc_t& vertex, const UndirectedGraphType& graph) {
    boost::property_map<UndirectedGraphType, boost::vertex_index_t>::const_type indexMap = get(boost::vertex_index, graph);

    return indexMap[vertex];
}

inline vertex_desc_t getVertexAtIndexByPositionShift(const quint32 vertexIndex, const UndirectedGraphType& graph) {
    UndirectedGraphType::vertex_iterator vertexBegin, vertexEnd;
    boost::tie(vertexBegin, vertexEnd) = vertices(graph);
    std::advance (vertexBegin,vertexIndex);

    return *vertexBegin;
}

inline vertex_desc_t getVertexAtIndexFromPropertyMap(const quint32 vertexIndex, const UndirectedGraphType& graph) {
    vertex_iter_t vertexItBegin, vertexItEnd, next;
    boost::tie(vertexItBegin, vertexItEnd) = vertices(graph);
    for (next = vertexItBegin; vertexItBegin != vertexItEnd; vertexItBegin = next) {
        ++next;
        if(getIndexOfVertex(*vertexItBegin, graph) == vertexIndex) {
            return *vertexItBegin;
        }
    }

    //if the vertex was not found - return arbitrary vertex
    qDebug() << "Error! The vertex at specified index was not found!";
    return *(--vertexItBegin);
}

inline QSet<int> getAdjacentVertices(const vertex_desc_t& vertex, const UndirectedGraphType& graph) {
    adj_iter_t adjVerItBegin, adjVerItEnd;
    QSet<int> adjVerticesSet;

    tie(adjVerItBegin, adjVerItEnd) = boost::adjacent_vertices(vertex, graph);
    for (; adjVerItBegin != adjVerItEnd; ++adjVerItBegin)  {
        const quint32 vi = getIndexOfVertex(*adjVerItBegin, graph);
        adjVerticesSet.insert(vi);
    }

    return adjVerticesSet;

}

inline int getMaxDegreeVertexIndex(const UndirectedGraphType &graph) {

    vertex_iter_t vertexItBegin, vertexItEnd, next;
    boost::tie(vertexItBegin, vertexItEnd) = vertices(graph);

    quint32 baseMaxDegree = boost::out_degree(*vertexItBegin, graph);
    quint32 baseVertexIndex =  graphsops::getIndexOfVertex(*vertexItBegin, graph);
    next = vertexItBegin;
    ++next;
    vertexItBegin = next;

    for (; vertexItBegin != vertexItEnd; vertexItBegin = next) {
        ++next;

        const quint32 currentMaxDegree = boost::out_degree(*vertexItBegin, graph);
        const quint32 currentVertexIndex =  graphsops::getIndexOfVertex(*vertexItBegin, graph);

        if(baseMaxDegree < currentMaxDegree) {
            baseMaxDegree = currentMaxDegree;
            baseVertexIndex = currentVertexIndex;
        }
    }

    return baseVertexIndex;
}

inline bool areVerticesAdjacent(const quint32 v1, const quint32 v2, const UndirectedGraphType& graph) {

    QSet<int> adjVerticesOfV1 = graphsops::getAdjacentVertices(getVertexAtIndexFromPropertyMap(v1, graph), graph);

    return adjVerticesOfV1.contains(v2);
}

inline void findIndependentEdges (std::multiset<quint32>& independentEdgesContainer, quint32& edgesCount, const UndirectedGraphType& graph ) {

edge_iter_t edgeIterBegin, edgeIterEnd;

tie(edgeIterBegin, edgeIterEnd) = edges(graph);

for (; edgeIterBegin != edgeIterEnd; ++edgeIterBegin)  {
    const vertex_desc_t targetVertex = target(*edgeIterBegin, graph);
    const quint32 targetVertexIndex = graphsops::getIndexOfVertex(targetVertex, graph);

    const vertex_desc_t sourceVertex = source(*edgeIterBegin, graph);
    const quint32 sourceVertexIndex = graphsops::getIndexOfVertex(sourceVertex, graph);

    independentEdgesContainer.insert(sourceVertexIndex);
    independentEdgesContainer.insert(targetVertexIndex);
    ++edgesCount;
}

}

inline void intializeVertexIndexProperties(UndirectedGraphType& graph) {

    vertex_iter_t i, iend;
    boost::property_map<UndirectedGraphType, boost::vertex_index_t>::type indexMap = get(boost::vertex_index, graph);

    int c = 0;
    for (boost::tie(i, iend) = vertices(graph); i != iend; ++i, ++c) {
        indexMap[*i] = c;
    }
}

}
#endif // GRAPHSOPERATIONSHELPER_H
