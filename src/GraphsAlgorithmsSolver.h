#ifndef GRAPHSALGORITHMSSOLVER_H
#define GRAPHSALGORITHMSSOLVER_H

#include <QObject>
#include <QList>

#include "GraphsTypesHelper.h"
#include "GraphsOperationsHelper.h"

class GraphsAlgorithmsSolver : public QObject
{
    Q_OBJECT
public:
    explicit GraphsAlgorithmsSolver (QObject *parent = 0);

    //qml invokables
    Q_INVOKABLE void createGraph (const quint32 verticesCount = 0);
    Q_INVOKABLE void generateGraph (const quint32 verticesCount, const quint32 edgesCount);
    Q_INVOKABLE void removeGraph (const int indexForRemoving = -1);
    Q_INVOKABLE void addEdgeToCurrentGraph (const quint32 u, const quint32 v, const int currentGraphIndex = -1);
    Q_INVOKABLE QList<int> solveAlgorithmForCurrentGraph (const quint32 algType, const int currentGraphIndex = -1) const;
    Q_INVOKABLE void getAllVertexCoversOfCurrentGraph (const int currentGraphIndex = -1, const bool isNeedSignals = true);
    Q_INVOKABLE static mersenne_twister_t& getRandomGenerator ();
    Q_INVOKABLE quint32 getVerticesSizeForCurrentGraph (const int currentGraphIndex = -1) const;
    Q_INVOKABLE quint32 getEdgesSizeForCurrentGraph (const int currentGraphIndex = -1) const;
    Q_INVOKABLE QList<int> getInitialAdjacentVertices (const quint32 vertex, const int currentGraphIndex = -1) const;
    Q_INVOKABLE QList<int> getEdgesListForCurrentGraph (const int currentGraphIndex = -1) const;
    Q_INVOKABLE QList<int> getArrayOfCoversLengths() const;
    Q_INVOKABLE QList<int> getExactSolution() const;
    Q_INVOKABLE int getCurrentGraphsCount() const;
    Q_INVOKABLE QString getAlgInfo() const;
    Q_INVOKABLE quint32 getNumberOfOperations() const;
    Q_INVOKABLE void clearOldValues();



    //algorithms
    static QList<int> findMVCWithStandardAlg(UndirectedGraphType graph);
    void searchForAllVertexCovers(const UndirectedGraphType graph, const bool isNeedSignals = true);

    //others
    static void intializeVertexIndexProperties (UndirectedGraphType& graph);

signals:
    void foundNewVertexCover(QList<int> vertexCover) const;
    void collectingVertexCoversFinished() const;

public slots:

private:
    //members
    QList<UndirectedGraphType> m_graphsList;
    QList<int> m_arrayOfCoversLengths;
    QList<QList<int> > m_arrayOfCovers;
};

#endif // GRAPHSALGORITHMSSOLVER_H
