#ifndef GRAPHSTYPESHELPER_H
#define GRAPHSTYPESHELPER_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/random.hpp>

typedef boost::property<boost::vertex_index_t, int> VertexProperty;
typedef boost::adjacency_list<boost::listS, boost::listS, boost::undirectedS, VertexProperty> UndirectedGraphType;
typedef boost::graph_traits<UndirectedGraphType>::vertex_descriptor vertex_desc_t;
typedef boost::graph_traits<UndirectedGraphType>::adjacency_iterator adj_iter_t;
typedef boost::graph_traits<UndirectedGraphType>::edge_iterator edge_iter_t;
typedef boost::graph_traits<UndirectedGraphType>::out_edge_iterator out_edge_iter_t;
typedef boost::graph_traits<UndirectedGraphType>::vertex_iterator vertex_iter_t;
typedef boost::random::mt19937 mersenne_twister_t;

enum IncidentEdgesCount { NO_INCIDENT_EDGES = 0, ONE_INCIDENT_EDGE = 1 };

#endif // GRAPHSTYPESHELPER_H
