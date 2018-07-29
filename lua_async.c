#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
#include <uv.h>
#include <stdio.h>

LUAMOD_API int luaopen_async(lua_State *L);

int main(int argc, const char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "cannot start: expect 1 file but get %d\n", argc - 1);
        return 1;
    }
    lua_State *L = luaL_newstate(); /* create state */
    if (L == NULL)
    {
        fprintf(stderr, "cannot create state: not enough memory\n");
        return 1;
    }
    luaL_openlibs(L);
    luaL_requiref(L, "async", luaopen_async, 1);
    int result = luaL_dofile(L, argv[1]);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    if (result != LUA_OK)
    {
        const char *msg = lua_tostring(L, -1);
        fprintf(stderr, "%s\n", msg);
        lua_pop(L, 1);
    }
    lua_close(L);
    return result;
}
