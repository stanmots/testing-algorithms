#include "GreedyMethod.h"

namespace graphsalgs {

QList<int> findMVCWithGreedyMethod(UndirectedGraphType graph) {

    if(num_vertices(graph) < 2) {
        return QList<int>();
    }

    QList<int> minimumVertexCoverList;
    vertex_iter_t vertexItBegin, vertexItEnd, next;
    bool isFoundMinimumVertexCover = false;

    while (!isFoundMinimumVertexCover) {

        /*[1] check if there are vertices with just one incident edge in the graph
        * and add them to the vertex cover */

        graphsops::addHangingVerticesToCover(minimumVertexCoverList, graph);

        /*[2] check if we have already the minimum vertex cover */
        if( num_edges(graph) == 0 ) {
            isFoundMinimumVertexCover = true;
            break;
        }
        /*[3] find the vertex with the most incident edges degree
        * and add it to the vertex cover */
        else {
            vertex_desc_t maxEdgesVertex;
            boost::tie(vertexItBegin, vertexItEnd) = vertices(graph);
            quint32 maxDegree = 0;
            for (next = vertexItBegin; vertexItBegin != vertexItEnd; vertexItBegin = next) {
                ++next;
                const quint32 currentVertexDegree = out_degree(*vertexItBegin, graph);
                if(currentVertexDegree > maxDegree) {
                    maxDegree = currentVertexDegree;
                    maxEdgesVertex = *vertexItBegin;
                }
            }
            minimumVertexCoverList.append(graphsops::getIndexOfVertex(maxEdgesVertex, graph));
            clear_vertex(maxEdgesVertex, graph);
            remove_vertex(maxEdgesVertex, graph);

            if( num_edges(graph) == 0 ) {
                //loop finished
                isFoundMinimumVertexCover = true;
            }
        }
    }

    return minimumVertexCoverList;
}

}
