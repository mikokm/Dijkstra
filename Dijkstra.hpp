#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "Graph.hpp"

#include <list>

class Dijkstra {
public:
	Dijkstra(Graph &graph);
	~Dijkstra();

	Dijkstra& operator=(const Dijkstra&) = delete;

	std::list<Node*> get_shortest_path(unsigned int start_id, unsigned int end_id);

private:
	void initialize_nodes(unsigned int start_id, unsigned int end_id);

	Graph &m_graph;
	Node *m_start_node, *m_end_node;
};

#endif