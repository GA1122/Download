JsVar *jsvArrayPopFirst(JsVar *arr) {
  assert(jsvIsArray(arr));
  if (jsvGetFirstChild(arr)) {
    JsVar *child = jsvLock(jsvGetFirstChild(arr));
    if (jsvGetFirstChild(arr) == jsvGetLastChild(arr))
      jsvSetLastChild(arr, 0);  
    jsvSetFirstChild(arr, jsvGetNextSibling(child));  
    jsvUnRef(child);  
    if (jsvGetNextSibling(child)) {
      JsVar *v = jsvLock(jsvGetNextSibling(child));
      jsvSetPrevSibling(v, 0);
      jsvUnLock(v);
    }
    jsvSetNextSibling(child, 0);
    return child;  
  } else {
    return 0;
  }
}
