void jslKill() {
  lex->tk = LEX_EOF;  
  if (lex->it.var) jsvLockAgain(lex->it.var);  
  jsvStringIteratorFree(&lex->it);
  if (lex->tokenValue) {
    jsvUnLock(lex->tokenValue);
    lex->tokenValue = 0;
  }
  jsvUnLock(lex->sourceVar);
  lex->tokenStart.it.var = 0;
  lex->tokenStart.currCh = 0;
}
