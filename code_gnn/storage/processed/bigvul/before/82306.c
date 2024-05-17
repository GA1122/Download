NO_INLINE JsVar *jspNewPrototype(const char *instanceOf) {
  JsVar *objFuncName = jsvFindChildFromString(execInfo.root, instanceOf, true);
  if (!objFuncName)  
    return 0;

  JsVar *objFunc = jsvSkipName(objFuncName);
  if (!objFunc) {
    objFunc = jspNewBuiltin(instanceOf);
    if (!objFunc) {  
      jsvUnLock(objFuncName);
      return 0;
    }

    jsvSetValueOfName(objFuncName, objFunc);
  }

  JsVar *prototypeName = jsvFindChildFromString(objFunc, JSPARSE_PROTOTYPE_VAR, true);
  jspEnsureIsPrototype(objFunc, prototypeName);  
  jsvUnLock2(objFunc, objFuncName);

  return prototypeName;
}
