NO_INLINE JsVar *__jspeConditionalExpression(JsVar *lhs) {
  if (lex->tk=='?') {
    JSP_ASSERT_MATCH('?');
    if (!JSP_SHOULD_EXECUTE) {
      jsvUnLock(jspeAssignmentExpression());
      JSP_MATCH(':');
      jsvUnLock(jspeAssignmentExpression());
    } else {
      bool first = jsvGetBoolAndUnLock(jsvSkipName(lhs));
      jsvUnLock(lhs);
      if (first) {
        lhs = jspeAssignmentExpression();
        JSP_MATCH(':');
        JSP_SAVE_EXECUTE();
        jspSetNoExecute();
        jsvUnLock(jspeAssignmentExpression());
        JSP_RESTORE_EXECUTE();
      } else {
        JSP_SAVE_EXECUTE();
        jspSetNoExecute();
        jsvUnLock(jspeAssignmentExpression());
        JSP_RESTORE_EXECUTE();
        JSP_MATCH(':');
        lhs = jspeAssignmentExpression();
      }
    }
  }

  return lhs;
}