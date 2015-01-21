#include "Dijkstra.hpp"

#include "Heap.hpp"

#include <numeric>
#include <stdexcept>

typedef Heap<Node*, NodeComparator> NodeHeap;

namespace {
	std::list<Node*> build_path(Node *end_node) {
		std::list<Node*> path;

		Node *node = end_node;
		while (node != nullptr) {
			path.push_front(node);
			node = node->nearest_node;
		}

		return path;
	}

	void process_node(Node *node, Node *neighbour, unsigned int weight, NodeHeap &heap)
	{
		unsigned int distance = node->distance + weight;

		if (distance < neighbour->distance) {
			neighbour->distance = distance;
			neighbour->nearest_node = node;

			if (neighbour->state == Node::State::Open) {
				heap.reorder(neighbour);
			}
			else {
				heap.push(neighbour);
			}

			neighbour->state = Node::State::Open;
		}
	}
}

Dijkstra::Dijkstra(Graph &graph) : m_graph(graph) {
}

Dijkstra::~Dijkstra() {
}

void Dijkstra::initialize_nodes(unsigned int start_id, unsigned int end_id) {
	m_start_node = m_end_node = nullptr;

	for (auto &p : m_graph.get_nodes()) {
		Node *node = p.second;
		node->distance = std::numeric_limits<unsigned int>::max();
		node->nearest_node = nullptr;
		node->state = Node::State::Unknown;

		if (node->id == start_id) {
			m_start_node = node;
		}

		if (node->id == end_id) {
			m_end_node = node;
		}
	}

	if (m_start_node == nullptr || m_end_node == nullptr) {
		throw std::runtime_error("Failed to find the start/end node(s)!");
	}

	m_start_node->distance = 0;
}

std::list<Node*> Dijkstra::get_shortest_path(unsigned int start_id, unsigned int end_id) {
	initialize_nodes(start_id, end_id);

	NodeHeap heap;
	heap.push(m_start_node);

	while (!heap.empty()) {
		Node *node = heap.pop();
		node->state = Node::State::Closed;

		if (node == m_end_node) {
			break;
		}

		for (auto &e : node->edges) {
			if (e.node->state != Node::State::Closed) {
				process_node(node, e.node, e.weight, heap);
			}
		}
	}

	return build_path(m_end_node);
}