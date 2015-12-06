#include "CombinedMethod.h"
#include "CoveringPairsMethod.h"
#include "ShortestPathMethod.h"
#include "GuaranteedPredictionV20Method.h"

#include "AlgorithmPropertiesSingleton.h"

namespace graphsalgs {

QList<int> findMVCWithCombinedMethod(UndirectedGraphType graph) {

    QList<QList<int> > mvcs;

    /*check whether we need to stop the algorithm */
    if (num_edges(graph) == 0 ) {
        return QList<int>();
    }

    mvcs.append(findMVCWithCoveringPairsMethod(graph));
    mvcs.append(findMVCWithShortestPathMethod(graph));
    mvcs.append(findMVCWithGuaranteedPredictionV20Method(graph));


    int minSize = mvcs.at(0).size();
    quint32 minCoverIndex = 0;

    for(int i = 1; i < mvcs.size(); ++i) {
        if(minSize > mvcs.at(i).size()) {
            minSize = mvcs.at(i).size();
            minCoverIndex = i;
        }
    }

    AlgorithmPropertiesSingleton::getInstance().getAlgStepsRef().clear();

    return mvcs.at(minCoverIndex);
}

}
