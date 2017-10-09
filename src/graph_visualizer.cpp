#include "graph_visualizer.hpp"
#include <string>
#include <limits>

Graph_visualizer::Graph_visualizer(Graph<Node<int>>& graph, sf::RenderWindow& target, A_star& algoritm):
    _graph(graph),
    _target(target),
    _a_star(algoritm)
{
    _font.loadFromFile("DroidSans.ttf");
}

void Graph_visualizer::init()
{
    // REAL value for final application (uncomment this)
    // for window(800,800) and graph(11) -> gap = 4
    _gap = _target.getSize().x/((_graph.size()-1)*10*2);
    
    // TEMP value for test cell detection algoritm (isPointToCell)
    // _gap = 20;
    _cellSize = static_cast<float>(_target.getSize().x - _gap*(_graph.size()-1))/_graph.size();
    _cell.setSize(sf::Vector2f(_cellSize,_cellSize));

    _baseTexture.create(_target.getSize().x, _target.getSize().y);
    drawBase(_baseTexture);
}

void Graph_visualizer::render()
{
    // Draw field
    _target.draw(sf::Sprite(_baseTexture .getTexture()));

    // Draw algorithm data
    _a_star.lock(); // lock mutex ->

    for(auto element: _a_star.cameFrom())
    {
        draw(element.first, CellType::Visited);
    }

    for(auto element: _a_star.frontier())
    {
        draw(element.second, CellType::Frontier);
    }

    for(auto element: _a_star.path())
    {
        draw(element, CellType::Path);
    }

    if(_a_star.startNode())
    {
        draw(_a_star.startNode(), CellType::Start_shadow);
    }

    if(_a_star.goalNode())
    {
        draw(_a_star.goalNode(), CellType::Stop_shadow);
    }

// TEMP ->
    for(auto& element: _a_star.nodeCost())
    {
        drawWeight(_target, element.first, element.second);
    }   
// TEMP <-

    _a_star.unlock(); // lock mutex <-

    // Draw wall
    for(auto element: _graph.wall().get())
    {
        draw(element, CellType::Wall);
    }

    // Draw start and goal cells
    draw(_graph.start(), CellType::Start);
    draw(_graph.goal(), CellType::Stop);  
}

void Graph_visualizer::draw(const iNode* cell, CellType type)
{
    draw(_target, cell, type);
}

void Graph_visualizer::draw(sf::RenderTarget& target, const iNode* cell, CellType type)
{
    sf::Color color;
    switch(type)
    {
        case(CellType::Empty):
            color = sf::Color::White;
            break;
        case(CellType::Wall):
            color = sf::Color::Black;
            break;
        case(CellType::Start):
            color = sf::Color::Green;
            break;
        case(CellType::Stop):
            color = sf::Color::Red;
            break;
        case(CellType::Start_shadow):
            color = sf::Color(0,255,0,125);
            break;
        case(CellType::Stop_shadow):
            color = sf::Color(255,0,0,125);
            break;
        case(CellType::Frontier):
            color = sf::Color::Blue;
            break;
        case(CellType::Visited):
            color = sf::Color::Cyan;
            break;
        case(CellType::Path):
            color = sf::Color(125,125,125);
            break;
    }
    _cell.setFillColor(color);

    float x = (_cellSize + _gap)*cell->x;
    float y = (_cellSize + _gap)*cell->y;

    _cell.setPosition(x,y);
    target.draw(_cell);

    _cell.setFillColor(sf::Color::White);
}

void Graph_visualizer::drawWeight(sf::RenderTarget& target, const iNode* node, const double value)
{
    float x = (_cellSize + _gap)*node->x;
    float y = (_cellSize + _gap)*node->y;   

    std::string valueText;
    value != std::numeric_limits<double>::max()? valueText = std::to_string(static_cast<int>(value)) :
                                                 valueText = '-';

    sf::Text text(valueText, _font);

// TEMP
    text.setCharacterSize(14);
// TEMP
    // text.setCharacterSize(static_cast<unsigned int>(_cellSize/2));
    text.setColor(sf::Color::White);

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left +textRect.width/2.0f,
                   textRect.top  +textRect.height/2.0f);    
    text.setPosition(x + _cellSize/2.0f, y + _cellSize/2.0f);

    _target.draw(text);
}

void Graph_visualizer::drawBase(sf::RenderTarget& target)
{
    for(auto row: _graph.data())
    {
        for(auto cell: row)
        {
            draw(_baseTexture, &cell);
        }
    }    
}

bool Graph_visualizer::isPointToCell(sf::Vector2i& coord)
{
    return(coord.x%(static_cast<int>(_cellSize) + _gap) < static_cast<int>(_cellSize) &&
           coord.y%(static_cast<int>(_cellSize) + _gap) < static_cast<int>(_cellSize));   
}

sf::Vector2i Graph_visualizer::getCellCoord(sf::Vector2i coord)
{
    if(isPointToCell(coord))
    {
        int x = (coord.x*_graph.size())/_target.getSize().x;
        int y = (coord.y*_graph.size())/_target.getSize().y;

        if(_graph.inBounds(x, y))
        {
            return(sf::Vector2i(x,y));            
        }
    }

    return(sf::Vector2i(-1,-1));
}