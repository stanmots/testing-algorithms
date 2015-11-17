#ifndef COVERINGPAIRHOLDER_H
#define COVERINGPAIRHOLDER_H

#include <QDebug>

class CoveringPairHolder
{
public:
    CoveringPairHolder();
    CoveringPairHolder(quint32 firstDegree, quint32 secondDegree,
                       quint32 commonDegree, std::pair<int, int> coveringPair);

    quint32 getCommonDegree() const { return m_CommonDegree; }
    std::pair<int, int> getCoverintPair() const { return m_CoveringPair; }

    quint32 getFirstVertexDegree() const { return m_FirstVertexDegree; }
    quint32 getSecondVertexDegree() const { return m_SecondVertexDegree; }


private:
    quint32 m_CommonDegree;
    quint32 m_FirstVertexDegree;
    quint32 m_SecondVertexDegree;
    std::pair<int, int> m_CoveringPair;
};

#endif // COVERINGPAIRHOLDER_H
