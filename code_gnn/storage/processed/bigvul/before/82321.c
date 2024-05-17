NO_INLINE JsVar *jspeConstruct(JsVar *func, JsVar *funcName, bool hasArgs) {
  assert(JSP_SHOULD_EXECUTE);
  if (!jsvIsFunction(func)) {
    jsExceptionHere(JSET_ERROR, "Constructor should be a function, but is %t", func);
    return 0;
  }

  JsVar *thisObj = jsvNewObject();
  if (!thisObj) return 0;  
  JsVar *prototypeName = jsvFindChildFromString(func, JSPARSE_PROTOTYPE_VAR, true);
  jspEnsureIsPrototype(func, prototypeName);  
  JsVar *prototypeVar = jsvSkipName(prototypeName);
  jsvUnLock3(jsvAddNamedChild(thisObj, prototypeVar, JSPARSE_INHERITS_VAR), prototypeVar, prototypeName);

  JsVar *a = jspeFunctionCall(func, funcName, thisObj, hasArgs, 0, 0);

   
  if (a) {
    jsvUnLock(thisObj);
    thisObj = a;
  } else {
    jsvUnLock(a);
  }
  return thisObj;
}
