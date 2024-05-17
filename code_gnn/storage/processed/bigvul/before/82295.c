JsVar *jspGetNamedVariable(const char *tokenName) {
  JsVar *a = JSP_SHOULD_EXECUTE ? jspeiFindInScopes(tokenName) : 0;
  if (JSP_SHOULD_EXECUTE && !a) {
     
    if (jswIsBuiltInObject(tokenName)) {
      JsVar *obj = jswFindBuiltInFunction(0, tokenName);
      if (!obj)
        obj = jspNewBuiltin(tokenName);
      if (obj) {  
        a = jsvAddNamedChild(execInfo.root, obj, tokenName);
        jsvUnLock(obj);
      }
    } else {
      a = jswFindBuiltInFunction(0, tokenName);
      if (!a) {
         
        a = jsvMakeIntoVariableName(jsvNewFromString(tokenName), 0);
      }
    }
  }
  return a;
}
