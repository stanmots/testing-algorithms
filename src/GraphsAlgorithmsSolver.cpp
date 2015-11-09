#include <QtDebug>
#include <QTime>
#include <QSet>

#include "GraphsAlgorithmsSolver.h"
#include "UnguaranteedPredictionMethod.h"
#include "ConnectedTreeMethod.h"
#include "GuaranteedPredictionMethod.h"
#include "GuaranteedPredictionV20Method.h"
#include "AlgorithmPropertiesSingleton.h"
#include "ShortestPathMethod.h"

using namespace boost;

extern QList<int> graphsalgs::GuaranteedPredictionV20Method_MVC;

GraphsAlgorithmsSolver::GraphsAlgorithmsSolver(QObject *parent) :
    QObject(parent) {}

mersenne_twister_t& GraphsAlgorithmsSolver::getRandomGenerator() {

    static mersenne_twister_t m_randomGenerator(static_cast<unsigned int>(QTime::currentTime().msec()));
    return m_randomGenerator;
}

void GraphsAlgorithmsSolver::intializeVertexIndexProperties(UndirectedGraphType& graph) {

    vertex_iter_t i, iend;
    property_map<UndirectedGraphType, vertex_index_t>::type indexMap = get(vertex_index, graph);

    int c = 0;
    for (boost::tie(i, iend) = vertices(graph); i != iend; ++i, ++c) {
        indexMap[*i] = c;
    }
}

QString GraphsAlgorithmsSolver::getAlgInfo() const {

    return AlgorithmPropertiesSingleton::getInstance().getAlgSteps();
}

quint32 GraphsAlgorithmsSolver::getNumberOfOperations() const {

    return AlgorithmPropertiesSingleton::getInstance().getNumbeOfOperations();
}

QList<int> GraphsAlgorithmsSolver::getArrayOfCoversLengths() const{

    return m_arrayOfCoversLengths;
}

QList<int> GraphsAlgorithmsSolver::getExactSolution() const {

    QList<int> exactSolution;

    if(m_arrayOfCovers.size() > 0) {

        int minSize = m_arrayOfCovers.at(0).size();
        int minIndex = 0;


        for(int i = 1; i < m_arrayOfCovers.size(); ++i) {
            if(m_arrayOfCovers.at(i).size() < minSize) {
                minIndex = i;
                minSize = m_arrayOfCovers.at(i).size();
            }
        }

        exactSolution.append(m_arrayOfCovers.at(minIndex));
    }

    return exactSolution;
}

void GraphsAlgorithmsSolver::clearOldValues() {
    m_arrayOfCovers.clear();
    m_arrayOfCoversLengths.clear();
}

QList<int> GraphsAlgorithmsSolver::findMVCWithStandardAlg(UndirectedGraphType graph) {

    if(num_vertices(graph) < 2) {
        return QList<int>();
    }

    QList<int> minimumVertexCoverList;
    vertex_iter_t vertexItBegin, vertexItEnd, next;
    bool isFoundMinimumVertexCover = false;

    while (!isFoundMinimumVertexCover) {

        /*[1] check if there are vertices with just one incident edge in the graph
        * and add them to the vertex cover */

        graphsops::addHangingVerticesToCover(minimumVertexCoverList, graph);

        /*[2] check if we have already the minimum vertex cover */
        if( num_edges(graph) == 0 ) {
            isFoundMinimumVertexCover = true;
            break;
        }
        /*[3] find the vertex with the most incident edges degree
        * and add it to the vertex cover */
        else {
            vertex_desc_t maxEdgesVertex;
            tie(vertexItBegin, vertexItEnd) = vertices(graph);
            quint32 maxDegree = 0;
            for (next = vertexItBegin; vertexItBegin != vertexItEnd; vertexItBegin = next) {
                ++next;
                const quint32 currentVertexDegree = out_degree(*vertexItBegin, graph);
                if(currentVertexDegree > maxDegree) {
                    maxDegree = currentVertexDegree;
                    maxEdgesVertex = *vertexItBegin;
                }
            }
            minimumVertexCoverList.append(graphsops::getIndexOfVertex(maxEdgesVertex, graph));
            clear_vertex(maxEdgesVertex, graph);
            remove_vertex(maxEdgesVertex, graph);

            if( num_edges(graph) == 0 ) {
                //loop finished
                isFoundMinimumVertexCover = true;
            }
        }
    }

    return minimumVertexCoverList;
}

