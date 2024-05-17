static NO_INLINE JsVar *jspGetNamedFieldInParents(JsVar *object, const char* name, bool returnName) {
  JsVar * child = jspeiFindChildFromStringInParents(object, name);

   
  if (!child) {
    child = jswFindBuiltInFunction(object, name);
  }

   
  if (child && returnName) {
    child = jsvSkipNameAndUnLock(child);
    JsVar *nameVar = jsvNewFromString(name);
    JsVar *newChild = jsvCreateNewChild(object, nameVar, child);
    jsvUnLock2(nameVar, child);
    child = newChild;
  }

  if (!child) {
    if (jsvIsFunction(object) && strcmp(name, JSPARSE_PROTOTYPE_VAR)==0) {
      JsVar *proto = jsvNewObject();
      jsvObjectSetChild(proto, JSPARSE_CONSTRUCTOR_VAR, object);
      child = jsvAddNamedChild(object, proto, JSPARSE_PROTOTYPE_VAR);
      jspEnsureIsPrototype(object, child);
      jsvUnLock(proto);
    } else if (strcmp(name, JSPARSE_INHERITS_VAR)==0) {
      const char *objName = jswGetBasicObjectName(object);
      if (objName) {
        child = jspNewPrototype(objName);
      }
    }
  }

  return child;
}
