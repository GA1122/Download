JsVar *jspEvaluateExpressionVar(JsVar *str) {
  JsLex lex;

  assert(jsvIsString(str));
  JsLex *oldLex = jslSetLex(&lex);
  jslInit(str);
  lex.lineNumberOffset = oldLex->lineNumberOffset;

  JsVar *v = jspeExpression();
  jslKill();
  jslSetLex(oldLex);

  return jsvSkipNameAndUnLock(v);
}