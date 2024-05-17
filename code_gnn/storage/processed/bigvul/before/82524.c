char *jslGetTokenValueAsString() {
  assert(lex->tokenl < JSLEX_MAX_TOKEN_LENGTH);
  lex->token[lex->tokenl]  = 0;  
  return lex->token;
}
