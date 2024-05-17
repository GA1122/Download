void jslSeekTo(size_t seekToChar) {
  if (lex->it.var) jsvLockAgain(lex->it.var);  
  jsvStringIteratorFree(&lex->it);
  jsvStringIteratorNew(&lex->it, lex->sourceVar, seekToChar);
  jsvUnLock(lex->it.var);  
  lex->tokenStart.it.var = 0;
  lex->tokenStart.currCh = 0;
  jslPreload();
}