void GraphsAlgorithmsSolver::getAllVertexCoversOfCurrentGraph(const int currentGraphIndex, const bool isNeedSignals) {
    if(currentGraphIndex < 0 || currentGraphIndex >= m_graphsList.size()) {
        qDebug() << "Error! Cannot get vertex covers, because the graph index is incorrect!";
        return;
    }

    searchForAllVertexCovers(m_graphsList.at(currentGraphIndex), isNeedSignals);
}

void GraphsAlgorithmsSolver::searchForAllVertexCovers(const UndirectedGraphType graph, const bool isNeedSignals)  {


    //need to clear arrays before searching
    m_arrayOfCoversLengths.clear();
    m_arrayOfCovers.clear();

    if(num_vertices(graph) == 0) {
        if(isNeedSignals == true) {
            emit collectingVertexCoversFinished();
        }
        return;
    }

    vertex_iter_t vertexItBegin, vertexItEnd, next;
    UndirectedGraphType currentProcessingGraph;
    tie(vertexItBegin, vertexItEnd) = vertices(graph);
    for (next = vertexItBegin; vertexItBegin != vertexItEnd; vertexItBegin = next) {
        ++next;

        //[0] restore graph
        currentProcessingGraph = graph;
        QList<int> vertexCover;

        /*[1] remove current vertex and all its incident edges from the graph
        and put it to the cover*/
        const quint32 baseVertexIndex = graphsops::getIndexOfVertex(*vertexItBegin, graph);
        vertexCover.append(baseVertexIndex);
        vertex_desc_t baseVertex = graphsops::getVertexAtIndexByPositionShift(baseVertexIndex, currentProcessingGraph);
        clear_vertex(baseVertex, currentProcessingGraph);
        remove_vertex(baseVertex, currentProcessingGraph);

        if(num_edges(currentProcessingGraph) == 0) {
            m_arrayOfCoversLengths.append(vertexCover.length());
            m_arrayOfCovers.append(vertexCover);

            if(isNeedSignals == true) {
                emit foundNewVertexCover(vertexCover);
            }
        } else {

            bool isFoundVertexCover = false;
            while(!isFoundVertexCover) {
                //[2] find vertices with the maximum incident edges
                vertex_iter_t innerVertexItBegin, innerVertexItEnd, innerNext;
                quint32 maxDegree = 0;
                QSet<quint32> maxEdgesVerticesSet;
                tie(innerVertexItBegin, innerVertexItEnd) = vertices(currentProcessingGraph);

                for (innerNext = innerVertexItBegin; innerVertexItBegin != innerVertexItEnd; innerVertexItBegin = innerNext) {
                    ++innerNext;
                    const quint32 currentVertexDegree = out_degree(*innerVertexItBegin, currentProcessingGraph);
                    if(currentVertexDegree > maxDegree) {
                        maxDegree = currentVertexDegree;
                        maxEdgesVerticesSet.clear();
                        maxEdgesVerticesSet.insert(graphsops::getIndexOfVertex(*innerVertexItBegin, currentProcessingGraph));
                    } else if(currentVertexDegree == maxDegree) {
                        maxEdgesVerticesSet.insert(graphsops::getIndexOfVertex(*innerVertexItBegin, currentProcessingGraph));
                    }
                }
                /*[3] if we have more than one vertices with max edges choose that one
                 * which has at least one edge that is not covered by others*/
                vertex_desc_t vertexForRemoving;
                if(maxEdgesVerticesSet.size() > 1) {
                    quint32 oneEdgeNotCoveredVertexIndex;
                    bool foundOneEdgeNotCoveredVertex = false;
                    QSetIterator<quint32> maxEdgesVertIter(maxEdgesVerticesSet);

                    while (maxEdgesVertIter.hasNext() && foundOneEdgeNotCoveredVertex == false) {
                        const quint32 currentOneEdgeNotCoveredVertexIndex = maxEdgesVertIter.next();
                        out_edge_iter_t outEdgeIterBegin, outEdgeIterEnd;
                        tie(outEdgeIterBegin, outEdgeIterEnd) = out_edges(graphsops::getVertexAtIndexFromPropertyMap(currentOneEdgeNotCoveredVertexIndex, currentProcessingGraph),
                                                                          currentProcessingGraph);

                        for (; outEdgeIterBegin != outEdgeIterEnd; ++outEdgeIterBegin)  {
                            const vertex_desc_t targetVertex = target(*outEdgeIterBegin, currentProcessingGraph);
                            const quint32 targetVertexIndex = graphsops::getIndexOfVertex(targetVertex, currentProcessingGraph);
                            if(maxEdgesVerticesSet.contains(targetVertexIndex) == false) {
                                oneEdgeNotCoveredVertexIndex = currentOneEdgeNotCoveredVertexIndex;
                                foundOneEdgeNotCoveredVertex = true;
                                break;
                            }
                        }
                    }

                    if(foundOneEdgeNotCoveredVertex == false) {
                        oneEdgeNotCoveredVertexIndex = *(maxEdgesVerticesSet.begin());
                    }
                    vertex_desc_t oneEdgeNotCoveredVertex = graphsops::getVertexAtIndexFromPropertyMap(oneEdgeNotCoveredVertexIndex, currentProcessingGraph);
                    vertexCover.append(oneEdgeNotCoveredVertexIndex);
                    vertexForRemoving = oneEdgeNotCoveredVertex;
                }
                //[3.1] Otherwise choose an arbitrary vertex
                else {
                    vertex_desc_t arbitraryVertex = graphsops::getVertexAtIndexFromPropertyMap(*(maxEdgesVerticesSet.begin()), currentProcessingGraph);
                    vertexCover.append(*(maxEdgesVerticesSet.begin()));
                    vertexForRemoving = arbitraryVertex;
                }
                clear_vertex(vertexForRemoving, currentProcessingGraph);
                remove_vertex(vertexForRemoving, currentProcessingGraph);

                //[4] check if we have the complete vertex cover
                if(num_edges(currentProcessingGraph) == 0) {
                    m_arrayOfCoversLengths.append(vertexCover.length());
                    m_arrayOfCovers.append(vertexCover);

                    if(isNeedSignals == true) {
                        emit foundNewVertexCover(vertexCover);
                    }
                    isFoundVertexCover = true;
                    break;
                }
            }
        }
    }
   if(isNeedSignals == true) {
       emit collectingVertexCoversFinished();
   }
}

