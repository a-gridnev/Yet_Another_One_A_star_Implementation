#pragma once


#include "node.hpp"
#include "graph.hpp"
#include "graph_visualizer.hpp"
#include "a_star.hpp"
#include "ui.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <functional>


class App
{
private:
    sf::RenderWindow _window;

    Graph<Node<int>> _grid;
    Graph_visualizer _visualizer;
    UI               _ui;
    A_star           _a_star;

    enum class Operation
    {
        None,
        Move,
        Draw,
        Erase
    };

public:
    App();
    void run();
    void processEvent();
    void render();

    bool inWindow(sf::Vector2i&);
};