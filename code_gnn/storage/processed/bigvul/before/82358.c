void jsvAppendStringBuf(JsVar *var, const char *str, size_t length) {
  assert(jsvIsString(var));
  JsvStringIterator dst;
  jsvStringIteratorNew(&dst, var, 0);
  jsvStringIteratorGotoEnd(&dst);
   
  while (length) {
    jsvStringIteratorAppend(&dst, *(str++));
    length--;
  }
  jsvStringIteratorFree(&dst);
}
