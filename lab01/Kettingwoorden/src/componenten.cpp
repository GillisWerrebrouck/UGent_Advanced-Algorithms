#include "graaf.hpp"
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

template <RichtType RT, typename PreOrderFunc, typename PostOrderFunc>
void diepte_eerst_zoeken(const Graaf<RT> &g, PreOrderFunc pre_order_func, PostOrderFunc post_order_func)
{
	vector<bool> visited(g.aantalKnopen(), false);

	for (int nodeIndex = 0; nodeIndex < g.aantalKnopen(); nodeIndex++) {
		if (visited[nodeIndex]) continue;
		depthFirstSearchRecursive(g, nodeIndex, visited, pre_order_func, post_order_func);
	}
}

template <RichtType RT, typename PreOrderFunc, typename PostOrderFunc>
void depthFirstSearchRecursive( const Graaf<RT> &g, int nodeIndex, vector<bool> &visited, PreOrderFunc pre_order_func, PostOrderFunc post_order_func)
{
	pre_order_func(nodeIndex);

	visited[nodeIndex] = true;
	auto it = g[nodeIndex].begin();

	// iterate over all child nodes
	while (it != g[nodeIndex].end()) {
		// only consider nodes that haven't been visited yet
		if (!visited[it->first])
			depthFirstSearchRecursive(g, it->first, visited, pre_order_func, post_order_func);
		it++;
	}

	post_order_func(nodeIndex);
}

vector<int> sterk_geconnecteerde_componenten(const Graaf<GERICHT> &g)
{
	// initialize a mapping for the nodes and their componentnumber
	vector<int> result(g.aantalKnopen());

	Graaf<RichtType::GERICHT> reversed(g.aantalKnopen());

	// reverse the graph
	for (int i = 0; i < g.aantalKnopen(); i++) {
		for (auto it = g[i].begin(); it != g[i].end(); it++) {
			reversed.voegVerbindingToe(it->first, i);
		}
	}

	stack<int> postorder;
	stack<int> preorder; // not really used

	// DFS in the reversed graph to get the postorder of nodes
	diepte_eerst_zoeken(
		reversed,
		[&preorder](int node) { preorder.push(node); },
		[&postorder](int node) { postorder.push(node); }
	);

	vector<bool> visited = vector<bool>(g.aantalKnopen(), false);
	int componentNumber = 0;

	// iterate over all postorder nodes
	while (!postorder.empty()) {
		int node = postorder.top();
		postorder.pop();
		if (!visited[node]) {
			// using lamba function to get all nodes for the current component
			depthFirstSearchRecursive(
				g,
				node,
				visited,
				[](int node) { },
				[&result, componentNumber](int node) {
					result[node] = componentNumber;
				}
			);
			componentNumber++;
		}
	}

	return result;
}

Graaf<GERICHT> componenten_graaf(Graaf<GERICHT> g, std::vector<int> componentnummers)
{
  int numberOfComponents = *std::max_element(componentnummers.begin(), componentnummers.end()) + 1;

	// initialize a graph which will hold a node for eacht component and a connection between components if a connection between nodes of those components exists
	Graaf<GERICHT> componentGraph(numberOfComponents);

	for (int i = 0; i <= numberOfComponents; i++) {
		for (int j = 0; j < g.aantalKnopen(); j++) {
			if (componentnummers[j] == i) {
				auto it = g[j].begin();

				while (it != g[j].end()) {
					if (componentnummers[it->first] != i)
						// a connection between nodes of two different components exists so it is added to the component graph as a connection between those components
						componentGraph.voegVerbindingToe(i, componentnummers[it->first]);
					it++;
				}
			}
		}
	}

	return componentGraph;
}
