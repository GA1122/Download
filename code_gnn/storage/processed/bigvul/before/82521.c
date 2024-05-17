static void NO_INLINE jslGetNextCh() {
  lex->currCh = jslNextCh();

   
  lex->it.charIdx++;
  if (lex->it.charIdx >= lex->it.charsInVar) {
    lex->it.charIdx -= lex->it.charsInVar;
    if (lex->it.var && jsvGetLastChild(lex->it.var)) {
      lex->it.var = _jsvGetAddressOf(jsvGetLastChild(lex->it.var));
      lex->it.ptr = &lex->it.var->varData.str[0];
      lex->it.varIndex += lex->it.charsInVar;
      lex->it.charsInVar = jsvGetCharactersInVar(lex->it.var);
    } else {
      lex->it.var = 0;
      lex->it.ptr = 0;
      lex->it.varIndex += lex->it.charsInVar;
      lex->it.charsInVar = 0;
    }
  }
}
