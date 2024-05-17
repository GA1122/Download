void jsvArrayAddUnique(JsVar *arr, JsVar *v) {
  JsVar *idx = jsvGetIndexOf(arr, v, false);  
  if (!idx) {
    jsvArrayPush(arr, v);  
  } else {
    jsvUnLock(idx);
  }
}
