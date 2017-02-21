#include "push_relable.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include <ctime>

using namespace std;

MaxFlow::MaxFlow(Network &net, long long counter): Net(net), stepCounter(counter) {
    maxHeight = net.getNumberOfVertexes();
    excess.assign(net.getNumberOfVertexes(), 0);
    height.assign(net.getNumberOfVertexes(), 0);
    height[Net.getSource()] = Net.getNumberOfVertexes();
    maxHeight = height[Net.getSource()];
}

void MaxFlow::addEdge(size_t edge_from, size_t edge_to, long long edge_capacity){
    Net.addEdge(edge_from, edge_to, edge_capacity);
}


void MaxFlow::initializePreflow() {
    size_t n = Net.getNumberOfVertexes();
    for (size_t i = 0; i < n; ++i) {
        if (i == Net.getSource()) {
            continue;
        }
        Net.setFlow(Net.getSource(), i, Net.getCapacity(Net.getSource(), i));
        Net.setFlow(i, Net.getSource(), -Net.getCapacity(Net.getSource(), i));
        excess[i] = Net.getCapacity(Net.getSource(), i);
        excess[Net.getSource()] -= Net.getCapacity(Net.getSink(), i);
        if (Net.getCapacity(Net.getSource(), i) != 0 && i != Net.getSink()){
            nodes.push_back(i);
        }
    }
}

size_t MaxFlow::getHeight(size_t i) const {
    return height[i];
}

long long MaxFlow::getExcess(size_t i) const {
    return excess[i];
}

long long MaxFlow::getMaxFlowValue() {
    if (stepCounter == 0) return 0;
    initializePreflow();
    for (size_t i = 1; !nodes.empty() && i < stepCounter; ++i){
        size_t vertex = nodes.front();
        nodes.pop_front();
        discharge(vertex);
    }
    return excess[Net.getSink()];
}

void MaxFlow::discharge(size_t vertex){
    size_t i = 0;
    while (excess[vertex] > 0) {
        if (height[vertex] == height[Net.getNeighbour(vertex, i)] + 1 && Net.getCapacity(vertex, Net.getNeighbour(vertex, i)) - Net.getFlow(vertex, Net.getNeighbour(vertex, i)) > 0) {
            push(vertex, Net.getNeighbour(vertex, i));
        }
        ++i;
        if (i == Net.getNeighboursSize(vertex)){
            relable(vertex);
            i = 0;
        }
    }
}

void MaxFlow::push (size_t from, size_t to) {
    long long delta_flow = std::min(excess[from], Net.getCapacity(from, to) - Net.getFlow(from, to));
    if (excess[to] == 0 && delta_flow > 0 && to != Net.getSource() && to != Net.getSink()){
        nodes.push_back(to);
    }
    Net.changeFlow(from, to, delta_flow);
    Net.changeFlow(to, from, -delta_flow);
    excess[from] -= delta_flow;
    excess[to] += delta_flow;
}

void MaxFlow::relable (size_t vertex) {
    height[vertex] = maxHeight;
    size_t n = Net.getNeighboursSize(vertex);
    for (size_t i = 0; i < n; ++i) {
        if (Net.getCapacity(vertex, Net.getNeighbour(vertex, i)) - Net.getFlow(vertex, Net.getNeighbour(vertex, i)) > 0) {
            height[vertex] = min(height[vertex], height[Net.getNeighbour(vertex, i)]);
        }
    }
    height[vertex]++;
    maxHeight = max(maxHeight, height[vertex]);
}

long long MaxFlow::getFlow(Edge e) {
    long long flow = std::min(e.capacity, Net.getFlow(e.from, e.to));
    if (flow < 0) {
        flow = 0;
    }
    Net.changeFlow(e.from, e.to, -flow);
    return flow;
}

size_t MaxFlow::getNumberOfVertexes() const {
    return Net.getNumberOfVertexes();
}

size_t MaxFlow::getMaxHeight() const{
    return maxHeight;
}

long long MaxFlow::getStepCounter() const{
    return stepCounter;
}

Network* MaxFlow::getNetwork() {
    return &Net;
}

int PushRelable()
{
    size_t vertex_number, edges_number;
    size_t edge_from, edge_to, edge_capacity;

    cin >> vertex_number >> edges_number;

    Network net(vertex_number, 0, vertex_number - 1);

    vector <Edge> edges(edges_number);

    for (size_t i = 0; i < edges_number; i++) {
        cin >> edge_from >> edge_to >> edge_capacity;
        edge_from--;
        edge_to--;
        edges[i] = Edge(edge_from, edge_to, edge_capacity);
        net.addEdge(edge_from, edge_to, edge_capacity);
    }

    MaxFlow solution(net);

    cout << solution.getMaxFlowValue() << endl;

    for (size_t i = 0; i < edges_number; ++i) {
        cout << solution.getFlow(edges[i]) << endl;
    }

    return 0;
}
