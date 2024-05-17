void luaD_growstack (lua_State *L, int n) {
  if (n <= L->stacksize)   
    luaD_reallocstack(L, 2*L->stacksize);
  else
    luaD_reallocstack(L, L->stacksize + n);
}
