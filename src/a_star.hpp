#pragma once


#include "process.hpp"

#include "node.hpp"
#include "graph.hpp"

#include "iterable_priority_queue.hpp"
#include "heuristics.hpp"

#include <vector>
#include <unordered_map>

#include <functional>
#include <limits>

using Frontier_t = iterable_priority_queue<std::pair<double, iNode*>>;
using CameFrom_t = std::unordered_map<iNode*,iNode*>;
using Path_t     = std::vector<iNode*>;
using Cost_t     = std::unordered_map<iNode*, double>;

using Heuristic_t = std::function<double(const iNode*, const iNode*)>;

using Delay      = std::chrono::milliseconds;


class A_star: public Process
{
    Graph<Node<int>>* _graph {nullptr};

    iNode* _startNode {nullptr};
    iNode* _goalNode  {nullptr};

    Delay _delay {10};
    Delay _scanDelay {100};

    Heuristic_t heuristic = manhattan<iNode>;

    Frontier_t  _frontier;
    CameFrom_t  _cameFrom;
    Cost_t      _cost_so_far;
    Cost_t      _nodeCost;

    Path_t      _path;

public:
    A_star(Graph<Node<int>>&);

    void run();
    void runThread();

    void start();
    void pause();
    void stop();
    void restart();

    Path_t createPath();
    void clearResult();

    void delay(const Delay&);

    const Delay delay() const;

    const iNode* startNode() const;
    const iNode* goalNode()  const;

    const Frontier_t& frontier() const;
    const CameFrom_t& cameFrom() const;
    const Path_t&     path()     const;

    const Cost_t&     nodeCost() const;
};