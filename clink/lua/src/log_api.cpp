// Copyright (c) 2020 Martin Ridgers
// License: http://opensource.org/licenses/MIT

#include "pch.h"
#include "lua_state.h"

#include <core/base.h>
#include <core/log.h>
#include <core/str.h>

//------------------------------------------------------------------------------
/// -name:  log.info
/// -ver:   1.1.3
/// -arg:   message:string
/// Writes info <span class="arg">message</span> to the Clink log file.  Use
/// this sparingly, or it could cause performance problems or disk space
/// problems.
int log_info(lua_State* state)
{
    lua_Debug ar = {};
    lua_getstack(state, 1, &ar);
    lua_getinfo(state, "Sl", &ar);
    const char* source = ar.source ? ar.source : "?";
    int line = ar.currentline;

    const char* message = checkstring(state, 1);
    if (message)
        logger::info(source, line, "%s", message);

    return 0;
}

//------------------------------------------------------------------------------
void log_lua_initialise(lua_state& lua)
{
    struct {
        const char* name;
        int         (*method)(lua_State*);
    } methods[] = {
        { "info",        &log_info },
    };

    lua_State* state = lua.get_state();

    lua_createtable(state, sizeof_array(methods), 0);

    for (const auto& method : methods)
    {
        lua_pushstring(state, method.name);
        lua_pushcfunction(state, method.method);
        lua_rawset(state, -3);
    }

    lua_setglobal(state, "log");
}
