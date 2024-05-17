int table_is_an_array(lua_State *L) {
    int count = 0, max = 0;
#if LUA_VERSION_NUM < 503
    lua_Number n;
#else
    lua_Integer n;
#endif

     
    int stacktop;

    stacktop = lua_gettop(L);

    lua_pushnil(L);
    while(lua_next(L,-2)) {
         
        lua_pop(L,1);  
         
#if LUA_VERSION_NUM < 503
        if ((LUA_TNUMBER != lua_type(L,-1)) || (n = lua_tonumber(L, -1)) <= 0 ||
            !IS_INT_EQUIVALENT(n))
#else
        if (!lua_isinteger(L,-1) || (n = lua_tointeger(L, -1)) <= 0)
#endif
        {
            lua_settop(L, stacktop);
            return 0;
        }
        max = (n > max ? n : max);
        count++;
    }
     
    lua_settop(L, stacktop);
    return max == count;
}
