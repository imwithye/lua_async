#include <uv.h>
#include <stdlib.h>
#include <stdio.h>
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>

typedef struct
{
    int cb_ref;
    lua_State *L;
    int is_interval;
} time_handler_data;

void free_handler(uv_timer_t *handler)
{
    free(handler->data);
    free(handler);
}

void uv_timer_cb_handler(uv_timer_t *handler)
{
    time_handler_data *handler_data = (time_handler_data *)(handler->data);
    lua_rawgeti(handler_data->L, LUA_REGISTRYINDEX, handler_data->cb_ref);
    lua_pcall(handler_data->L, 0, 0, 0);
    if (!handler_data->is_interval)
    {
        free_handler(handler);
    }
}

static int settimeout(lua_State *L)
{
    size_t timeout = luaL_checkinteger(L, 1);
    time_handler_data *handler_data = malloc(sizeof(time_handler_data));
    handler_data->cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    handler_data->L = L;
    handler_data->is_interval = 0;
    uv_timer_t *handler = malloc(sizeof(uv_timer_t));
    handler->data = handler_data;
    uv_timer_init(uv_default_loop(), handler);
    uv_timer_start(handler, uv_timer_cb_handler, timeout, 0);
    lua_pushlightuserdata(L, handler);
    return 1;
}

static int cleartimeout(lua_State *L)
{
    uv_timer_t *handler = (uv_timer_t *)lua_touserdata(L, 1);
    uv_timer_stop(handler);
    free(handler->data);
    free(handler);
    return 0;
}

static int setinterval(lua_State *L)
{
    size_t interval = luaL_checkinteger(L, 1);
    time_handler_data *handler_data = malloc(sizeof(time_handler_data));
    handler_data->cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    handler_data->L = L;
    handler_data->is_interval = 1;
    uv_timer_t *handler = malloc(sizeof(uv_timer_t));
    handler->data = handler_data;
    uv_timer_init(uv_default_loop(), handler);
    uv_timer_start(handler, uv_timer_cb_handler, 0, interval);
    lua_pushlightuserdata(L, handler);
    return 1;
}

static int clearinterval(lua_State *L)
{
    uv_timer_t *handler = (uv_timer_t *)lua_touserdata(L, 1);
    uv_timer_stop(handler);
    free_handler(handler);
    return 0;
}

static const luaL_Reg async[] = {
    {"settimeout", settimeout},
    {"cleartimeout", cleartimeout},
    {"setinterval", setinterval},
    {"clearinterval", clearinterval},
    {NULL, NULL}};

LUAMOD_API int luaopen_async(lua_State *L)
{
    luaL_newlib(L, async);
    return 1;
}