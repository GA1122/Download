int jsvGarbageCollect() {
  if (isMemoryBusy) return false;
  isMemoryBusy = MEMBUSY_GC;
  JsVarRef i;
  for (i=1;i<=jsVarsSize;i++)  {
    JsVar *var = jsvGetAddressOf(i);
    if ((var->flags&JSV_VARTYPEMASK) != JSV_UNUSED) {  
      var->flags |= (JsVarFlags)JSV_GARBAGE_COLLECT;
      if (jsvIsFlatString(var))
        i = (JsVarRef)(i+jsvGetFlatStringBlocks(var));
    }
  }
   
  for (i=1;i<=jsVarsSize;i++)  {
    JsVar *var = jsvGetAddressOf(i);
    if ((var->flags & JSV_GARBAGE_COLLECT) &&  
        jsvGetLocks(var)>0)  
      jsvGarbageCollectMarkUsed(var);
    if (jsvIsFlatString(var))
      i = (JsVarRef)(i+jsvGetFlatStringBlocks(var));
  }
   
  unsigned int freedCount = 0;
  jsVarFirstEmpty = 0;
  JsVar *lastEmpty = 0;
  for (i=1;i<=jsVarsSize;i++)  {
    JsVar *var = jsvGetAddressOf(i);
    if (var->flags & JSV_GARBAGE_COLLECT) {
      if (jsvIsFlatString(var)) {
        unsigned int count = (unsigned int)jsvGetFlatStringBlocks(var);
        freedCount+=count;
        var->flags = JSV_UNUSED;
        if (lastEmpty) jsvSetNextSibling(lastEmpty, i);
        else jsVarFirstEmpty = i;
        lastEmpty = var;
        while (count-- > 0) {
          i++;
          var = jsvGetAddressOf((JsVarRef)(i));
          var->flags = JSV_UNUSED;
          if (lastEmpty) jsvSetNextSibling(lastEmpty, i);
          else jsVarFirstEmpty = i;
          lastEmpty = var;
        }
      } else {
        if (jsvHasSingleChild(var)) {
           
          JsVarRef ch = jsvGetFirstChild(var);
          if (ch) {
            JsVar *child = jsvGetAddressOf(ch);  
            if (child->flags!=JSV_UNUSED &&  
                !(child->flags&JSV_GARBAGE_COLLECT))  
              jsvUnRef(child);
          }
        }
         
        assert(!jsvHasChildren(var) || !jsvGetFirstChild(var) ||
            jsvGetAddressOf(jsvGetFirstChild(var))->flags==JSV_UNUSED ||
            (jsvGetAddressOf(jsvGetFirstChild(var))->flags&JSV_GARBAGE_COLLECT));
        assert(!jsvHasChildren(var) || !jsvGetLastChild(var) ||
            jsvGetAddressOf(jsvGetLastChild(var))->flags==JSV_UNUSED ||
            (jsvGetAddressOf(jsvGetLastChild(var))->flags&JSV_GARBAGE_COLLECT));
        assert(!jsvIsName(var) || !jsvGetPrevSibling(var) ||
            jsvGetAddressOf(jsvGetPrevSibling(var))->flags==JSV_UNUSED ||
            (jsvGetAddressOf(jsvGetPrevSibling(var))->flags&JSV_GARBAGE_COLLECT));
        assert(!jsvIsName(var) || !jsvGetNextSibling(var) ||
            jsvGetAddressOf(jsvGetNextSibling(var))->flags==JSV_UNUSED ||
            (jsvGetAddressOf(jsvGetNextSibling(var))->flags&JSV_GARBAGE_COLLECT));
        var->flags = JSV_UNUSED;
        if (lastEmpty) jsvSetNextSibling(lastEmpty, i);
        else jsVarFirstEmpty = i;
        lastEmpty = var;
        freedCount++;
      }
    } else if (jsvIsFlatString(var)) {
      i = (JsVarRef)(i+jsvGetFlatStringBlocks(var));
    } else if (var->flags == JSV_UNUSED) {
      if (lastEmpty) jsvSetNextSibling(lastEmpty, i);
      else jsVarFirstEmpty = i;
      lastEmpty = var;
    }
  }
  if (lastEmpty) jsvSetNextSibling(lastEmpty, 0);
  isMemoryBusy = MEM_NOT_BUSY;
  return (int)freedCount;
}
