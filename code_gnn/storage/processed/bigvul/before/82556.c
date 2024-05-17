void jspReplaceWith(JsVar *dst, JsVar *src) {
  if (jsvIsArrayBufferName(dst)) {
    size_t idx = (size_t)jsvGetInteger(dst);
    JsVar *arrayBuffer = jsvLock(jsvGetFirstChild(dst));
    jsvArrayBufferSet(arrayBuffer, idx, src);
    jsvUnLock(arrayBuffer);
    return;
  }
  if (!jsvIsName(dst)) {
    jsExceptionHere(JSET_ERROR, "Unable to assign value to non-reference %t", dst);
    return;
  }
  jsvSetValueOfName(dst, src);
   
  if (jsvIsNewChild(dst)) {
    JsVar *parent = jsvLock(jsvGetNextSibling(dst));
    if (!jsvIsString(parent)) {
      if (!jsvHasChildren(parent)) {
        jsExceptionHere(JSET_ERROR, "Field or method \"%s\" does not already exist, and can't create it on %t", dst, parent);
      } else {
        jsvUnRef(parent);
        jsvSetNextSibling(dst, 0);
        jsvUnRef(parent);
        jsvSetPrevSibling(dst, 0);
        jsvAddName(parent, dst);
      }
    }
    jsvUnLock(parent);
  }
}
