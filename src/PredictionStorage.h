#ifndef PREDICTIONSTORAGE_H
#define PREDICTIONSTORAGE_H

#include <QObject>
#include "GraphsTypesHelper.h"

class PredictionStorage
{
public:
    PredictionStorage(std::vector<std::pair<quint32, quint32> > equation,
                       quint32 equationSize,
                       QList<int> partialVertexCover,
                       UndirectedGraphType graph);

     UndirectedGraphType m_graph;
     quint32 m_equationSize;
     std::vector<std::pair<quint32, quint32> > m_equation;
     QList<int> m_partialVertexCover;
};

#endif // PREDICTIONSTORAGE_H
