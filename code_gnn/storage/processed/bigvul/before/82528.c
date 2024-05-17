static bool jslIsToken(const char *token, int startOffset) {
  int i;
  for (i=startOffset;i<lex->tokenl;i++) {
    if (lex->token[i]!=token[i]) return false;
  }
  return token[lex->tokenl] == 0;  
}
