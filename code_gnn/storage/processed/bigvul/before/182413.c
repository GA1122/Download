 int mp_pack(lua_State *L) {
     int nargs = lua_gettop(L);
     int i;
     mp_buf *buf;
 
     if (nargs == 0)
         return luaL_argerror(L, 0, "MessagePack pack needs input.");
 
     if (!lua_checkstack(L, nargs))
         return luaL_argerror(L, 0, "Too many arguments for MessagePack pack.");
 
     buf = mp_buf_new(L);
      for(i = 1; i <= nargs; i++) {
           
//         luaL_checkstack(L, 1, "in function mp_check");
          lua_pushvalue(L, i);
  
          mp_encode_lua_type(L,buf,0);
 
         lua_pushlstring(L,(char*)buf->b,buf->len);
 
          
         buf->free += buf->len;
         buf->len = 0;
     }
     mp_buf_free(L, buf);
 
      
     lua_concat(L, nargs);
     return 1;
 }