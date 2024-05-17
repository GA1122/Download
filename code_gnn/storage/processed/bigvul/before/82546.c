static ALWAYS_INLINE void jslTokenAppendChar(char ch) {
   
  if (lex->tokenl < JSLEX_MAX_TOKEN_LENGTH-1) {
    lex->token[lex->tokenl++] = ch;
  }
}
