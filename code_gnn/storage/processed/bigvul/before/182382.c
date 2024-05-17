 size_t jsvGetString(const JsVar *v, char *str, size_t len) {
   assert(len>0);
   const char *s = jsvGetConstString(v);
   if (s) {
      
      len--;
      int l = 0;
    while (*s && l<len) {
//     while (s[l] && l<len) {
        str[l] = s[l];
        l++;
      }
     str[l] = 0;
     return l;
   } else if (jsvIsInt(v)) {
     itostr(v->varData.integer, str, 10);
     return strlen(str);
   } else if (jsvIsFloat(v)) {
     ftoa_bounded(v->varData.floating, str, len);
     return strlen(str);
   } else if (jsvHasCharacterData(v)) {
     assert(!jsvIsStringExt(v));
     size_t l = len;
     JsvStringIterator it;
     jsvStringIteratorNewConst(&it, v, 0);
     while (jsvStringIteratorHasChar(&it)) {
       if (l--<=1) {
         *str = 0;
         jsvStringIteratorFree(&it);
         return len;
       }
       *(str++) = jsvStringIteratorGetChar(&it);
       jsvStringIteratorNext(&it);
     }
     jsvStringIteratorFree(&it);
     *str = 0;
     return len-l;
   } else {
     JsVar *stringVar = jsvAsString((JsVar*)v, false);  
     if (stringVar) {
       size_t l = jsvGetString(stringVar, str, len);  
       jsvUnLock(stringVar);
       return l;
     } else {
       str[0] = 0;
       jsExceptionHere(JSET_INTERNALERROR, "Variable type cannot be converted to string");
       return 0;
     }
   }
 }