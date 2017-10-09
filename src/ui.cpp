#include "ui.hpp"

UI::UI(sf::RenderWindow& window, Process& process): _window(window),
                                                    _process(process)
{
    ImGui::SFML::Init(_window);

    // Load Fonts
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->AddFontDefault();

    ImFontConfig config;
    const ImWchar icon_ranges[] = {ICON_MIN, ICON_MAX, 0 };
    extern const unsigned int icon_compressed_size;
    extern const unsigned int icon_compressed_data[];

    io.Fonts->AddFontFromMemoryCompressedTTF(icon_compressed_data, icon_compressed_size, 20.0f, &config, icon_ranges);
    io.Fonts->AddFontFromMemoryCompressedTTF(icon_compressed_data, icon_compressed_size, 36.0f, &config, icon_ranges);

    io.Fonts->AddFontFromFileTTF("DroidSans.ttf",24.f);
    io.Fonts->AddFontFromFileTTF("DroidSans.ttf",28.f);

    ImGui::SFML::UpdateFontTexture();

    // add Event "hide UI"
    eventPool[Event::Hide_UI] = std::bind(&hide, this);
}


void UI::update()
{
    static sf::Clock delta;

    ImGui::SFML::Update(_window, delta.restart());
}

void UI::draw()
{
// Elements positions and sizes
    // y 
    // ^  x1            x2       x3
    // |  .-------------.--------.-------------|
    // |  |             |        |             |
    // |  ._____w1____. .__w3__. .__w3__. -----| y1
    // |  |           | |      | |      | h2   |
    // |  | h1        | .______. .______.      |
    // |  |           | ._______w2______.------| y2
    // |  |           | |               | h3   |
    // |  .___________. ._______________.      |
    // | /
    // |/_indent________________________________-> x


    static ImVec2 indent(10,20);
    static ImVec2 gap(6,6);
    
    static float h1 {100};
    static float w1 {220};

    static float h2 {0.6*h1 - gap.y/2};
    static float w2 {140};

    static float h3 {h1 - h2 - gap.y};
    static float w3 {w2/2 - gap.x/2};

    static float x1 {indent.x};
    static float x2 {x1 + w1 + gap.x};
    static float x3 {x2 + w3 + gap.x};

    static float y1 {_window.getSize().y - (indent.y + h1)};
    static float y2 {_window.getSize().y - (indent.y + h3)};
 
// Window flags
    static auto  uiWindowFlags = ImGuiWindowFlags_NoTitleBar|
                                 ImGuiWindowFlags_NoResize|
                                 ImGuiWindowFlags_NoMove|
                                 ImGuiWindowFlags_NoCollapse;

// Windows and buttons rounding
    static float rounding = 10.0f;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,  rounding);

// Button style
    ImGui::PushStyleColor(ImGuiCol_Button,        ImColor());
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(0,0,0,50));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImColor(0,0,0,100));

// Fonts
    static ImFont*& fontIcon_20px      = ImGui::GetIO().Fonts->Fonts[1];
    static ImFont*& fontIcon_36px      = ImGui::GetIO().Fonts->Fonts[2];
    static ImFont*& fontDroidSans_24px = ImGui::GetIO().Fonts->Fonts[3];
    static ImFont*& fontDroidSans_28px = ImGui::GetIO().Fonts->Fonts[4];


