#include <QTime>

#include "GraphsAlgorithmsSolver.h"
#include "GraphsOperationsHelper.h"
#include "AlgorithmPropertiesSingleton.h"
#include "BranchAndBoundExactAlgorithm.h"

using namespace boost;

GraphsAlgorithmsSolver::GraphsAlgorithmsSolver(QObject *parent) :
    QObject(parent) {}

mersenne_twister_t& GraphsAlgorithmsSolver::getRandomGenerator() {

    static mersenne_twister_t m_randomGenerator(static_cast<unsigned int>(QTime::currentTime().msec()));
    return m_randomGenerator;
}

QString GraphsAlgorithmsSolver::getAlgInfo() const {

    return AlgorithmPropertiesSingleton::getInstance().getAlgSteps();
}

quint32 GraphsAlgorithmsSolver::getNumberOfOperations() const {

    return AlgorithmPropertiesSingleton::getInstance().getNumbeOfOperations();
}

bool GraphsAlgorithmsSolver::prepareProblemSolving(const int currentGraphIndex) const {

    if(currentGraphIndex < 0 || currentGraphIndex >= m_graphsList.size()) {
        qDebug() << "Error! Cannot solve algorithm, because the graph index is incorrect!";
        return false;
    }

    if(boost::num_vertices(m_graphsList.at(currentGraphIndex)) < 1) {
        qDebug() << "Error! The number of vertices is too small!";
        return false;
    }

    AlgorithmPropertiesSingleton::getInstance().clearCommonContainer();

    return true;
}

QList<int> GraphsAlgorithmsSolver::findExactSolutionForCurrentGraph(const int currentGraphIndex) const {

    if(prepareProblemSolving(currentGraphIndex) == false) {
        return QList<int>();
    }

    return graphsalgs::findMVCWithBBExactAlg(m_graphsList.at(currentGraphIndex));
}

QList<int> GraphsAlgorithmsSolver::solveAlgorithmForCurrentGraph(const quint32 algType, const int currentGraphIndex) const {

    if(prepareProblemSolving(currentGraphIndex) == false) {
        return QList<int>();
    }

    return (*AlgorithmPropertiesSingleton::getInstance().getAlgsListRef()[algType])(m_graphsList.at(currentGraphIndex));
}

void GraphsAlgorithmsSolver::createGraph(const quint32 verticesCount) {
    UndirectedGraphType g(verticesCount);
    graphsops::intializeVertexIndexProperties(g);
    m_graphsList.append(g);
}

void GraphsAlgorithmsSolver::generateGraph(const quint32 verticesCount, const quint32 edgesCount) {
    UndirectedGraphType g;
    generate_random_graph(g, verticesCount, edgesCount, getRandomGenerator(), false, false);
    graphsops::intializeVertexIndexProperties(g);
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
