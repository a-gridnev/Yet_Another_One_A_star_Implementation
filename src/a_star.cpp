#include "a_star.hpp"

A_star::A_star(Graph<Node<int>>& graph): Process(State::Stop,
                                                 std::thread(&A_star::runThread, this)),
                                         _graph(&graph)
{}

void A_star::runThread()
{
    while(true)
    {
        switch(_state)
        {
            case(State::Work):
                run();
                _state != State::Restart? _state = State::Stop :
                                          _state = State::Work;
                break;

            case(State::Stop):
                sleep_for(_scanDelay);
                break;
        }
    }    
}

void A_star::run()
{

    lock();   // lock mutex ->
    clearResult(); 
    unlock(); // lock mutex <-

    _startNode = _graph->start();                                               // FIX IT
    _goalNode  = _graph->goal();                                                // FIX IT

    _frontier.push({0,_startNode});
    _cameFrom[_startNode] = nullptr;

    _cost_so_far[_startNode] = 0;
    _nodeCost[_startNode] = 0;


    while(!_frontier.empty())
    {  
        if(_state == State::Work)
        {
            iNode* current = _frontier.top().second;
            _frontier.pop();

            if(current == _goalNode)
            {
                createPath();
                break;
            }

            for(auto& next: _graph->neighbors().get(current))
            {
                if(!_graph->wall().contain(next))
                {
                    double costBetweenNodes = _graph->cost(current, next);

                    if(costBetweenNodes == std::numeric_limits<double>::max())
                    {
                        continue;
                    }

                    double new_cost = _cost_so_far[current] + costBetweenNodes;
                    
                    if(_cost_so_far.find(next) == _cost_so_far.end() ||
                       new_cost < _cost_so_far[next])
                    {
                        _cost_so_far[next] = new_cost;

                        double priority = new_cost + heuristic(_goalNode, next);

                        _frontier.push({priority,next});
                        _cameFrom[next] = current;

                        _nodeCost[next] = priority;
                    }      
                }
                sleep_for(_delay);
            }            
        }

        else if(_state == State::Pause)
        {
            sleep_for(_scanDelay);

            if(_state == State::Restart)
            {
                return;                
            }
        }
    }
}

void A_star::start()
{
    _state = State::Work;
}

// Little BUG: Restart algorithm if _state == State::Stop
void A_star::pause()
{
    _state == State::Work? _state = State::Pause : _state = State::Work;
}

void A_star::stop()
{
    _state = State::Stop;
}

void A_star::restart()
{
    _state = State::Restart;
}

Path_t A_star::createPath()
{
    iNode* current = _goalNode;

    while(current != _startNode)
    {
        _path.push_back(current);
        current = _cameFrom[current];
    }
}

void A_star::clearResult()
{
    _frontier.clear();
    _cameFrom.clear();
    _cost_so_far.clear();
    _nodeCost.clear();

    _path.clear();
}

void A_star::delay(const Delay& delay)
{
    _delay = delay;
}

const Delay A_star::delay() const
{
    return(_delay);
}

const iNode* A_star::startNode() const
{
    return(_startNode);
}

const iNode* A_star::goalNode() const
{
    return(_goalNode);
}


const Frontier_t& A_star::frontier() const
{
    return(_frontier);
}

const CameFrom_t& A_star::cameFrom() const
{
    return(_cameFrom);
}

const Path_t& A_star::path() const
{
    return(_path);
}

const Cost_t& A_star::nodeCost() const
{
    return(_nodeCost);
}