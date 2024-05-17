LUALIB_API int luaopen_cmsgpack_safe(lua_State *L) {
    int i;

    luaopen_cmsgpack(L);

     
    for (i = 0; i < (sizeof(cmds)/sizeof(*cmds) - 1); i++) {
        lua_getfield(L, -1, cmds[i].name);
        lua_pushcclosure(L, mp_safe, 1);
        lua_setfield(L, -2, cmds[i].name);
    }

#if LUA_VERSION_NUM < 502
     
    lua_pushvalue(L, -1);
    lua_setglobal(L, LUACMSGPACK_SAFE_NAME);
#endif

    return 1;
}