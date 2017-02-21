#ifndef PUSH_RELABLE
#define PUSH_RELABLE

#include "network.h"
#include <deque>


class MaxFlow {
private:

    Network Net;

    std::vector <size_t> height;
    std::vector <long long> excess;
    std::deque <size_t> nodes;
    size_t maxHeight;
    long long stepCounter;

    void discharge(size_t vertex);

    void push (size_t from, size_t to);

    void relable (size_t vertex);    

public:

    MaxFlow () {};

    MaxFlow (Network &net, long long counter = 0);

    void initializePreflow();

    long long getMaxFlowValue();

    size_t getHeight(size_t i) const;

    size_t getNumberOfVertexes() const;

    Network* getNetwork();

    long long getExcess(size_t i) const;

    long long getFlow(Edge e);

    void addEdge(size_t edge_from, size_t edge_to, long long edge_capacity);

    size_t getMaxHeight() const;

    long long getStepCounter() const;
};

int PushRelable();

#endif // PUSH_RELABLE

