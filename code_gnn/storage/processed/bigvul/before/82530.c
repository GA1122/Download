static void jslLexRegex() {
  lex->tokenValue = jsvNewFromEmptyString();
  if (!lex->tokenValue) {
    lex->tk = LEX_EOF;
    return;
  }
  JsvStringIterator it;
  jsvStringIteratorNew(&it, lex->tokenValue, 0);
  jsvStringIteratorAppend(&it, '/');
  jslGetNextCh();
  while (lex->currCh && lex->currCh!='/') {
    if (lex->currCh == '\\') {
      jsvStringIteratorAppend(&it, lex->currCh);
      jslGetNextCh();
    } else if (lex->currCh=='\n') {
       
      break;
    }
    jsvStringIteratorAppend(&it, lex->currCh);
    jslGetNextCh();
  }
  lex->tk = LEX_REGEX;
  if (lex->currCh!='/') {
    lex->tk++;  
  } else {
    jsvStringIteratorAppend(&it, '/');
    jslGetNextCh();
    while (lex->currCh=='g' ||
        lex->currCh=='i' ||
        lex->currCh=='m' ||
        lex->currCh=='y' ||
        lex->currCh=='u') {
      jslTokenAppendChar(lex->currCh);
      jsvStringIteratorAppend(&it, lex->currCh);
      jslGetNextCh();
    }
  }
  jsvStringIteratorFree(&it);
}
