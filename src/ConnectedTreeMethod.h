#ifndef CONNECTEDTREEMETHOD_H
#define CONNECTEDTREEMETHOD_H

#include "GraphsTypesHelper.h"
#include "GraphsOperationsHelper.h"

namespace graphsalgs {

struct ConnectedTree {
    std::vector<std::pair<quint32, std::vector<quint32> > > nodeData;
    quint32 nodeStage;
};

QList<int> findMVCWithConnectedTreeMethod (UndirectedGraphType graph);

}

#endif // CONNECTEDTREEMETHOD_H
