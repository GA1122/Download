int mp_unpack_one(lua_State *L) {
    int offset = luaL_optinteger(L, 2, 0);
     
    lua_pop(L, lua_gettop(L)-1);
    return mp_unpack_full(L, 1, offset);
}