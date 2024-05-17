NO_INLINE void jspEnsureIsPrototype(JsVar *instanceOf, JsVar *prototypeName) {
  if (!prototypeName) return;
  JsVar *prototypeVar = jsvSkipName(prototypeName);
  if (!jsvIsObject(prototypeVar)) {
    if (!jsvIsUndefined(prototypeVar))
      jsExceptionHere(JSET_TYPEERROR, "Prototype should be an object, got %t", prototypeVar);
    jsvUnLock(prototypeVar);
    prototypeVar = jsvNewObject();  
    JsVar *lastName = jsvSkipToLastName(prototypeName);
    jsvSetValueOfName(lastName, prototypeVar);
    jsvUnLock(lastName);
  }
  JsVar *constructor = jsvFindChildFromString(prototypeVar, JSPARSE_CONSTRUCTOR_VAR, true);
  if (constructor) jsvSetValueOfName(constructor, instanceOf);
  jsvUnLock2(constructor, prototypeVar);
}