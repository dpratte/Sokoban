#ifndef DEF_LUAWRAPPERS_H
#define DEF_LUAWRAPPERS_H
/** adds the C++ API functions to LUA scripts. */
void LUA_API skb_register_wrappers(lua_State* L);

int LUA_API skb_quit(lua_State* L);

#endif
