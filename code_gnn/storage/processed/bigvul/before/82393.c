static NO_INLINE void jsvFreePtrInternal(JsVar *var) {
  assert(jsvGetLocks(var)==0);
  var->flags = JSV_UNUSED;
  jshInterruptOff();  
  jsvSetNextSibling(var, jsVarFirstEmpty);
  jsVarFirstEmpty = jsvGetRef(var);
  touchedFreeList = true;
  jshInterruptOn();
}
