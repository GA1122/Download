static int req_aprtable2luatable_cb(void *l, const char *key,
                                    const char *value)
{
    int t;
    lua_State *L = (lua_State *) l;      
     
     
     

    lua_getfield(L, -1, key);    
     
    t = lua_type(L, -1);
    switch (t) {
    case LUA_TNIL:
    case LUA_TNONE:{
            lua_pop(L, 1);       
            lua_newtable(L);     
            lua_pushnumber(L, 1);        
            lua_pushstring(L, value);    
            lua_settable(L, -3);         
            lua_setfield(L, -2, key);    
            break;
        }
    case LUA_TTABLE:{
             
            int size = lua_rawlen(L, -1);
            lua_pushnumber(L, size + 1);         
            lua_pushstring(L, value);    
            lua_settable(L, -3);         
            lua_setfield(L, -2, key);    
            break;
        }
    }

     
     
    lua_getfield(L, -2, key);    
    if (lua_isnoneornil(L, -1)) {        
        lua_pop(L, 1);           
        lua_pushstring(L, value);        
        lua_setfield(L, -3, key);        
    }
    else {
        lua_pop(L, 1);
    }
    return 1;
}
