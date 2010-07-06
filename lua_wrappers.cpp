#include "stdafx.h"
#include "lua_wrappers.h"
#include "Application.h"

static const luaL_reg skb_funcs[] = {
    {"quit", skb_quit},
    {0, 0}
};
void LUA_API skb_register_wrappers(lua_State* L){
    luaL_openlib(L, "skb", skb_funcs, 0);
}

int LUA_API skb_quit(lua_State* L)
{
    Application* app = Application::instance();
    app->close();
    return 0;
}
