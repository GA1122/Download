JsVar *jslNewTokenisedStringFromLexer(JslCharPos *charFrom, size_t charTo) {
  JsLex *oldLex = lex;
  JsLex newLex;
  lex = &newLex;
  size_t length = 0;
  jslInit(oldLex->sourceVar);
  jslSeekToP(charFrom);
  int lastTk = LEX_EOF;
  while (lex->tk!=LEX_EOF && jsvStringIteratorGetIndex(&lex->it)<=charTo+1) {
    if ((lex->tk==LEX_ID || lex->tk==LEX_FLOAT || lex->tk==LEX_INT) &&
        ( lastTk==LEX_ID ||  lastTk==LEX_FLOAT ||  lastTk==LEX_INT)) {
      jsExceptionHere(JSET_SYNTAXERROR, "ID/number following ID/number isn't valid JS");
      length = 0;
      break;
    }
    if (lex->tk==LEX_ID ||
        lex->tk==LEX_INT ||
        lex->tk==LEX_FLOAT ||
        lex->tk==LEX_STR ||
        lex->tk==LEX_TEMPLATE_LITERAL) {
      length += jsvStringIteratorGetIndex(&lex->it)-jsvStringIteratorGetIndex(&lex->tokenStart.it);
    } else {
      length++;
    }
    lastTk = lex->tk;
    jslGetNextToken();
  }

  JsVar *var = jsvNewStringOfLength((unsigned int)length, NULL);
  if (var) {  
    JsvStringIterator dstit;
    jsvStringIteratorNew(&dstit, var, 0);
    jslSeekToP(charFrom);
    while (lex->tk!=LEX_EOF && jsvStringIteratorGetIndex(&lex->it)<=charTo+1) {
      if (lex->tk==LEX_ID ||
          lex->tk==LEX_INT ||
          lex->tk==LEX_FLOAT ||
          lex->tk==LEX_STR ||
          lex->tk==LEX_TEMPLATE_LITERAL) {
        jsvStringIteratorSetCharAndNext(&dstit, lex->tokenStart.currCh);
        JsvStringIterator it = jsvStringIteratorClone(&lex->tokenStart.it);
        while (jsvStringIteratorGetIndex(&it)+1 < jsvStringIteratorGetIndex(&lex->it)) {
          jsvStringIteratorSetCharAndNext(&dstit, jsvStringIteratorGetChar(&it));
          jsvStringIteratorNext(&it);
        }
        jsvStringIteratorFree(&it);
      } else {
        jsvStringIteratorSetCharAndNext(&dstit, (char)lex->tk);
      }
      lastTk = lex->tk;
      jslGetNextToken();
    }
    jsvStringIteratorFree(&dstit);
  }
  jslKill();
  lex = oldLex;

  return var;
}