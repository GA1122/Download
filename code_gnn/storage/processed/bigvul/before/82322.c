NO_INLINE JsVar *jspeExpression() {
  while (!JSP_SHOULDNT_PARSE) {
    JsVar *a = jspeAssignmentExpression();
    if (lex->tk!=',') return a;
    jsvCheckReferenceError(a);
    jsvUnLock(a);
    JSP_ASSERT_MATCH(',');
  }
  return 0;
}
