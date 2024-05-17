void jsvAppendString(JsVar *var, const char *str) {
  assert(jsvIsString(var));
  JsvStringIterator dst;
  jsvStringIteratorNew(&dst, var, 0);
  jsvStringIteratorGotoEnd(&dst);
   
  while (*str)
    jsvStringIteratorAppend(&dst, *(str++));
  jsvStringIteratorFree(&dst);
}
