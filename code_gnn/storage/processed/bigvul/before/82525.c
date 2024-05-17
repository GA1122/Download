JsVar *jslGetTokenValueAsVar() {
  if (lex->tokenValue) {
    return jsvLockAgain(lex->tokenValue);
  } else {
    assert(lex->tokenl < JSLEX_MAX_TOKEN_LENGTH);
    lex->token[lex->tokenl]  = 0;  
    return jsvNewFromString(lex->token);
  }
}
