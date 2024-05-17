ALWAYS_INLINE void jsvFreePtr(JsVar *var) {
   
  assert((!jsvGetNextSibling(var) && !jsvGetPrevSibling(var)) ||  
      jsvIsRefUsedForData(var) ||   
      (jsvIsName(var) && (jsvGetNextSibling(var)==jsvGetPrevSibling(var))));  

  if (jsvIsNameWithValue(var)) {
#ifdef CLEAR_MEMORY_ON_FREE
    jsvSetFirstChild(var, 0);  
#endif  
  } else if (jsvHasSingleChild(var)) {
    if (jsvGetFirstChild(var)) {
      JsVar *child = jsvLock(jsvGetFirstChild(var));
      jsvUnRef(child);
#ifdef CLEAR_MEMORY_ON_FREE
      jsvSetFirstChild(var, 0);  
#endif  
      jsvUnLock(child);  
    }
  }
   

   
  if (jsvHasStringExt(var)) {
    JsVarRef stringDataRef = jsvGetLastChild(var);
#ifdef CLEAR_MEMORY_ON_FREE
    jsvSetLastChild(var, 0);
#endif  
    while (stringDataRef) {
      JsVar *child = jsvGetAddressOf(stringDataRef);
      assert(jsvIsStringExt(child));
      stringDataRef = jsvGetLastChild(child);
      jsvFreePtrInternal(child);
    }
    if (jsvIsFlatString(var)) {
      size_t count = jsvGetFlatStringBlocks(var);
      JsVarRef i = (JsVarRef)(jsvGetRef(var)+count);
      while (count--) {
        JsVar *p = jsvGetAddressOf(i--);
        p->flags = JSV_UNUSED;  
        jsvFreePtrInternal(p);
      }
    } else if (jsvIsBasicString(var)) {
#ifdef CLEAR_MEMORY_ON_FREE
      jsvSetFirstChild(var, 0);  
#endif  
    }

  }
   

  if (jsvHasChildren(var)) {
    JsVarRef childref = jsvGetFirstChild(var);
#ifdef CLEAR_MEMORY_ON_FREE
    jsvSetFirstChild(var, 0);
    jsvSetLastChild(var, 0);
#endif  
    while (childref) {
      JsVar *child = jsvLock(childref);
      assert(jsvIsName(child));
      childref = jsvGetNextSibling(child);
      jsvSetPrevSibling(child, 0);
      jsvSetNextSibling(child, 0);
      jsvUnRef(child);
      jsvUnLock(child);
    }
  } else {
#ifdef CLEAR_MEMORY_ON_FREE
#if JSVARREF_SIZE==1
    assert(jsvIsFloat(var) || !jsvGetFirstChild(var));
    assert(jsvIsFloat(var) || !jsvGetLastChild(var));
#else
    assert(!jsvGetFirstChild(var));  
    assert(!jsvGetLastChild(var));
#endif
#endif  
    if (jsvIsName(var)) {
      assert(jsvGetNextSibling(var)==jsvGetPrevSibling(var));  
      if (jsvGetNextSibling(var)) {
        jsvUnRefRef(jsvGetNextSibling(var));
        jsvUnRefRef(jsvGetPrevSibling(var));
      }
    }
  }

  jsvFreePtrInternal(var);
}
