#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <list>
#include <map>
#include <string>
#include <utility>

struct Node;

struct Edge {
	Node *node;
	int weight;
};

struct Node {
	enum class State {
		Closed, Open, Unknown
	};

	unsigned int id, distance, index;
	std::list<Edge> edges;
	Node *nearest_node;
	State state;
};

struct NodeComparator {
	bool operator()(const Node *lhs, const Node *rhs) const {
		return lhs->distance < rhs->distance;
	}
};


class Graph {
public:
	typedef std::map<int, Node*> NodeMap;

	Graph() = default;
	~Graph() {
		for (auto &p : m_nodes) {
			delete p.second;
		}
	}

	Graph(Graph && graph) {
		std::swap(m_nodes, graph.m_nodes);
	}

	Graph& operator=(const Graph&) = delete;
	Graph(const Graph&) = delete;

	inline const NodeMap& get_nodes() const {
		return m_nodes;
	}

	inline Node* get_or_add(int id) {
		Node *node = m_nodes[id];
		if (node == nullptr) {
			node = new Node();
			node->id = id;

			m_nodes[id] = node;
		}

		return node;
	}

private:
	NodeMap m_nodes;
};

#endif