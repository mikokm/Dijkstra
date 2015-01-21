#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include "Graph.hpp"
#include "Dijkstra.hpp"

namespace Json {
	static const char *g_graph_file = "graph.json";
	static const char *g_journeys_file = "journeys.json";

	typedef std::list<std::pair<int, int>> PairList;
	typedef std::list<std::list<Node*>> PathList;

	std::vector<char> read_file(const std::string &filename) {
		std::ifstream file(filename);

		if (!file.is_open()) {
			throw std::runtime_error("Failed to open file!");
		}

		file.seekg(0, std::ios::end);
		auto len = static_cast<size_t>(file.tellg());
		file.seekg(0, std::ios::beg);

		std::vector<char> buffer(len + 1, '\0');
		file.read(buffer.data(), len);

		if (!file) {
			throw std::runtime_error("Failed to read the file!");
		}

		return buffer;
	}

	PairList read_journeys(const std::string &filename) {
		auto buffer = read_file(filename);

		rapidjson::Document doc;
		doc.Parse(buffer.data());

		PairList pairs;
		for (rapidjson::SizeType i = 0; i < doc.Size(); ++i) {
			pairs.push_back(std::make_pair<int, int>(doc[i]["from"].GetInt(), doc[i]["to"].GetInt()));
		}

		return pairs;
	}

	void write_journeys(const PairList &pairs, const PathList &paths) {
		rapidjson::Document doc;
		doc.SetArray();

		auto &alloc = doc.GetAllocator();
		auto path = paths.begin();

		for (auto &pair : pairs) {
			rapidjson::Value o(rapidjson::kObjectType);
			o.AddMember("from", rapidjson::Value().SetInt(pair.first), alloc);
			o.AddMember("to", rapidjson::Value().SetInt(pair.second), alloc);

			rapidjson::Value arr(rapidjson::kArrayType);
			for (Node *n : *path) {
				arr.PushBack(rapidjson::Value().SetInt(n->id), alloc);
			}

			o.AddMember("route", arr, alloc);
			doc.PushBack(o, alloc);

			++path;
		}

		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);

		std::cout << buffer.GetString() << std::endl;
	}

	Graph build_graph(const std::string &filename) {
		auto buffer = read_file(filename);

		rapidjson::Document doc;
		doc.Parse(buffer.data());

		Graph graph;
		for (rapidjson::SizeType i = 0; i < doc.Size(); ++i) {
			Node *from_node = graph.get_or_add(doc[i]["from"].GetInt());
			Node *to_node = graph.get_or_add(doc[i]["to"].GetInt());

			if (from_node == to_node) {
				continue;
			}

			from_node->edges.push_back(Edge{ to_node, doc[i]["weight"].GetInt() });
		}

		return graph;
	}
}

void solve() {
	std::ios_base::sync_with_stdio(false);

	Graph graph = Json::build_graph(Json::g_graph_file);
	Dijkstra dijkstra(graph);

	Json::PairList journeys = Json::read_journeys(Json::g_journeys_file);
	Json::PathList routes;

	for (auto &p : journeys) {
		auto path = dijkstra.get_shortest_path(p.first, p.second);
		routes.push_back(path);
	}

	Json::write_journeys(journeys, routes);
}

int main() {
	try {
		solve();
	}
	catch (std::exception &e) {
		std::cerr << "Failed: " << e.what() << std::endl;
	}

	//std::cin.get();
}
