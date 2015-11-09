#ifndef OPTIMIZEDMVCALGORITHM_H
#define OPTIMIZEDMVCALGORITHM_H

#include "GraphsTypesHelper.h"
#include "GraphsOperationsHelper.h"

namespace graphsalgs {

std::pair<int, int> getMaxIndependentPair (const UndirectedGraphType& graph);
QList<int> findMVCWithUnguaranteedPredictionMethod (UndirectedGraphType graph);
QList<std::pair<int, int> > getWPList (std::pair<int, int> maxPair, const UndirectedGraphType& graph);
quint32 getPHeuristic (const quint32 vertexForDeleteIndex, const quint32 vertexForUncoverIndex, UndirectedGraphType graph);

}

#endif // OPTIMIZEDMVCALGORITHM_H
