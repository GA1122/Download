NO_INLINE void jspeSkipBlock() {
  int brackets = 1;
  while (lex->tk && brackets) {
    if (lex->tk == '{') brackets++;
    else if (lex->tk == '}') {
      brackets--;
      if (!brackets) return;
    }
    JSP_ASSERT_MATCH(lex->tk);
  }
}
