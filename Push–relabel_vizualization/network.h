#ifndef NETWORK_H
#define NETWORK_H

#include <sys/types.h>
#include <vector>
#include <ctime>
#include <cstdlib>


struct Edge{
    size_t from;
    size_t to;
    long long capacity;

    Edge(): from(0), to(0), capacity(0) {};
    Edge(size_t a, size_t b, long long c): from(a), to(b), capacity(c) {}

};

struct NetworkEdge{

    size_t from;
    size_t to;
    long long capacity;
    long long flow;

    NetworkEdge();
    NetworkEdge(size_t from, size_t to, long long capacity);
    NetworkEdge(size_t from, size_t to, long long capacity, long long flow);
};

class Network{
private:
    bool isGraphConnected(std::vector<std::vector<int>> &e);
    void dfs(size_t v,std::vector<bool> &used, std::vector<std::vector<int>> &e);

    size_t numberOfVertexes;
    size_t source;
    size_t sink;
    std::vector < std::vector <NetworkEdge> > edges;
    std::vector < std::vector <size_t> > neighbours;

public:

    Network(size_t n, size_t s, size_t t);

    Network(Network &Net);

    Network();

    void addEdge(size_t edge_from, size_t edge_to, long long edge_capacity);

    Network& operator*();

    size_t getSource() const;
    size_t getSink() const;
    size_t getNumberOfVertexes() const;
    long long getCapacity(size_t i, size_t j) const;
    long long getFlow(size_t i, size_t j) const;
    size_t getNeighboursSize(size_t i) const;
    size_t getNeighbour(size_t i, size_t j) const;
    std::vector<std::vector<NetworkEdge> > getEdges() const;
    std::vector<std::vector<size_t> > getNeighbours() const;

    void setFlow(size_t i, size_t j, long long flow);
    void changeFlow(size_t i, size_t j, long long delta);

    void generateNetwork(size_t numberOfEdges, int maxCapacity);

};

#endif // NETWORK_H