// Draw GUI
    if(visible)
    {
        // Info window
        ImGui::SetNextWindowPos(ImVec2(x1,y1));
        ImGui::SetNextWindowSize(ImVec2(w1,h1));

        ImGui::PushFont(fontDroidSans_24px);

        ImGui::Begin("##info", NULL, uiWindowFlags);
        ImGui::Text("%s: %.3f",   "Length", 15.6487);
        ImGui::Text("%s: %.3f ms","Time", 0.253);
        ImGui::Text("%s: %d",     "Operations", 123);
        ImGui::End();

        ImGui::PopFont();


        // Buttons
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,  ImVec2(0,0));

        ImGui::PushFont(fontIcon_36px);
        
        switch(_process.state())
        {
            case(Process::State::Stop):
                ImGui::SetNextWindowPos(ImVec2(x2,y1));
                ImGui::SetNextWindowSize(ImVec2(w2,h2));

                button(Event::Start);
                break;

            case(Process::State::Work):
                ImGui::SetNextWindowPos(ImVec2(x2,y1));
                ImGui::SetNextWindowSize(ImVec2(w2,h2));

                button(Event::Pause);
                break;

            case(Process::State::Pause):
                ImGui::SetNextWindowPos(ImVec2(x2,y1));
                ImGui::SetNextWindowSize(ImVec2(w3,h2));
                
                button(Event::Continue);


                ImGui::SetNextWindowPos(ImVec2(x3,y1));
                ImGui::SetNextWindowSize(ImVec2(w3,h2));

                button(Event::Restart);
                break;
        }

        ImGui::PopFont();


        ImGui::PushFont(fontDroidSans_28px);

        ImGui::SetNextWindowPos(ImVec2(x2,y2));
        ImGui::SetNextWindowSize(ImVec2(w2,h3));

        button(Event::Clear_Wall);

        ImGui::PopFont();


        ImGui::PopStyleVar(); // Padding

    }

    ImGui::PopStyleVar(2); // Rounding


    // "Hide UI" button
    static int hideUIrounding = 7;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, hideUIrounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,  hideUIrounding);
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,  ImVec2(0,0));

    ImGui::PushFont(fontIcon_20px);

    static ImVec2 hideUIwinSize(ImVec2(27,27));
    static ImVec2 hideUIwinPos(_window.getSize().x - hideUIwinSize.x ,
                               _window.getSize().y - hideUIwinSize.y);

    ImGui::SetNextWindowPos(hideUIwinPos);
    ImGui::SetNextWindowSize(hideUIwinSize);

    button(Event::Hide_UI);

    ImGui::PopFont();

    ImGui::PopStyleVar(3);


    ImGui::PopStyleColor(3); // Buttons color

    // Render UI
    ImGui::Render();
}

void UI::hide()
{
    // show_UI? show_UI = false : show_UI = true;
    visible = !visible;
}

void UI::button(Event event)
{
    const char* windowName;
    const char* buttonText;

    switch(event)
    {
        case(Event::Start):
            windowName = "##start";
            buttonText = ICON_START;
            break;

        case(Event::Pause):
            windowName = "##pause";
            buttonText = ICON_PAUSE;
            break;

        case(Event::Continue):
            windowName = "##continue";
            buttonText = ICON_START;
            break;

        case(Event::Restart):
            windowName = "##restart";
            buttonText = ICON_RESTART;
            break;

        case(Event::Clear_Wall):
            windowName = "##clean_wall";
            buttonText = "clear walls";
            break;

        case(Event::Hide_UI):
            windowName = "##hide_ui";
            buttonText = "";
            break;
    }

    ImGui::Begin(windowName, NULL, ImGuiWindowFlags_NoTitleBar|
                                   ImGuiWindowFlags_NoResize|
                                   ImGuiWindowFlags_NoMove|
                                   ImGuiWindowFlags_NoCollapse);

    ImGui::SetCursorPos(ImVec2(0,0));

    // small hack: special for Event::Hide_UI that has "conditional icon"
    if(event == Event::Hide_UI)
    {
        buttonText = ImGui::IsWindowHovered() &&  visible ||
                    !ImGui::IsWindowHovered() && !visible? ICON_UI_HIDDEN :
                                                           ICON_UI_VISIBLE;
    }

    if(ImGui::Button(buttonText, ImVec2(ImGui::GetWindowWidth(),ImGui::GetWindowHeight())))
    {
       eventPool[event]();
    }

    ImGui::End();    
}