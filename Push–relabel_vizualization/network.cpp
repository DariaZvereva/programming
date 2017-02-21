#include "network.h"
#include <iostream>

NetworkEdge::NetworkEdge(): from(0), to(0), capacity(0), flow(0) {}

NetworkEdge::NetworkEdge(size_t from, size_t to, long long capacity):
    from(from), to(to), capacity(capacity), flow(0) {}

NetworkEdge::NetworkEdge(size_t from, size_t to, long long capacity, long long flow):
    from(from), to(to), capacity(capacity), flow(flow) {}

Network::Network(Network &Net){
    numberOfVertexes = Net.getNumberOfVertexes();
    source = Net.getSource();
    sink = Net.getSink();
    edges = Net.getEdges();
    neighbours = Net.getNeighbours();
}

Network::Network(size_t n, size_t s, size_t t):
    numberOfVertexes(n), source(s), sink(t) {
    neighbours.resize(n);
    edges.resize(n);
    for (size_t i = 0; i < n; ++i) {
        edges[i].resize(n);
    }
}

void Network::dfs(size_t v,std::vector<bool> &used, std::vector<std::vector<int>> &e){
    used[v] = true;
    for (size_t to = 0; to < numberOfVertexes; ++to){
        if (!used[to] && e[v][to] == 1) {
            dfs(to, used, e);
        }
    }
}

bool Network::isGraphConnected(std::vector<std::vector<int>> &e){
    std::vector<bool> used(numberOfVertexes, false);
    used[source] = true;
    dfs(source, used, e);
    for (size_t i = 0; i < numberOfVertexes; ++i){
        if (!used[i]){
            return false;
        }
    }
    return true;
}

void Network::generateNetwork(size_t numberOfEdges, int maxCapacity){
    srand(time(0));
    std::vector<std::vector<int> > tempEdges;
    tempEdges.resize(numberOfVertexes);
   // while (!isGraphConnected(tempEdges)) {
        for (size_t i = 0; i < numberOfVertexes; ++i){
            tempEdges[i].assign(numberOfVertexes, 0);
        }
        for (size_t i = 0; i < numberOfEdges; ++i) {
            size_t start = 0;
            size_t finish = 0;
            while (start == finish || tempEdges[start][finish] != 0) {
                start = rand() % numberOfVertexes;
                finish = rand() % numberOfVertexes;
            }
            tempEdges[start][finish] = 1;
            tempEdges[finish][start] = -1;
      //  }
   // }
   // for (size_t i = 0; i < numberOfVertexes; ++i){
       // for (size_t j = 0; j < numberOfVertexes; ++j){
     //       if (tempEdges[i][j] == 1) {
                int capacity = 1 + rand() % maxCapacity;
                addEdge(start, finish, capacity);
       //     }
       // }
    }
}

Network::Network(){
    numberOfVertexes = 0;
    source = 0;
    sink = 0;
}

std::vector< std::vector<NetworkEdge> > Network::getEdges() const{
    return edges;
}

std::vector< std::vector<size_t> > Network::getNeighbours() const{
    return neighbours;
}

void Network::addEdge(size_t edge_from, size_t edge_to, long long edge_capacity){
    edges[edge_from][edge_to].capacity += edge_capacity;
    neighbours[edge_from].push_back(edge_to);
    neighbours[edge_to].push_back(edge_from);
}

size_t Network::getSource() const {
    return source;
}

size_t Network::getSink() const {
    return sink;
}

size_t Network::getNumberOfVertexes() const {
    return numberOfVertexes;
}

void Network::setFlow(size_t i, size_t j, long long flow){
    edges[i][j].flow = flow;
}

void Network::changeFlow(size_t i, size_t j, long long delta){
    edges[i][j].flow += delta;
}

long long Network::getCapacity(size_t i, size_t j) const{
    return edges[i][j].capacity;
}

long long Network::getFlow(size_t i, size_t j) const {
    return edges[i][j].flow;
}

size_t Network::getNeighboursSize(size_t i) const {
    return neighbours[i].size();
}

size_t Network::getNeighbour(size_t i, size_t j) const {
    return neighbours[i][j];
}

Network& Network::operator*(){
    return *this;
}
