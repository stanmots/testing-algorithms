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
    Q_INVOKABLE static mersenne_twister_t& getRandomGenerator ();

    Q_INVOKABLE void createGraph (const quint32 verticesCount = 0);
    Q_INVOKABLE void generateGraph (const quint32 verticesCount, const quint32 edgesCount);
    Q_INVOKABLE void removeGraph (const int indexForRemoving = -1);
    Q_INVOKABLE void addEdgeToCurrentGraph (const quint32 u, const quint32 v, const int currentGraphIndex = -1);

    Q_INVOKABLE QList<int> solveAlgorithmForCurrentGraph (const quint32 algType, const int currentGraphIndex = -1) const;
    Q_INVOKABLE QList<int> findExactSolutionForCurrentGraph (const int currentGraphIndex = -1) const;

    Q_INVOKABLE quint32 getVerticesSizeForCurrentGraph (const int currentGraphIndex = -1) const;
    Q_INVOKABLE quint32 getEdgesSizeForCurrentGraph (const int currentGraphIndex = -1) const;
    Q_INVOKABLE QList<int> getInitialAdjacentVertices (const quint32 vertex, const int currentGraphIndex = -1) const;
    Q_INVOKABLE QList<int> getEdgesListForCurrentGraph (const int currentGraphIndex = -1) const;
    Q_INVOKABLE int getCurrentGraphsCount() const;
    Q_INVOKABLE QString getAlgInfo() const;
    Q_INVOKABLE quint32 getNumberOfOperations() const;

private:
    bool prepareProblemSolving(const int currentGraphIndex = -1) const;

    QList<UndirectedGraphType> m_graphsList;
};

#endif // GRAPHSALGORITHMSSOLVER_H
