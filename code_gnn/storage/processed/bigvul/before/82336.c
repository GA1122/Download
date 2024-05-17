NO_INLINE JsVar *jspePostfixExpression() {
  JsVar *a;
  if (lex->tk==LEX_PLUSPLUS || lex->tk==LEX_MINUSMINUS) {
    int op = lex->tk;
    JSP_ASSERT_MATCH(op);
    a = jspePostfixExpression();
    if (JSP_SHOULD_EXECUTE) {
      JsVar *one = jsvNewFromInteger(1);
      JsVar *res = jsvMathsOpSkipNames(a, one, op==LEX_PLUSPLUS ? '+' : '-');
      jsvUnLock(one);
      jspReplaceWith(a, res);
      jsvUnLock(res);
    }
  } else
    a = jspeFactorFunctionCall();
  return __jspePostfixExpression(a);
}
