#include "CoveringPairHolder.h"

CoveringPairHolder::CoveringPairHolder():m_FirstVertexDegree(0),
    m_SecondVertexDegree(0),
    m_CommonDegree(0),
    m_CoveringPair(std::make_pair(0,0))
{
}

CoveringPairHolder::CoveringPairHolder(quint32 firstVertex, quint32 secondDegree,
                                       quint32 commonDegree,
                                       std::pair<int, int> coveringPair):
    m_FirstVertexDegree(firstVertex),
    m_SecondVertexDegree(secondDegree),
    m_CommonDegree(commonDegree),
    m_CoveringPair(coveringPair)
{

}
