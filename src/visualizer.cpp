#include "Visualizer.hpp"

Visualizer::Visualizer(Graph<Node<int>>& graph,
                                   sf::RenderWindow& target,
                                   A_star& algoritm):
    _graph(graph),
    _target(target),
    _a_star(algoritm)
{
    if(!_font.loadFromFile("DroidSans.ttf"))
    {
        // TODO: add validation
    }
}

void Visualizer::init()
{
    // Cell
    // for window(800,800) and graph(11) -> gap = 4
    _gap = _target.getSize().x/((_graph.size()-1)*10*2);
    _cellSize = static_cast<float>(_target.getSize().x -
                                   _gap*(_graph.size()-1))/_graph.size();

    _cell.setSize(sf::Vector2f(_cellSize,_cellSize));

    // Base layer
    _baseTexture.create(_target.getSize().x, _target.getSize().y);
    
    drawBase(_baseTexture);

    // Cell Text
    _cellText.setFont(_font);
    _cellText.setCharacterSize(static_cast<unsigned int>(_cellSize/2));
    _cellText.setColor(sf::Color::White);
}

void Visualizer::render()
{
    // Draw base layer
    _target.draw(sf::Sprite(_baseTexture.getTexture()));

    // Draw algorithm data
    _a_star.lock(); // lock mutex ->

    for(auto& element: _a_star.cameFrom())
    {
        draw(element.first, CellType::Visited);
    }

    for(auto& element: _a_star.frontier())
    {
        draw(element.second, CellType::Frontier);
    }

    for(auto& element: _a_star.path())
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

    // Draw nodes weight
    for(auto& element: _a_star.nodeCost())
    {
        drawWeight(_target, element.first, element.second);
    }   

    _a_star.unlock(); // lock mutex <-

    // Draw wall
    for(auto& element: _graph.wall().get())
    {
        draw(element, CellType::Wall);
    }

    // Draw start and goal cells
    draw(_graph.start(), CellType::Start);
    draw(_graph.goal(), CellType::Stop);  
}

void Visualizer::draw(const iNode* cell,
                            const CellType type)
{
    draw(_target, cell, type);
}

void Visualizer::draw(sf::RenderTarget& target,
                            const iNode* cell,
                            const CellType type)
{
    _cell.setFillColor(CellColor[type]);

    float x = (_cellSize + _gap)*cell->x;
    float y = (_cellSize + _gap)*cell->y;

    _cell.setPosition(x,y);

    target.draw(_cell);
}

void Visualizer::drawWeight(sf::RenderTarget& target,
                                  const iNode* node,
                                  const double value)
{
    float x = (_cellSize + _gap)*node->x;
    float y = (_cellSize + _gap)*node->y;   

    std::string valueText;
    value != std::numeric_limits<double>::max()?
                valueText = std::to_string(static_cast<int>(value)) :
                valueText = '-';

    _cellText.setString(valueText);

    sf::FloatRect textRect = _cellText.getLocalBounds();
    _cellText.setOrigin(textRect.left +textRect.width/2.0f,
                        textRect.top  +textRect.height/2.0f);  

    _cellText.setPosition(x + _cellSize/2.0f, y + _cellSize/2.0f);

    _target.draw(_cellText);
}

void Visualizer::drawBase(sf::RenderTarget& target)
{
    for(auto row: _graph.data())
    {
        for(auto cell: row)
        {
            draw(target, &cell);
        }
    }    
}

sf::Vector2i Visualizer::getCellCoord(const sf::Vector2i& coord) const
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

bool Visualizer::isPointToCell(const sf::Vector2i& coord) const
{
    return(coord.x%(static_cast<int>(_cellSize) + _gap) < static_cast<int>(_cellSize) &&
           coord.y%(static_cast<int>(_cellSize) + _gap) < static_cast<int>(_cellSize));   
}