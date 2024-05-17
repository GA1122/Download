NO_INLINE JsVar *jspeFactorTypeOf() {
  JSP_ASSERT_MATCH(LEX_R_TYPEOF);
  JsVar *a = jspeUnaryExpression();
  JsVar *result = 0;
  if (JSP_SHOULD_EXECUTE) {
    if (!jsvIsVariableDefined(a)) {
      result=jsvNewFromString("undefined");
    } else {
      a = jsvSkipNameAndUnLock(a);
      result=jsvNewFromString(jsvGetTypeOf(a));
    }
  }
  jsvUnLock(a);
  return result;
}
