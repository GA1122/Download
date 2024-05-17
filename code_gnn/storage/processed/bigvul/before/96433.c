void mp_encode_lua_table_as_map(lua_State *L, mp_buf *buf, int level) {
    size_t len = 0;

     
    lua_pushnil(L);
    while(lua_next(L,-2)) {
        lua_pop(L,1);  
        len++;
    }

     
    mp_encode_map(L,buf,len);
    lua_pushnil(L);
    while(lua_next(L,-2)) {
         
        lua_pushvalue(L,-2);  
        mp_encode_lua_type(L,buf,level+1);  
        mp_encode_lua_type(L,buf,level+1);  
    }
}
