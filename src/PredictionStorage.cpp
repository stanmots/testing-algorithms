#include "PredictionStorage.h"

PredictionStorage::PredictionStorage(std::vector<std::pair<quint32, quint32> > equation,
                                      quint32 equationSize,
                                      QList<int> partialVertexCover,
                                      UndirectedGraphType graph):
    m_equation(equation),
    m_equationSize(equationSize),
    m_partialVertexCover(partialVertexCover),
    m_graph(graph)
{
}
