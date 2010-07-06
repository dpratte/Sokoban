#include "stdafx.h"
#include "lua_wrappers.h"
#include "Application.h"

void LUA_API skb_register_wrappers(lua_State* L){
    lua_register(L, "skb_quit", skb_quit);
}

int LUA_API skb_quit(lua_State* L)
{
    Application* app = Application::instance();
    app->close();
    return 0;
}
