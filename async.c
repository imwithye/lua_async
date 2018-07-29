#include <uv.h>
#include <stdlib.h>
#include "async.h"

typedef struct
{
    int cb_ref;
    lua_State *L;
} time_handler_data;

void uv_timer_cb_handler(uv_timer_t *hander)
{
    time_handler_data *handler_data = (time_handler_data *)(hander->data);
    lua_rawgeti(handler_data->L, LUA_REGISTRYINDEX, handler_data->cb_ref);
    lua_pcall(handler_data->L, 0, 0, 0);
}

static int settimeout(lua_State *L)
{
    size_t timeout = luaL_checkinteger(L, 1);
    time_handler_data *handler_data = malloc(sizeof(time_handler_data));
    handler_data->cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    handler_data->L = L;
    uv_timer_t *handler = malloc(sizeof(uv_timer_t));
    handler->data = handler_data;
    uv_timer_init(uv_default_loop(), handler);
    uv_timer_start(handler, uv_timer_cb_handler, timeout, 0);
    lua_pushlightuserdata(L, handler);
    return 1;
}

static int cleartimeout(lua_State *L)
{
    uv_timer_t *handler = (uv_timer_t *)lua_touserdata(L, 2);
    uv_timer_stop(handler);
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