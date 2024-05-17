void jslSeekToP(JslCharPos *seekToChar) {
  if (lex->it.var) jsvLockAgain(lex->it.var);  
  jsvStringIteratorFree(&lex->it);
  lex->it = jsvStringIteratorClone(&seekToChar->it);
  jsvUnLock(lex->it.var);  
  lex->currCh = seekToChar->currCh;
  lex->tokenStart.it.var = 0;
  lex->tokenStart.currCh = 0;
  jslGetNextToken();
}
