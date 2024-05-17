bool jsvIsStringEqualOrStartsWithOffset(JsVar *var, const char *str, bool isStartsWith, size_t startIdx, bool ignoreCase) {
  if (!jsvHasCharacterData(var)) {
    return 0;  
  }

  JsvStringIterator it;
  jsvStringIteratorNew(&it, var, startIdx);
  if (ignoreCase) {
      while (jsvStringIteratorHasChar(&it) && *str &&
             jsvStringCharToLower(jsvStringIteratorGetChar(&it)) == jsvStringCharToLower(*str)) {
        str++;
        jsvStringIteratorNext(&it);
      }
  } else {
      while (jsvStringIteratorHasChar(&it) && *str &&
             jsvStringIteratorGetChar(&it) == *str) {
        str++;
        jsvStringIteratorNext(&it);
      }
  }
  bool eq = (isStartsWith && !*str) ||
            jsvStringIteratorGetChar(&it)==*str;  
  jsvStringIteratorFree(&it);
  return eq;
}
