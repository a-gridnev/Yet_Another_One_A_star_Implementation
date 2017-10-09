#pragma once 


#include "process.hpp"
#include "icon_code.hpp"

#include <imgui.h>
#include <imgui-sfml.h>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <functional>
#include <string>
#include <unordered_map>


class UI
{
public:
    enum class Event
    {
        Start,
        Pause,
        Stop,
        Continue,
        Restart,
        Clear_Wall,
        Hide_UI
    };

    std::unordered_map<Event, std::function<void()>> eventPool;

private:
    sf::RenderWindow& _window;
    Process& _process;

    bool visible {true};

public:
     UI(sf::RenderWindow&, Process&);
     
     void update();
     void draw();

     void hide();

private:
    void button(Event);
}; 