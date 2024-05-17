JsVar *jsvFindChildFromString(JsVar *parent, const char *name, bool addIfNotFound) {
   
  char fastCheck[4];
  fastCheck[0] = name[0];
  if (name[0]) {
    fastCheck[1] = name[1];
    if (name[1]) {
      fastCheck[2] = name[2];
      if (name[2]) {
        fastCheck[3] = name[3];
      } else {
        fastCheck[3] = 0;
      }
    } else {
      fastCheck[2] = 0;
      fastCheck[3] = 0;
    }
  } else {
    fastCheck[1] = 0;
    fastCheck[2] = 0;
    fastCheck[3] = 0;
  }

  assert(jsvHasChildren(parent));
  JsVarRef childref = jsvGetFirstChild(parent);
  while (childref) {
    JsVar *child = jsvGetAddressOf(childref);
    if (*(int*)fastCheck==*(int*)child->varData.str &&  
        jsvIsStringEqual(child, name)) {
      return jsvLockAgain(child);
    }
    childref = jsvGetNextSibling(child);
  }

  JsVar *child = 0;
  if (addIfNotFound) {
    child = jsvMakeIntoVariableName(jsvNewFromString(name), 0);
    if (child)  
      jsvAddName(parent, child);
  }
  return child;
}
