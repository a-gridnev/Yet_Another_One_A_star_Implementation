#pragma once


#include "graph.hpp"
#include "node.hpp"
#include "a_star.hpp"

#include <SFML/Graphics.hpp>

#include <cmath>


class Graph_visualizer
{
public:
    enum class CoordSystem
    {
        Cartesian,
        Grid
    };

private:
    Graph<Node<int>>& _graph;
    A_star& _a_star;
    sf::RenderWindow& _target;

    sf::RectangleShape _cell;

    float _cellSize;
    int _gap;

    sf::RenderTexture _baseTexture;

    sf::Font _font;

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

public:
    Graph_visualizer(Graph<Node<int>>&, sf::RenderWindow&, A_star&);

    void init();

    void render();

    void draw(const iNode*, CellType=CellType::Empty);
    void draw(sf::RenderTarget&, const iNode*, CellType=CellType::Empty);

    void drawWeight(sf::RenderTarget& target, const iNode*, const double value);
    void drawBase(sf::RenderTarget&);

    sf::Vector2i getCellCoord(sf::Vector2i);

    bool isPointToCell(sf::Vector2i&);
};