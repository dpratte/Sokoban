#include "stdafx.h"
#include "lua_wrappers.h"
#include "Application.h"

Application* Application::_instance = new Application();

Application::Application()
: _stop(false),
_eventStringMap()
{
    _state = lua_open();
    luaL_openlibs(_state);

    mapEventTable();
    skb_register_wrappers(_state);
    //ctor
}

void Application::mapEventTable()
{
    //TODO: See if a better way of doing this mapping is available...
    _eventStringMap.insert(std::pair<int, const char*>(sf::Event::Closed,               "CANVAS_CLOSED"));
    _eventStringMap.insert(std::pair<int, const char*>(sf::Event::Resized,              "CANVAS_RESIZED"));
    _eventStringMap.insert(std::pair<int, const char*>(sf::Event::MouseMoved,           "CANVAS_MOUSE_MOVED"));
    _eventStringMap.insert(std::pair<int, const char*>(sf::Event::MouseLeft,            "CANVAS_MOUSE_LEFT"));
    _eventStringMap.insert(std::pair<int, const char*>(sf::Event::MouseEntered,         "CANVAS_MOUSE_ENTERED"));
    _eventStringMap.insert(std::pair<int, const char*>(sf::Event::MouseButtonPressed,   "CANVAS_MOUSE_CLICK"));
    _eventStringMap.insert(std::pair<int, const char*>(sf::Event::MouseWheelMoved,      "CANVAS_MOUSE_WHEEL"));
    _eventStringMap.insert(std::pair<int, const char*>(sf::Event::MouseButtonReleased,  "CANVAS_MOUSE_RELEASED"));
    _eventStringMap.insert(std::pair<int, const char*>(sf::Event::KeyPressed,           "CANVAS_KEY_PRESSED"));
    _eventStringMap.insert(std::pair<int, const char*>(sf::Event::KeyReleased,          "CANVAS_KEY_RELEASED"));
    _eventStringMap.insert(std::pair<int, const char*>(sf::Event::TextEntered,          "CANVAS_TEXT_ENTERED"));
    _eventStringMap.insert(std::pair<int, const char*>(sf::Event::LostFocus,            "CANVAS_FOCUS_LOST"));
    _eventStringMap.insert(std::pair<int, const char*>(sf::Event::GainedFocus,          "CANVAS_FOCUS_GAINED"));
    _eventStringMap.insert(std::pair<int, const char*>(sf::Event::JoyButtonPressed,     "CANVAS_JOYPAD_PRESSED"));
    _eventStringMap.insert(std::pair<int, const char*>(sf::Event::JoyButtonReleased,    "CANVAS_JOYPAD_RELEASED"));
    _eventStringMap.insert(std::pair<int, const char*>(sf::Event::JoyMoved,             "CANVAS_JOYPAD_MOVED"));

}

Application* Application::instance()
{
    return Application::_instance;
}

Application::~Application()
{
    lua_close(_state);
    //dtor
}

void Application::close()
{
    raiseLuaEvent("SKB_QUIT");
    _stop = true;
}

void Application::raiseLuaEvent(const char *name)
{
    lua_pushstring(_state, "skb_OnEvent");
    lua_gettable(_state, LUA_GLOBALSINDEX);
    lua_pushstring(_state, name);
    lua_call(_state, 1, 1);

    lua_pop(_state, 1);
}

sf::Window* Application::canvas()
{
    return window;
}
lua_State* Application::luaState()
{
    return _state;
}
int Application::exec(int argc, char* argv[])
{
    std::cout << "Loading the main configuration file..." << std::endl;
    lua_State* L = _state;

    if (luaL_loadfile(L, "main_init.lua") || lua_pcall(L, 0, 0, 0))
    {
        printf("error: %s", lua_tostring(L, -1));
        return -1;
    }
    if (luaL_loadfile(L, "events.lua") || lua_pcall(L, 0, 0, 0))
    {
        printf("error: %s", lua_tostring(L, -1));
        return -1;
    }

    lua_getglobal(_state, "skb_window_width");
    lua_getglobal(_state, "skb_window_height");
    lua_getglobal(_state, "skb_window_depth");
    lua_getglobal(_state, "skb_window_title");

    if(!lua_isnumber(_state, -4)){
        std::cout << "invalid data for skb_window_width. " << std::endl;
        return 0;
    }
    if(!lua_isnumber(_state, -3)){
        std::cout << "invalid data for skb_window_height. " << std::endl;
        return 0;
    }
    if(!lua_isnumber(_state, -2)){
        std::cout << "invalid data for skb_window_depth. " << std::endl;
        return 0;
    }
    if(!lua_isstring(_state, -1)){
        std::cout << "invalid data for skb_window_title. " << std::endl;
        return 0;
    }
    std::string skb_window_title(lua_tostring(_state, -1));
    int skb_window_depth = (int)lua_tonumber(_state, -2);
    int skb_window_height = (int)lua_tonumber(_state, -3);
    int skb_window_width = (int)lua_tonumber(_state, -4);

    lua_pop(_state, 1);
    lua_pop(_state, 1);
    lua_pop(_state, 1);
    lua_pop(_state, 1);
    window = new sf::Window(sf::VideoMode(skb_window_width, skb_window_height, skb_window_depth), skb_window_title);

    while (!_stop)
    {
        sf::Event Event;
        while (window->GetEvent(Event))
        {
            std::map<int, const char*>::iterator luaName = _eventStringMap.find(Event.Type);

            if(luaName == _eventStringMap.end()){
                std::cout << "We havent mapped event with ID " << Event.Type << " to a lua-friendly version..." << std::endl;
                continue;
            }
            raiseLuaEvent(luaName->second);
            //If caused a SKB_QUIT, stop processing immediately.
            if(_stop)
                break;
        }

        window->Display();
    }


    return 0;
}
