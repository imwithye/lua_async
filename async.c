#include "async.h"

static int settimeout(lua_State *L)
{
    return 0;
}

static int cleartimeout(lua_State *L)
{
    return 0;
}

static const luaL_Reg async[] = {
    {"settimeout", settimeout},
    {"cleartimeout", cleartimeout},
    {NULL, NULL}};

LUAMOD_API int luaopen_async(lua_State *L)
{
    luaL_newlib(L, async);
    return 1;
}