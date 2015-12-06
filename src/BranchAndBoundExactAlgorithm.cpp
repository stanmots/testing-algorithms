#include "BranchAndBoundExactAlgorithm.h"
#include "AlgorithmPropertiesSingleton.h"

namespace graphsalgs {

QSet<int> partialVC;
void branchAndBoundAlg(UndirectedGraphType& graph);

QList<int> findMVCWithBBExactAlg(UndirectedGraphType graph) {

    partialVC.clear();
    branchAndBoundAlg(graph);
    return AlgorithmPropertiesSingleton::getInstance().getMVCContainterRef();
}

void branchAndBoundAlg(UndirectedGraphType& graph) {

    //[1] check if MVC was found
    if( num_edges(graph) == 0 ) {

        if(AlgorithmPropertiesSingleton::getInstance().getMVCContainterRef().size() == 0 ||
                partialVC.size() < AlgorithmPropertiesSingleton::getInstance().getMVCContainterRef().size()) {

            AlgorithmPropertiesSingleton::getInstance().getMVCContainterRef() = partialVC.toList();
        }

        return;
    }

    //[2] calculate bound
    if(AlgorithmPropertiesSingleton::getInstance().getMVCContainterRef().size() != 0){

        int heuristicVerticesCount = AlgorithmPropertiesSingleton::getInstance().getMVCContainterRef().size() - partialVC.size();

        if(heuristicVerticesCount <= 0) {
            return;
        }
    }

    //[3] put the vertex with the largest current degree into cover
    quint32 indexOfBaseVertexWithLargestDegree = graphsops::getMaxDegreeVertexIndex(graph);
    vertex_desc_t baseVertexWithLargestDegree = graphsops::getVertexAtIndexFromPropertyMap(indexOfBaseVertexWithLargestDegree, graph);

    QSet<int> baseAdjVertices = graphsops::getAdjacentVertices(baseVertexWithLargestDegree, graph);

    QList<int> uncoveredBaseAdjVertices = baseAdjVertices.toList();

    clear_vertex(baseVertexWithLargestDegree, graph);
    partialVC.insert(indexOfBaseVertexWithLargestDegree);

    //[4] recursive call
    branchAndBoundAlg(graph);

    //[5] uncover current vertex with largest degree
    partialVC.remove(indexOfBaseVertexWithLargestDegree);

    //[6] cover inner adjacent vertices
    partialVC.unite(baseAdjVertices);

    QList<QList<int> > uncoveredInnerAdjVerticesGroups;

    for(int i = 0; i < uncoveredBaseAdjVertices.size(); ++i) {

        vertex_desc_t currentVertex = graphsops::getVertexAtIndexFromPropertyMap(uncoveredBaseAdjVertices[i], graph);
        QList<int> currentUncoveredInnerAdjVertices;

        foreach (const int &adjVertexIndex, graphsops::getAdjacentVertices(currentVertex, graph)) {

            currentUncoveredInnerAdjVertices.append(adjVertexIndex);

        }

        uncoveredInnerAdjVerticesGroups.append(currentUncoveredInnerAdjVertices);
        clear_vertex(currentVertex, graph);
    }

    //[7] recursive call
    branchAndBoundAlg(graph);

    //[8] restore adjacent vertices
    for(int i = 0; i < uncoveredBaseAdjVertices.size(); ++i) {

        vertex_desc_t baseInnerVertex = graphsops::getVertexAtIndexFromPropertyMap(uncoveredBaseAdjVertices[i], graph);

        for(int j = 0; j < uncoveredInnerAdjVerticesGroups[i].size(); ++j) {

            vertex_desc_t adjInnerVertex = graphsops::getVertexAtIndexFromPropertyMap(uncoveredInnerAdjVerticesGroups[i][j], graph);
            add_edge(baseInnerVertex, adjInnerVertex, graph);

        }

        //[9] restore base edges
        partialVC.remove(uncoveredBaseAdjVertices[i]);
        add_edge(baseVertexWithLargestDegree, baseInnerVertex, graph);

    }
}


}


