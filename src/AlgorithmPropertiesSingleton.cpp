#include "AlgorithmPropertiesSingleton.h"

#include "CombinedMethod.h"
#include "CoveringPairsMethod.h"
#include "ShortestPathMethod.h"
#include "GuaranteedPredictionV20Method.h"
#include "UnguaranteedPredictionMethod.h"
#include "ConnectedTreeMethod.h"
#include "GuaranteedPredictionMethod.h"
#include "GreedyMethod.h"


void AlgorithmPropertiesSingleton::clearCommonContainer() {
    m_algSteps.clear();
    m_MVC.clear();
    m_NumbeOfOperations = 0;
}

const AlgorithmPropertiesSingleton::AlgFuncPtr AlgorithmPropertiesSingleton::m_algsList[ALGS_NUMBER] =
{
    graphsalgs::findMVCWithCoveringPairsMethod,
    graphsalgs::findMVCWithCombinedMethod,
    graphsalgs::findMVCWithShortestPathMethod,
    graphsalgs::findMVCWithGuaranteedPredictionV20Method,
    graphsalgs::findMVCWithUnguaranteedPredictionMethod,
    graphsalgs::findMVCWithConnectedTreeMethod,
    graphsalgs::findMVCWithGuaranteedPredictionMethod,
    graphsalgs::findMVCWithGreedyMethod,
};
