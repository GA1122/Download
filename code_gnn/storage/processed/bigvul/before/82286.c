JsVar *jspEvaluate(const char *str, bool stringIsStatic) {

   
  JsVar *evCode;
  if (stringIsStatic)
    evCode = jsvNewNativeString((char*)str, strlen(str));
  else
    evCode = jsvNewFromString(str);
  if (!evCode) return 0;

  JsVar *v = 0;
  if (!jsvIsMemoryFull())
    v = jspEvaluateVar(evCode, 0, 0);
  jsvUnLock(evCode);

  return v;
}
