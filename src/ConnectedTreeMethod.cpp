#include <QtDebug>

#include "ConnectedTreeMethod.h"

namespace graphsalgs {

QList<int> findMVCWithConnectedTreeMethod(UndirectedGraphType graph) {

    QList<int> mvc;

    /*check whether we need to stop the algorithm */
    if( num_edges(graph) == 0 ) {
        return mvc;
    }

    const quint32 verticesCount = boost::num_vertices(graph);
    const quint32 edgesCount = boost::num_edges(graph);
    std::vector<ConnectedTree> baseTree;

    //[0] first-pass filling the tree
    for(quint32 i = 1; i < verticesCount; ++i) {

        ConnectedTree currentNode;
        currentNode.nodeStage = i;

        vertex_desc_t iVertex;
        iVertex = graphsops::getVertexAtIndexByPositionShift(i, graph);
        const quint32 iVertexDegree = boost::out_degree(iVertex, graph);

        for(quint32 j = 0; j < i; ++j) {

            //[0][1] check if the current pair forms the baseIndexnimum cover
            vertex_desc_t jVertex;
            jVertex = graphsops::getVertexAtIndexByPositionShift(j, graph);
            QSet<int> testAdjVertices = graphsops::getAdjacentVertices(jVertex, graph);
            testAdjVertices.remove(i);

            const quint32 commonDegree = iVertexDegree + testAdjVertices.size();

            if(commonDegree == edgesCount) {
                //found baseIndexnimum vertex cover
                mvc.append(i);
                mvc.append(j);

                return mvc;

            } else {

                quint32 arr[2] = {j, i};
                std::vector<quint32> currentNodeData(arr, arr+2);
                std::pair<quint32, std::vector<quint32> > nodePair = std::make_pair(commonDegree, currentNodeData);
                currentNode.nodeData.push_back(nodePair);
            }
        }

        baseTree.push_back(currentNode);
    }

    //[1] main searching mvc loop
    while(baseTree.size() > 0) {

        std::vector<ConnectedTree> tempTree;

        for(quint32 baseIndex = 1; baseIndex < baseTree.size(); ++baseIndex) {

            vertex_desc_t baseVertex;

            const quint32 currentStage = baseTree[baseIndex].nodeStage;
            baseVertex = graphsops::getVertexAtIndexByPositionShift(currentStage, graph);
            const quint32 stageDegree = boost::out_degree(baseVertex, graph);

            ConnectedTree newNode;
            newNode.nodeStage = currentStage;

            for(quint32 treeIndex = 0; treeIndex < baseIndex; ++treeIndex) {

                //[1][1] select the node's data with the max degree
                quint32 maxDegree = 0;
                 std::vector<std::pair<quint32, std::vector<quint32> > > newNodeData;
                for(quint32 c = 0; c < baseTree[treeIndex].nodeData.size(); ++c) {
                    if(maxDegree < baseTree[treeIndex].nodeData[c].first) {
                        maxDegree = baseTree[treeIndex].nodeData[c].first;
                        newNodeData.clear();
                        newNodeData.push_back(baseTree[treeIndex].nodeData[c]);
                    } else if(maxDegree == baseTree[treeIndex].nodeData[c].first) {
                        newNodeData.push_back(baseTree[treeIndex].nodeData[c]);
                    }
                }

                baseTree[treeIndex].nodeData = newNodeData;

                //[1][2] form new node
                for(quint32 n = 0; n < baseTree[treeIndex].nodeData.size(); ++n) {

                    //[1][3] determine common degree
                    quint32 commonDegree =  stageDegree + baseTree[treeIndex].nodeData[n].first;
                    for(quint32 testIndex = 0; testIndex < baseTree[treeIndex].nodeData[n].second.size(); ++testIndex) {

                        vertex_desc_t testVertex;
                        const quint32 testVertexIndex = baseTree[treeIndex].nodeData[n].second[testIndex];
                        testVertex = graphsops::getVertexAtIndexByPositionShift(testVertexIndex, graph);
                        QSet<int> testAdjVertices = graphsops::getAdjacentVertices(testVertex, graph);

                        if(testAdjVertices.contains(currentStage)) {
                            --commonDegree;
                        }
                    }

                    //[1][4] check if we have already mvc
                    if(commonDegree == edgesCount ) {
                        mvc.append(currentStage);

                        for(quint32 t = 0; t < baseTree[treeIndex].nodeData[n].second.size(); ++t) {
                            mvc.append(baseTree[treeIndex].nodeData[n].second[t]);
                        }

                        return mvc;

                    } else {

                        std::vector<quint32> currentNodeData(baseTree[treeIndex].nodeData[n].second);
                        currentNodeData.push_back(currentStage);
                        std::pair<quint32, std::vector<quint32> > newNodePair = std::make_pair(commonDegree, currentNodeData);
                        newNode.nodeData.push_back(newNodePair);
                    }
                }
            }

            tempTree.push_back(newNode);
        }

        baseTree.swap(tempTree);
    }

    return QList<int>();
}

}


