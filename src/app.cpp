#include "app.hpp"

App::App(): _grid(30),
            _a_star(_grid),
            _visualizer(_grid,_window,_a_star),
            _ui(_window, _a_star)

{
    // window
    _window.create(sf::VideoMode(800,800), "A*", sf::Style::Titlebar |
                                                 sf::Style::Close);
    _window.setFramerateLimit(30);
    _window.resetGLStates();

    // visualizer
    _visualizer.init();

    // UI
    _ui.eventPool[UI::Event::Start]    = std::bind(&A_star::start, &_a_star);
    _ui.eventPool[UI::Event::Pause]    = std::bind(&A_star::pause, &_a_star);
    _ui.eventPool[UI::Event::Continue] = std::bind(&A_star::start, &_a_star);
    _ui.eventPool[UI::Event::Restart]  = std::bind(&A_star::restart , &_a_star);
    
    _ui.eventPool[UI::Event::Clear_Wall] = std::bind(&Wall<Node<int>>::clear, &_grid.wall());
}

void App::processEvent()
{
    sf::Event event;

    sf::Vector2i mousePosition = sf::Mouse::getPosition(_window);

    static Operation operation {Operation::None};
    static iNode** movable;

    while(_window.pollEvent(event))
    {
        switch(event.type)
        {
            case(sf::Event::Closed):
                _window.close();
                break;

            case(sf::Event::KeyPressed):
                if(event.key.code == sf::Keyboard::Return)
                {
                    switch(_a_star.state())
                    {
                        case(A_star::State::Work):
                            _a_star.pause();
                            break;

                        case(A_star::State::Pause):
                            _a_star.restart();
                            break;

                        case(A_star::State::Stop):
                            _a_star.start();
                            break;
                    }
                }
                if(event.key.code == sf::Keyboard::Space)
                {
                    _a_star.pause();
                }
                break;

            case(sf::Event::MouseButtonReleased):
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    operation = Operation::None;                    
                }
                break;

            case(sf::Event::MouseButtonPressed):
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    if(!ImGui::GetIO().WantCaptureMouse)
                    {
                        sf::Vector2i cellCoord = _visualizer.getCellCoord(mousePosition);
                        if(_grid.inBounds(cellCoord.x, cellCoord.y))
                        {
                            iNode* cell = _grid.getNodeAdress(cellCoord.x, cellCoord.y);
                            if(_grid.canBeMoved(cell))
                            {
                                if(cell == _grid.start())
                                {
                                    movable = &_grid.start();
                                }
                                else if (cell == _grid.goal())
                                {
                                    movable = &_grid.goal();
                                }
                                operation = Operation::Move;                            
                            }

                            else
                            {
                                if(_grid.wall().contain(cell))
                                {
                                    _grid.wall().erase(cell);
                                    operation = Operation::Erase;                    
                                }
                                else
                                {
                                    _grid.wall().insert(cell);
                                    operation = Operation::Draw;  
                                }
                            }   
                        }
                    }                
                }
                break;
        }
    }

    if(_window.hasFocus() && inWindow(mousePosition))
    {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i cellCoord = _visualizer.getCellCoord(mousePosition);
            if(_grid.inBounds(cellCoord.x, cellCoord.y))
            {
                iNode* cell = _grid.getNodeAdress(cellCoord.x, cellCoord.y);

                switch(operation)
                {
                    case Operation::Move:
                        if(_grid.canBePlaced(cell))
                        {
                            *movable = cell;                            
                        }
                        break;
                    
                    case Operation::Draw:
                        if(_grid.canBeWalled(cell))
                        {
                            _grid.wall().insert(cell);
                        }
                        break;
                    
                    case Operation::Erase:
                        _grid.wall().erase(cell);
                        break;
                }
            }
        }
    }
}

void App::render()
{
    _window.clear(sf::Color(200,200,200));

    _visualizer.render();
}

void App::run()
{
    while(_window.isOpen())
    {
        // SFML
        processEvent();
        render();

        // ImGui 
        _ui.update();
        _ui.draw();

        _window.display();
    }
}

bool App::inWindow(sf::Vector2i& coord)
{
    sf::Vector2u windowSize = _window.getSize();
    return(coord.x > 0 && coord.x < windowSize.x &&
           coord.y > 0 && coord.y < windowSize.y);
}