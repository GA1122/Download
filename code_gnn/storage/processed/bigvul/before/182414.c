 int mp_unpack_full(lua_State *L, int limit, int offset) {
     size_t len;
     const char *s;
     mp_cur c;
     int cnt;  
     int decode_all = (!limit && !offset);
 
     s = luaL_checklstring(L,1,&len);  
 
     if (offset < 0 || limit < 0)  
         return luaL_error(L,
             "Invalid request to unpack with offset of %d and limit of %d.",
             offset, len);
     else if (offset > len)
         return luaL_error(L,
             "Start offset %d greater than input length %d.", offset, len);
 
     if (decode_all) limit = INT_MAX;
 
     mp_cur_init(&c,(const unsigned char *)s+offset,len-offset);
 
      
     for(cnt = 0; c.left > 0 && cnt < limit; cnt++) {
         mp_decode_to_lua_type(L,&c);
 
         if (c.err == MP_CUR_ERROR_EOF) {
             return luaL_error(L,"Missing bytes in input.");
         } else if (c.err == MP_CUR_ERROR_BADFMT) {
             return luaL_error(L,"Bad data format in input.");
         }
     }
 
     if (!decode_all) {
          
          int offset = len - c.left;
// 
//         luaL_checkstack(L, 1, "in function mp_unpack_full");
// 
           
          lua_pushinteger(L, c.left == 0 ? -1 : offset);
           
         lua_insert(L, 2);
         cnt += 1;  
     }
 
     return cnt;
 }