QList<int> GraphsAlgorithmsSolver::solveAlgorithmForCurrentGraph(const quint32 algType, const int currentGraphIndex) const {

    if(currentGraphIndex < 0 || currentGraphIndex >= m_graphsList.size()) {
        qDebug() << "Error! Cannot solve algorithm, because the graph index is incorrect!";
        return QList<int>();
    }

    if(boost::num_vertices(m_graphsList.at(currentGraphIndex)) < 1) {
        qDebug() << "Error! The number of vertices is too small!";
        return QList<int>();
    }

    if(num_vertices(m_graphsList.at(currentGraphIndex)) == 1) {
        QList<int> coverList;
        vertex_iter_t vertexItBegin, vertexItEnd;
        boost::tie(vertexItBegin, vertexItEnd) = vertices(m_graphsList.at(currentGraphIndex));

        coverList.push_back(graphsops::getIndexOfVertex(*vertexItBegin, m_graphsList.at(currentGraphIndex)));

        return coverList;
    }

    AlgorithmPropertiesSingleton::getInstance().clearCommonContainer();

    switch(algType) {
    case 0:
        return graphsalgs::findMVCWithShortestPathMethod(m_graphsList.at(currentGraphIndex));
        break;
    case 1:
        graphsalgs::GuaranteedPredictionV20Method_MVC.clear();
        return graphsalgs::findMVCWithGuaranteedPredictionV20Method(m_graphsList.at(currentGraphIndex));
        break;
    case 2:
        return graphsalgs::findMVCWithUnguaranteedPredictionMethod(m_graphsList.at(currentGraphIndex));
        break;
    case 3:
        return graphsalgs::findMVCWithConnectedTreeMethod(m_graphsList.at(currentGraphIndex));
        break;
    case 4:
        return graphsalgs::findMVCWithGuaranteedPredictionMethod(m_graphsList.at(currentGraphIndex));
        break;
    case 5:
        return findMVCWithStandardAlg(m_graphsList.at(currentGraphIndex));
        break;
    default:
        return QList<int>();
    }

    return QList<int>();
}

