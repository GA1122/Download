JsVar *jspGetNamedField(JsVar *object, const char* name, bool returnName) {

  JsVar *child = 0;
  if (jsvHasChildren(object))
    child = jsvFindChildFromString(object, name, false);

  if (!child) {
    child = jspGetNamedFieldInParents(object, name, returnName);

    if (!child && jsvIsFunction(object) && strcmp(name, JSPARSE_PROTOTYPE_VAR)==0) {
      JsVar *value = jsvNewObject();  
      child = jsvAddNamedChild(object, value, JSPARSE_PROTOTYPE_VAR);
      jsvUnLock(value);
    }
  }

  if (returnName) return child;
  else return jsvSkipNameAndUnLock(child);
}
