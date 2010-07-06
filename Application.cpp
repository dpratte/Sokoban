#include "stdafx.h"
#include "Application.h"
static LUA_API int foo (lua_State *L) {
    int n = lua_gettop(L);    /* number of arguments */
    lua_Number sum = 0;
    int i;
    for (i = 1; i <= n; i++) {
        if (!lua_isnumber(L, i)) {
            lua_pushstring(L, "incorrect argument to function `average'");
            lua_error(L);
        }
        sum += lua_tonumber(L, i);
    }
    lua_pushnumber(L, sum/n);        /* first result */
    lua_pushnumber(L, sum);         /* second result */
    return 2;                   /* number of results */
}
Application* Application::_instance = new Application();

Application::Application()
{
    _state = lua_open();
    luaL_openlibs(_state);
    lua_register(_state, "average", foo);
    //ctor
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

int Application::exec(int argc, char* argv[])
{
    std::cout << "This is a test." << std::endl;
    luaL_dofile(_state, "main_init.lua");
    return 0;
}
