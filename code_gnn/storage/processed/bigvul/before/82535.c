JsVar *jslNewStringFromLexer(JslCharPos *charFrom, size_t charTo) {
  size_t maxLength = charTo + 1 - jsvStringIteratorGetIndex(&charFrom->it);
  assert(maxLength>0);  
  JsVar *var = 0;
  if (maxLength > JSV_FLAT_STRING_BREAK_EVEN) {
    var = jsvNewFlatStringOfLength((unsigned int)maxLength);
    if (var) {
      char *flatPtr = jsvGetFlatStringPointer(var);
      *(flatPtr++) = charFrom->currCh;
      JsvStringIterator it = jsvStringIteratorClone(&charFrom->it);
      while (jsvStringIteratorHasChar(&it) && (--maxLength>0)) {
        *(flatPtr++) = jsvStringIteratorGetChar(&it);
        jsvStringIteratorNext(&it);
      }
      jsvStringIteratorFree(&it);
      return var;
    }
  }
  var = jsvNewFromEmptyString();
  if (!var) {  
    return 0;
  }

  JsVar *block = jsvLockAgain(var);
  block->varData.str[0] = charFrom->currCh;
  size_t blockChars = 1;

  size_t l = maxLength;
  JsvStringIterator it = jsvStringIteratorClone(&charFrom->it);
  while (jsvStringIteratorHasChar(&it) && (--maxLength>0)) {
    char ch = jsvStringIteratorGetChar(&it);
    if (blockChars >= jsvGetMaxCharactersInVar(block)) {
      jsvSetCharactersInVar(block, blockChars);
      JsVar *next = jsvNewWithFlags(JSV_STRING_EXT_0);
      if (!next) break;  
      jsvSetLastChild(block, jsvGetRef(next));
      jsvUnLock(block);
      block = next;
      blockChars=0;  
    }
    block->varData.str[blockChars++] = ch;
    jsvStringIteratorNext(&it);
  }
  jsvSetCharactersInVar(block, blockChars);
  jsvUnLock(block);
  assert((l == jsvGetStringLength(var)) || (jsErrorFlags&JSERR_MEMORY) || !jsvStringIteratorHasChar(&it));
  jsvStringIteratorFree(&it);


  return var;
}
