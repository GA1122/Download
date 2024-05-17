static StkId callrethooks (lua_State *L, StkId firstResult) {
  ptrdiff_t fr = savestack(L, firstResult);   
  luaD_callhook(L, LUA_HOOKRET, -1);
  if (f_isLua(L->ci)) {   
    while ((L->hookmask & LUA_MASKRET) && L->ci->tailcalls--)  
      luaD_callhook(L, LUA_HOOKTAILRET, -1);
  }
  return restorestack(L, fr);
}
