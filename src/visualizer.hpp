#pragma once


#include "graph.hpp"
#include "node.hpp"
#include "a_star.hpp"

#include <SFML/Graphics.hpp>

#include <cmath>
#include <limits>
#include <string>
#include <unordered_map>


class Visualizer
{
public:
    enum class CoordSystem
    {
        Cartesian,
        Grid
    };

private:
    Graph<Node<int>>& _graph;
    sf::RenderWindow& _target;
    A_star& _a_star;

    sf::RenderTexture _baseTexture;

    sf::Font _font;
    sf::Text _cellText;

    sf::RectangleShape _cell;

    float _cellSize;
    int _gap;

    enum class CellType
    {
        Empty,
        Wall,
        Start,
        Stop,
        Start_shadow,
        Stop_shadow,
        Frontier,
        Visited,
        Path
    };

    std::unordered_map<CellType, sf::Color>  CellColor =
    {
        {CellType::Empty,       sf::Color::White},
        {CellType::Wall,        sf::Color::Black},
        {CellType::Start,       sf::Color::Green},
        {CellType::Stop,        sf::Color::Red},
        {CellType::Start_shadow,sf::Color(0,255,0,125)},
        {CellType::Stop_shadow, sf::Color(255,0,0,125)},
        {CellType::Frontier,    sf::Color::Blue},
        {CellType::Visited,     sf::Color::Cyan},
        {CellType::Path,        sf::Color(125,125,125)}
    };

public:
    Visualizer(Graph<Node<int>>&, sf::RenderWindow&, A_star&);

    void init();


    void render();

    void draw(const iNode*,
              const CellType=CellType::Empty);
    void draw(sf::RenderTarget&,
              const iNode*,
              const CellType=CellType::Empty);

    void drawBase(sf::RenderTarget&);

    void drawWeight(sf::RenderTarget& target,
                    const iNode*,
                    const double value);


    bool isPointToCell(const sf::Vector2i&) const;

    sf::Vector2i getCellCoord(const sf::Vector2i&) const;
};