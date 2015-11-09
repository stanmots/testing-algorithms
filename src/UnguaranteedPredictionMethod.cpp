#include <QtDebug>

#include "UnguaranteedPredictionMethod.h"

namespace graphsalgs {

std::pair<int, int> getMaxIndependentPair (const UndirectedGraphType& graph) {
    if(boost::num_vertices(graph) < 2) {
        qDebug() << "Error! The number of vertices is too small!";
        return std::make_pair(-1, -1);
    }

    /*[1] find all independent pairs of graph's vertices
     */
    std::vector<std::pair<quint32, quint32> > processedVertices;
    std::vector<std::pair<quint32, quint32> > maxPairs;
    quint32 currentMaxDegree = 0;

    vertex_iter_t vertexItBegin, vertexItEnd, next;
    boost::tie(vertexItBegin, vertexItEnd) = vertices(graph);

    for (next = vertexItBegin; vertexItBegin != vertexItEnd; vertexItBegin = next) {
        ++next;
        QSet<int> avSet = graphsops::getAdjacentVertices(*vertexItBegin, graph);
        const quint32 baseVertexIndex =  graphsops::getIndexOfVertex(*vertexItBegin, graph);

        /*[1][1] loop through all vertices to find new independent pairs
         * with base vertex
         */
        vertex_iter_t vertexItBeginInner, vertexItEndInner, nextInner;
        boost::tie(vertexItBeginInner, vertexItEndInner) = vertices(graph);
        for (nextInner = vertexItBeginInner; vertexItBeginInner != vertexItEndInner; vertexItBeginInner = nextInner) {
            ++nextInner;

            const quint32 currentVertexIndex = graphsops::getIndexOfVertex(*vertexItBeginInner, graph);

            if( baseVertexIndex != currentVertexIndex) {

                if(avSet.contains(currentVertexIndex) != true &&
                        std::find_if(processedVertices.begin(),processedVertices.end(), graphsops::MatchPairs(baseVertexIndex, currentVertexIndex))
                        == processedVertices.end()) {

                    //[1][2] new pair is found here, need recheck current max degree
                    std::pair<quint32, quint32> currentPair = std::make_pair(baseVertexIndex, currentVertexIndex);
                    processedVertices.push_back(currentPair);

                    const quint32 commonDegree = boost::out_degree(*vertexItBeginInner, graph)
                            + boost::out_degree(*vertexItBegin, graph);

                    if(commonDegree > currentMaxDegree) {
                        currentMaxDegree = commonDegree;
                        maxPairs.clear();
                        maxPairs.push_back(currentPair);
                    } else if(commonDegree == currentMaxDegree) {
                        maxPairs.push_back(currentPair);
                    }
                }
            }
        }
    }

    /*[2] choose one pair from the array which contains vertices with
     * maximum degree
     */

    if(maxPairs.capacity() > 0) {
        if(maxPairs.capacity() == 1) {
            return maxPairs[0];
        }

        std::multiset<quint32> setWithCounts;

        //assign count to each vertex
        for(std::vector<std::pair<quint32, quint32> >::const_iterator it = maxPairs.begin(); it != maxPairs.end(); ++it) {
            setWithCounts.insert(it->first);
            setWithCounts.insert(it->second);
        }

        std::pair<int, int> maxPair(-1,-1);
        quint32 maxPairsCount = 0;
        for(std::vector<std::pair<quint32, quint32> >::const_iterator it = maxPairs.begin(); it != maxPairs.end(); ++it) {
            const quint32 pairsCount = setWithCounts.count(it->first) + setWithCounts.count(it->second);
            if(pairsCount > maxPairsCount) {
                maxPairsCount = pairsCount;
                maxPair = std::make_pair(it->first, it->second);
            }
        }

        return maxPair;

    } else {
        //there are no indie pairs
        return std::make_pair(-1, -1);
    }

}

quint32 getPHeuristic (const quint32 vertexForDeleteIndex, const quint32 vertexForUncoverIndex, UndirectedGraphType graph) {

    vertex_desc_t vertexForDelete, vertexForUncover;
    vertexForDelete = graphsops::getVertexAtIndexFromPropertyMap(vertexForDeleteIndex, graph);
    vertexForUncover = graphsops::getVertexAtIndexFromPropertyMap(vertexForUncoverIndex, graph);

    /*[1] remove vertex */
    boost::clear_vertex(vertexForDelete, graph);
    boost::remove_vertex(vertexForDelete, graph);

    QSet<int> adjVertices = graphsops::getAdjacentVertices(vertexForUncover, graph);

    /*[2] clear all vertices adjacent with vertexForUncover*/
    adj_iter_t adjVerItBegin, adjVerItEnd, nextAdjV;

    tie(adjVerItBegin, adjVerItEnd) = boost::adjacent_vertices(vertexForUncover, graph);

    for (nextAdjV = adjVerItBegin; adjVerItBegin != adjVerItEnd; adjVerItBegin = nextAdjV)  {
        ++nextAdjV;
        boost::clear_vertex(*adjVerItBegin, graph);
    }

    /*[3] traverse all vertices of the graph */
    vertex_iter_t vertexItBegin, vertexItEnd, next;
    boost::tie(vertexItBegin, vertexItEnd) = vertices(graph);
    quint32 pheuristicCount = 0;

    for (next = vertexItBegin; vertexItBegin != vertexItEnd; vertexItBegin = next) {
        ++next;

        const quint32 vIndex = graphsops::getIndexOfVertex(*vertexItBegin, graph);
        if(adjVertices.contains(vIndex) || boost::out_degree(*vertexItBegin, graph) > 0) {
            if(vIndex != vertexForUncoverIndex) {
                ++pheuristicCount;
            }
        }
    }

    return pheuristicCount;
}


QList<std::pair<int, int> > getWPList (std::pair<int, int> maxPair, const UndirectedGraphType& graph) {

    vertex_desc_t vertex1, vertex2;
    QList<std::pair<int, int> > wpList;
    vertex1 = graphsops::getVertexAtIndexFromPropertyMap(maxPair.first, graph);
    vertex2 = graphsops::getVertexAtIndexFromPropertyMap(maxPair.second, graph);

    /*[1] v1 = 0; v2 = 0 */
    int w1 = boost::out_degree(vertex1, graph) + boost::out_degree(vertex2, graph);
    int p1 = boost::num_vertices(graph) - 2;

    wpList.append(std::make_pair(w1, p1));

    /*[2] v1= 0; v2 = 1 */
    int w2 = boost::out_degree(vertex1, graph);
    int p2 = getPHeuristic(maxPair.first, maxPair.second, graph);

    wpList.append(std::make_pair(w2, p2));

    /*[3] v1= 1; v2 = 0 */
    int w3 = boost::out_degree(vertex2, graph);
    int p3 = getPHeuristic(maxPair.second, maxPair.first, graph);

    wpList.append(std::make_pair(w3, p3));

    return wpList;
}


QList<int> findMVCWithUnguaranteedPredictionMethod (UndirectedGraphType graph) {

    QList<int> minimumVertexCoverList;

    while(true) {

        /*[1] check and add all hanging vertices to the cover */
        graphsops::addHangingVerticesToCover(minimumVertexCoverList, graph);

        /*[1.1] check if we have already the minimum vertex cover */
        if( num_edges(graph) == 0 ) {
            return minimumVertexCoverList;
        }

        /*[2] check if there is Hamiltonian path, if it is then
        from a minimum vertex cover with size:
        a) (n+1)/2 -- vertex count is even
        b) n/2 -- vertex count is odd
        */

        if(graphsops::DoesEachVertexHaveSameDegree(graph) == true) {

            graphsops::deleteVerticesWithoutEdges(graph);

            const quint32 verticesCount = boost::num_vertices(graph);
            quint32 vertexCoverLimit = 0;

            if((verticesCount % 2) == 0) {
                //verticesCount is even
                vertexCoverLimit = verticesCount / 2;
            } else {
                //verticesCount is odd
                vertexCoverLimit = (verticesCount + 1) / 2;
            }

            vertex_iter_t vertexItBegin, vertexItEnd, next;
            boost::tie(vertexItBegin, vertexItEnd) = vertices(graph);
            quint32 i = 0;
            for (next = vertexItBegin; i < vertexCoverLimit && vertexItBegin != vertexItEnd; ++i, vertexItBegin = next) {
                ++next;
                minimumVertexCoverList.append(graphsops::getIndexOfVertex(*vertexItBegin, graph));
            }

            return minimumVertexCoverList;
        }

        /*[3] get maximum independent pair from the current graph */
        std::pair<int, int> maxPair = getMaxIndependentPair(graph);
        if(maxPair.first > -1) {

            QList<std::pair<int, int> > wpList = getWPList(maxPair, graph);

            quint16 caseType = -1, caseTypeP = -1, caseTypeW = -1, maxW = 0;
            int minP = wpList.at(0).second;
            bool isNeedFinishing = false;
            bool isIdenticalHeuristic = true;
            const int matchPTest = wpList.at(0).second;

            for(int i = 0; i < wpList.size(); ++i) {

                if(wpList.at(i).second == 0) {
                    //finished
                    isNeedFinishing = true;
                    caseType = i + 1;
                    break;
                }

                if(matchPTest != wpList.at(i).second) {
                    isIdenticalHeuristic = false;
                }

                if(wpList.at(i).second < minP) {
                    minP = wpList.at(i).second;
                    caseTypeP = i + 1;
                }

                if(wpList.at(i).first > maxW) {
                    maxW = wpList.at(i).first;
                    caseTypeW = i + 1;
                }
            }

            if(isNeedFinishing == false && isIdenticalHeuristic == true) {
                caseType = caseTypeW;
            } else if(isNeedFinishing == false && isIdenticalHeuristic == false) {
                caseType = caseTypeP;
            }

            vertex_desc_t vertex1, vertex2;

            switch(caseType) {
            case 1:
                vertex1 = graphsops::getVertexAtIndexFromPropertyMap(maxPair.first, graph);
                vertex2 = graphsops::getVertexAtIndexFromPropertyMap(maxPair.second, graph);

                boost::clear_vertex(vertex1, graph);
                boost::remove_vertex(vertex1, graph);
                boost::clear_vertex(vertex2, graph);
                boost::remove_vertex(vertex2, graph);

                minimumVertexCoverList.append(maxPair.first);
                minimumVertexCoverList.append(maxPair.second);
                break;
            case 2:
                vertex1 = graphsops::getVertexAtIndexFromPropertyMap(maxPair.first, graph);

                boost::clear_vertex(vertex1, graph);
                boost::remove_vertex(vertex1, graph);

                minimumVertexCoverList.append(maxPair.first);
                break;
            case 3:
                vertex2 = graphsops::getVertexAtIndexFromPropertyMap(maxPair.second, graph);

                boost::clear_vertex(vertex2, graph);
                boost::remove_vertex(vertex2, graph);

                minimumVertexCoverList.append(maxPair.second);
                break;
            }

            if(isNeedFinishing == true) {
                return minimumVertexCoverList;
            }

        } else {
            qDebug()<< "Warning! Cannot find max independent pair!";
            break;
        }
    }

    return QList<int>();
}

}