void GraphsAlgorithmsSolver::createGraph(const quint32 verticesCount) {
    UndirectedGraphType g(verticesCount);
    intializeVertexIndexProperties(g);
    m_graphsList.append(g);
}

void GraphsAlgorithmsSolver::generateGraph(const quint32 verticesCount, const quint32 edgesCount) {
    UndirectedGraphType g;
    generate_random_graph(g, verticesCount, edgesCount, getRandomGenerator(), false, false);
    intializeVertexIndexProperties(g);
    m_graphsList.append(g);
}

void GraphsAlgorithmsSolver::removeGraph(const int indexForRemoving) {
    if(indexForRemoving < 0 || indexForRemoving >= m_graphsList.size()) {
        qDebug() << "Error! Cannot remove graph, because the graph index is incorrect!";
        return;
    }
    m_graphsList.removeAt(indexForRemoving);
}

void GraphsAlgorithmsSolver::addEdgeToCurrentGraph(const quint32 u, const quint32 v, const int currentGraphIndex) {
    if(currentGraphIndex < 0 || currentGraphIndex >= m_graphsList.size()) {
        qDebug() << "Error! Cannot add edge, because the graph index is incorrect!";
        return;
    }

    add_edge(graphsops::getVertexAtIndexByPositionShift(u,m_graphsList.at(currentGraphIndex)),
             graphsops::getVertexAtIndexByPositionShift(v,m_graphsList.at(currentGraphIndex)),
             m_graphsList[currentGraphIndex]);
}

quint32 GraphsAlgorithmsSolver::getVerticesSizeForCurrentGraph(const int currentGraphIndex) const {
    if(currentGraphIndex < 0 || currentGraphIndex >= m_graphsList.size()) {
        qDebug() << "Error! Cannot get vertices size, because the graph index is incorrect!";
        return -1;
    }
    return num_vertices(m_graphsList.at(currentGraphIndex));
}

quint32 GraphsAlgorithmsSolver::getEdgesSizeForCurrentGraph(const int currentGraphIndex) const {
    if(currentGraphIndex < 0 || currentGraphIndex >= m_graphsList.size()) {
        qDebug() << "Error! Cannot get edges size, because the graph index is incorrect!";
        return -1;
    }
    return num_edges(m_graphsList.at(currentGraphIndex));
}

QList<int> GraphsAlgorithmsSolver::getInitialAdjacentVertices(const quint32 vertex, const int currentGraphIndex) const {
    if(currentGraphIndex < 0 || currentGraphIndex >= m_graphsList.size()) {
        qDebug() << "Error! Cannot get adjacent vertices, because the graph index is incorrect!";
        QList<int> emptyList;
        return emptyList;
    }

    QList<int> adjVerticesList;
    adj_iter_t neighbourBegin, neighbourEnd;
    tie(neighbourBegin, neighbourEnd) = adjacent_vertices(graphsops::getVertexAtIndexByPositionShift(vertex, m_graphsList.at(currentGraphIndex)) ,
                                                          m_graphsList.at(currentGraphIndex));
    for (; neighbourBegin != neighbourEnd; ++neighbourBegin)  {
        const quint32 vi = graphsops::getIndexOfVertex(*neighbourBegin, m_graphsList.at(currentGraphIndex));
        adjVerticesList.append(vi);
    }

    return adjVerticesList;
}

QList<int>  GraphsAlgorithmsSolver::getEdgesListForCurrentGraph(const int currentGraphIndex) const {
    if(currentGraphIndex < 0 || currentGraphIndex >= m_graphsList.size()) {
        qDebug() << "Error! Cannot get edges list, because the graph index is incorrect!";
        QList<int> emptyList;
        return emptyList;
    }

    QList<int>  edgesList;
    std::vector<std::pair<int, int> > sortedEdges = graphsops::getSortedEdges(m_graphsList.at(currentGraphIndex));

    for (std::vector<std::pair<int, int> >::iterator it = sortedEdges.begin(); it != sortedEdges.end(); ++it) {
        edgesList << it->first << it->second;
    }

    return edgesList;
}

int GraphsAlgorithmsSolver::getCurrentGraphsCount() const {
    return m_graphsList.count();
}
