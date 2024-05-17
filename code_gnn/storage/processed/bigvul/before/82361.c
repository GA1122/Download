JsVarInt jsvArrayAddToEnd(JsVar *arr, JsVar *value, JsVarInt initialValue) {
  assert(jsvIsArray(arr));
  JsVarInt index = initialValue;
  if (jsvGetLastChild(arr)) {
    JsVar *last = jsvLock(jsvGetLastChild(arr));
    index = jsvGetInteger(last)+1;
    jsvUnLock(last);
  }

  JsVar *idx = jsvMakeIntoVariableName(jsvNewFromInteger(index), value);
  if (!idx) return 0;  
  jsvAddName(arr, idx);
  jsvUnLock(idx);
  return index+1;
}
