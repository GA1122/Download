JsVar *jspGetVarNamedField(JsVar *object, JsVar *nameVar, bool returnName) {

  JsVar *child = 0;
  if (jsvHasChildren(object))
    child = jsvFindChildFromVar(object, nameVar, false);

  if (!child) {
    if (jsvIsArrayBuffer(object) && jsvIsInt(nameVar)) {
      child = jsvMakeIntoVariableName(jsvNewFromInteger(jsvGetInteger(nameVar)), object);
      if (child)  
        child->flags = (child->flags & ~JSV_VARTYPEMASK) | JSV_ARRAYBUFFERNAME;
    } else if (jsvIsString(object) && jsvIsInt(nameVar)) {
      JsVarInt idx = jsvGetInteger(nameVar);
      if (idx>=0 && idx<(JsVarInt)jsvGetStringLength(object)) {
        char ch = jsvGetCharInString(object, (size_t)idx);
        child = jsvNewStringOfLength(1, &ch);
      } else if (returnName)
        child = jsvCreateNewChild(object, nameVar, 0);  
    } else {
      char name[JSLEX_MAX_TOKEN_LENGTH];
      jsvGetString(nameVar, name, JSLEX_MAX_TOKEN_LENGTH);
      child = jspGetNamedFieldInParents(object, name, returnName);

      if (!child && jsvIsFunction(object) && jsvIsStringEqual(nameVar, JSPARSE_PROTOTYPE_VAR)) {
        JsVar *value = jsvNewObject();  
        child = jsvAddNamedChild(object, value, JSPARSE_PROTOTYPE_VAR);
        jsvUnLock(value);
      }
    }
  }

  if (returnName) return child;
  else return jsvSkipNameAndUnLock(child);
}
